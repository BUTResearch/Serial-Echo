#include "sara_n210.h"
#include <regex>

std::string ok_resp = "OK";
std::string modem_prefix = "[SARA_N210] ";

SARA_N210::SARA_N210(Serial & ser)
{
     this->_ser = &ser; 
     this->URC_cscon    = false;
     this->URC_downlink = false;
}

bool SARA_N210::isDownlinkInQueque()
{
    return this->URC_downlink;
}

void SARA_N210::debug(std::string text)
{
    std::cout << modem_prefix << text << std::endl;
}

std::string SARA_N210::readString()
{   
    std::string reply;

    reply = _ser->readString();

    // Just check if some Unexpected Result Code messsage received
    this->checkForURC(reply);

    return reply;
}

bool SARA_N210::readDownlink(int socket, downlink_t & downlink)
{  
    std::string payload;
    int length = 256;
    std::string command = "AT+NSORF=" + std::to_string(socket) + "," + std::to_string(length);
    this->URC_downlink = false;
    std::string response = sendCommandWithExpectedAnswer(command, ok_resp, true);

    if(checkForOK(response))
    {
        // Use rexex to find the IMSI
        std::smatch match;
        std::regex pattern("\\r\\n([0-9]),\"([0-9.]+)\",([0-9]+),([0-9]+),\"([0-f0-F]+)\",([0-9]+)\\r\\n\\r\\nOK\\r\\n");

        if (std::regex_match(response, match, pattern))
        {
            // Select the second submatch
            downlink.socket         = stoi(match[1].str());
            downlink.ip_addr        = match[2].str();
            downlink.port           = stoi(match[3].str());
            downlink.payload_length = stoi(match[4].str());
            downlink.payload        = match[5].str();
            
            /*
            this->debug("socket : " + match[1].str());
            this->debug("ip_add : " + match[2].str());
            this->debug("port   : " + match[3].str());
            this->debug("pay_len: " + match[4].str());
            this->debug("payload: " + match[5].str());
            */
        }
    }

    return checkForOK(response);
}

bool SARA_N210::isCSCONTrue()
{
    return this->URC_cscon;
}

void SARA_N210::checkForURC(std::string text)
{   
    // Check for downlink
    if(text.find("+NSONMI") != std::string::npos)
    {
        this->debug("Downlink received");
        this->URC_downlink = true;
    }
    
    // Check for CSCON: 1
    if(text.find("+CSCON: 1") != std::string::npos)
    {
        this->debug("+CSCON: 1 received");
        this->URC_cscon = true;
    }

    // Check for CSCON: 0
    if(text.find("+CSCON: 0") != std::string::npos)
    {
        this->debug("+CSCON: 0 received");
        this->URC_cscon = false;
    }   
}

size_t SARA_N210::sendString(std::string command)
{
    // Then do the regular task
    return _ser->println(command);
}

bool SARA_N210::receiveExpectAnswer(std::string expect, std::string &reply, bool debug_on)
{
    std::string curr_reply;
    reply.clear();
    
    int max_read_serial_loops = 10;

    // Protect the program from endles loop circulating
    while(max_read_serial_loops)
    {
        // Decrement the max_read_serial_loops number by one
        max_read_serial_loops--;

        curr_reply = this->readString();
        //this->debug(reply);
        
        if(debug_on == true)
        {
            this->debug(curr_reply);
        }

        // Add the newly received string to the reply string
        reply += curr_reply;

        if(reply.find(expect) != std::string::npos)
        {
            // Return the string that is collected from the all received strings
            return true;
        }

        if(reply.find("ERROR") != std::string::npos)
        {
            return false;
        }
    }

    return false;
}

std::string SARA_N210::sendCommandWithExpectedAnswer(std::string command, std::string expect, bool debug_on)
{
    std::string reply;
    bool status;

    if(debug_on == true)
    {
        this->debug(command);
    }

    this->sendString(command);

    status = this->receiveExpectAnswer(expect, reply, debug_on);

    if(status != true)
    {
        this->debug("function call this->receiveExpectAnswer() didnt end well");
    }

    usleep(300000);

    return reply;
}


