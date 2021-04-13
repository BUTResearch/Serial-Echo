#include "sara_n210.h"
#include <regex>

std::string modem_prefix = "[SARA_N210] ";

SARA_N210::SARA_N210(Serial & ser)
{
     _ser = &ser; 
}

std::string SARA_N210::send_command(std::string command, bool command_debug_on,  bool reply_debug_on)
{
    std::string reply;
    
    if(command_debug_on == true)
    {
        std::cout << modem_prefix << command << std::endl;
    }

    _ser->println(command); 
    reply = _ser->readString();
    //std::cout << "modem_prefix << Received: " << reply << std::endl;
    
    if(reply_debug_on == true)
    {
        std::cout << reply << std::endl;
    }

    usleep(300000);

    return reply;
}

bool SARA_N210::check_for_OK(std::string const & text)
{
    if(text.find("OK") != std::string::npos) 
    { 
        return true; 
    } 

    return false;
}

bool SARA_N210::is_modem_modem_ok()
{
    //std::string reply = ;
    return check_for_OK(send_command("AT", true, true));
}

std::string SARA_N210::get_IMSI()
{   
    std::string reply = send_command("AT+CIMI", true, true);
    
    std::string IMSI = "";

    if(check_for_OK(reply))
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

bool SARA_N210::setCFUN(bool on)
{
    std::string command = "AT+CFUN=0";

    if(on)
    {
        command = "AT+CFUN=1";
    }

    return check_for_OK(send_command(command, true, true));
}


bool SARA_N210::setCSCON(bool on)
{
    std::string command = "AT+CSCON=0";

    if(on)
    {
        command = "AT+CSCON=1";
    }

    return check_for_OK(send_command(command, true, true));
}

bool SARA_N210::setPDPContext(std::string pdp_type, std::string apn_name)
{
    return check_for_OK(send_command("AT+CGDCONT=1,\""+ pdp_type +"\",\"" + apn_name + "\",0,0", true, true));
}

bool SARA_N210::registerToOperator(std::string op_name)
{
    return check_for_OK(send_command("AT+COPS=1,2,\"" + op_name + "\"", true, true)); 
}

bool SARA_N210::waitForConnection(int & state)
{
    int max_read_serial_loops = 10;

    std::string reply;

    // Protect the program from endles loop circulating
    while(max_read_serial_loops)
    {
        // Decrement the max_read_serial_loops number by one
        max_read_serial_loops--;

        reply = _ser->readString();
        
        // If some reply received at least return false
        if(reply.length() > 0)
        {
            // If the reply contains the +CSCON: 1 URC return true
            if(reply.find("+CSCON: 1") != std::string::npos)
            {
                state = 1;
                return true;
            }

            if(reply.find("+CSCON: 0") != std::string::npos)
            {
                state = 0;
                return true;
            }

            return false;
        }

    }
    return false;
}


int SARA_N210::createUDPSocket(int port_num, int & socket)
{
    std::string reply;
    reply = send_command("AT+NSOCR=\"DGRAM\",17," + std::to_string(port_num) + ",1", true, true);

    if(reply.length() > 0)
    {

        if(reply.find("ERROR") != std::string::npos)
        {
            // The socket is probably opened already
            return false;
        }

        if(check_for_OK(reply) == false)
        {
            // Receiving skipped the OK, so receive it ALSO
            std::string appendix;
            appendix = _ser->readString();
            // Add the lately received response to the older one
            reply = reply + appendix;
        }

        // The response should be complete now
        if(check_for_OK(reply) == true)
        {
            std::string socket_num_string = "";

            // Use rexex to find the IMSI
            std::smatch match;
            std::regex pattern("\\r\\n([0-9]+)\\r\\n\\r\\nOK\\r\\n");

            if (std::regex_match(reply, match, pattern))
            {
                // Select the second submatch
                std::ssub_match sub_match = match[1];
                socket_num_string = sub_match.str();

                //std::cout << "Socket opened: " <<  socket_num_string << std::endl; 
                socket = stoi(socket_num_string);

                return true;
            }
        }
    }
   
    return false;
}

bool SARA_N210::sendPacket(std::string dest_ip_addr, int dest_port, std::string payload)
{
    std::string reply;
    reply = send_command("AT+NSOST=0,\"" + dest_ip_addr + "\"," + std::to_string(dest_port) + ",2,\"" + payload + "\"", true, true);

    // Uncomplete receive might occur, check that out
    if(check_for_OK(reply) == false)
    {   
        // If that happen, receive the rest (appendix) also
        std::string appendix;
        appendix = _ser->readString();
        if(appendix.length() > 0)
        {
            reply = reply + appendix;
        }
    }

    return check_for_OK(reply);
}




//bool SARA_N210::closeUDPSocket(int port_num)
//{
//    return check_for_OK(send_command("AT+USOCL=" + std::to_string(port_num), true, true));
//}

/*
    //send_command("AT+CIMI", true, true);
    //send_command("AT+CSCON=1", true, true);
    //send_command("AT+CFUN=1", true, true);
    //send_command("AT+CGDCONT=1,\"IP\",\"APN_name\",0,0", true, true);
    //send_command("AT+COPS=1,2,\"23003\"", true, true);
    
    // Wait until the ED is registered
    while(1)
    {   
        reply = serial.readString();
        
        if(reply.length() > 0)
        {
            if(reply.find("+CSCON: 1") != std::string::npos)
            {
                std::cout << "[N210] Received: " << reply << std::endl;
                usleep(1000000);
                break;
            } 
        }

        usleep(1000000);
    }

    // Registered and connected
    send_command("AT+NSOCR=\"DGRAM\",17,60002,1", true, true);
    send_command("AT+NSOST=0,\"127.0.0.1\",60000,2,\"AABB\"", true, true);

    while(1)
    {   
        reply = serial.readString();
        
        if(reply.length() > 0)
        {
            if(reply.find("+CSCON: 0") != std::string::npos)
            {
                std::cout << "[N210] Received: " << reply << std::endl;
                break;
            } 
        }

        usleep(1000000);
    }
*/