std::string SARA_N210::sendCommand(std::string command, bool debug_on)
{
    std::string reply;
    
    if(debug_on == true)
    {
        this->debug(command);
    }

    this->sendString(command);

    reply = this->readString();
    
    if(debug_on == true)
    {
        this->debug(reply);
    }

    usleep(300000);

    return reply;
}

bool SARA_N210::checkForOK(std::string const & text)
{
    if(text.find("OK") != std::string::npos) 
    { 
        return true; 
    } 

    return false;
}

bool SARA_N210::isModemOk()
{
    return checkForOK(sendCommandWithExpectedAnswer("AT", ok_resp, false));
}

std::string SARA_N210::toBinary(int number, int min_bits_count)
{
    std::string r;
    while(number!=0)
    {
        r=(number%2==0 ?"0":"1")+r; number/=2;
    }

    if(r.length() > min_bits_count)
    {
        return r;
    }

    int missing_pos = min_bits_count - r.length();

    for(int i = 0; i < missing_pos; i++)
    {
        r.insert(0, "0");
    }

    return r;
}

bool SARA_N210::setCFUN(bool on)
{
    std::string command = "AT+CFUN=" + std::to_string(on);

    return checkForOK(sendCommandWithExpectedAnswer(command, ok_resp, true));
}

bool SARA_N210::setCSCON(bool on)
{
    std::string command = "AT+CSCON=" + std::to_string(on);

    return checkForOK(sendCommandWithExpectedAnswer(command, ok_resp, true));
}

bool SARA_N210::setPDPContext(std::string pdp_type, std::string apn_name)
{
    return checkForOK(sendCommandWithExpectedAnswer("AT+CGDCONT=1,\""+ pdp_type +"\",\"" + apn_name + "\",0,0", ok_resp, true));
}

bool SARA_N210::registerToOperator(std::string op_name)
{
    return checkForOK(sendCommandWithExpectedAnswer("AT+COPS=1,2,\"" + op_name + "\"", ok_resp, true));
}

std::string SARA_N210::getIMSI()
{   
    std::string IMSI = "";

    std::string reply = sendCommand("AT+CIMI", true);
    
    if(checkForOK(reply))
    {
        // Use rexex to find the IMSI
        std::smatch match;
        std::regex pattern("\\r\\n([0-F]+)\\r\\n\\r\\nOK\\r\\n");

        if (std::regex_match(reply, match, pattern))
        {
            // Select the second submatch
            std::ssub_match sub_match = match[1];
            IMSI = sub_match.str();
        }
    }

    return IMSI;
}

bool SARA_N210::waitForURC()
{
    std::string response = this->readString();
        
    if(response.length() > 0)
    {
        // Hotfix
        if((response.find("+NSONMI:") != std::string::npos) && (response.find("OK") == std::string::npos))
        {
            std::string appendix;
            appendix = this->readString();
            response += appendix;
        }

        this->debug(response);
        return true;
    }

    return false;
}

int SARA_N210::createUDPSocket(int port_num, int & socket)
{
    std::string reply;
    std::string socket_num_string = "";
    
    reply = sendCommandWithExpectedAnswer("AT+NSOCR=\"DGRAM\",17," + std::to_string(port_num) + ",1", ok_resp, true);

    // The response should be complete now
    if(checkForOK(reply) == true)
    {
        // Use regex to find the IMSI
        std::smatch match;
        std::regex pattern("\\r\\n([0-9]+)\\r\\n\\r\\nOK\\r\\n");

        if (std::regex_match(reply, match, pattern))
        {
            // Select the second submatch
            std::ssub_match sub_match = match[1];
            socket_num_string = sub_match.str();

            socket = stoi(socket_num_string);

            return true;
        }
    }
   
    return false;
}

bool SARA_N210::sendPacket(std::string dest_ip_addr, int dest_port, std::string payload)
{
    std::string command = "AT+NSOST=0,\"" + dest_ip_addr + "\"," + std::to_string(dest_port) + ",2,\"" + payload + "\"";
    return checkForOK(sendCommandWithExpectedAnswer(command, ok_resp, true));
}