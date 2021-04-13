#include "rn2483.h" 
#include <regex>

const std::string DEFAULT_REPLAY     = "ok";
const std::string SYSTEM_GET_COMMAND = "sys get ";
const std::string SYSTEM_SET_COMMAND = "sys set ";
const std::string MAC_COMMAND        = "mac ";
const std::string MAC_GET_COMMAND    = "mac get ";
const std::string MAC_SET_COMMAND    = "mac set ";


RN2483::RN2483(Serial & ser) 
{ 
    _ser = &ser; 
} 

std::string RN2483::modem_get_command(const std::string layer, std::string command, std::string value)
{
    std::string whole_command = layer + command + " " + value;
     std::cout << whole_command << std::endl;
    _ser->println(whole_command.c_str());

    std::string reply = _ser->readString();
    std::cout << "[RN2483] Received: " << reply << std::endl; 

    return reply;
}

bool RN2483::modem_set_command(const std::string layer, std::string command, std::string value, std::string expected_reply)
{
    std::string whole_command = layer + command + " " + value;
     std::cout << whole_command << std::endl;
    _ser->println(whole_command.c_str());

    std::string reply = _ser->readString();
    std::cout << "[RN2483] Received: " << reply << std::endl; 

    return reply.find(expected_reply) != std::string::npos;
}

bool RN2483::is_modem_modem_ok() 
{ 
    std::string retVal;
    retVal = modem_get_command(SYSTEM_GET_COMMAND, "ver", "");

    if(retVal.length() > 0) 
    { 
        return true; 
    }

    return false; 
}


bool RN2483::set_ch_freq (int channel, int freq)
{
    std::string retVal;
    retVal = modem_get_command(MAC_SET_COMMAND, "ch freq", std::to_string(channel) + " " + std::to_string(freq));
    
    if(retVal.find("ok") == std::string::npos) 
    { 
        return true; 
    } 

    retVal.clear();
    return false; 
}

bool RN2483::get_ch_dc(int channel)
{
    std::string retVal;
    retVal = modem_get_command(MAC_GET_COMMAND, "ch dcycle", std::to_string(channel));
    
    if(retVal.find("ok") == std::string::npos) 
    { 
        return true; 
    } 

    retVal.clear();
    return false; 
}

bool RN2483::set_ch_dc(int channel, int value)
{
    std::string retVal;
    retVal = modem_get_command(MAC_SET_COMMAND, "ch dcycle", std::to_string(channel) + " " + std::to_string(value));
    
    if(retVal.find("ok") == std::string::npos) 
    { 
        return true; 
    } 

    retVal.clear();
    return false; 
}

std::string RN2483::get_hwEUI()
{
    std::string retVal;
    retVal = modem_get_command(SYSTEM_GET_COMMAND, "hweui", "");
    
    if(retVal.find("ok") == std::string::npos) 
    { 
        return retVal; 
    } 

    retVal.clear();
    return retVal; 
}

bool RN2483::set_NwkSKey(std::string & nwkskey)
{
    if(modem_set_command(MAC_SET_COMMAND, "nwkskey", nwkskey, DEFAULT_REPLAY))
    {
        return true;
    }

    return false;
}

bool RN2483::set_AppSKey(std::string & appskey)
{
    if(modem_set_command(MAC_SET_COMMAND, "appskey", appskey, DEFAULT_REPLAY))
    {
        return true;
    }

    return false;
}

bool RN2483::set_DevAddr(std::string & devaddr)
{
    if(modem_set_command(MAC_SET_COMMAND, "devaddr", devaddr, DEFAULT_REPLAY))
    {
        return true;
    }

    return false;
}

bool RN2483::join_ABP()
{
    if(modem_set_command(MAC_COMMAND, "join", "abp", "accepted"))
    {
        return true;
    }

    return false;
}

bool RN2483::set_DataRate(int dr)
{
    if(modem_set_command(MAC_SET_COMMAND, "dr", std::to_string(dr), DEFAULT_REPLAY))
    {
        return true;
    }

    return false;
}

bool RN2483::mac_save()
{
    if(modem_set_command(MAC_COMMAND, "save", " ", "ok"))
    {
        return true;
    }

    return false;
}

bool RN2483::modem_send_msg(std::string & message, int port, bool confirmed, std::string & received_data)
{   
    std::string send_method = "uncnf";

    if(confirmed)
    {
        send_method = "cnf";
    }

    std::string whole_command = "mac tx " + send_method + " " + std::to_string(port) + " " + message;
    std::cout << whole_command << std::endl;
    _ser->println(whole_command.c_str());
    
    std::string reply = _ser->readString();
    std::cout << "[RN2483] Received: " << reply << std::endl; 

    // If the received length is just for r\n\OK\r\n, receive AGAIN
    if((reply.find(DEFAULT_REPLAY) != std::string::npos) && reply.length() < 5)
    {
        reply = _ser->readString();
        std::cout << "[RN2483] Received: " << reply;

        if(reply.find("mac_tx_ok") != std::string::npos)
        {
            return true;
        }

        int position = reply.find("mac_rx");
        if(position != std::string::npos)
        {   
            std::smatch match;
            std::string downlink;

            //std::string input = "mac_rx 10 44617461204F4B";
            std::cout << "[RN2483] Downlink received, parsing..." << std::endl << std::endl;;
            std::regex pattern("mac_rx\\s\([0-9]+)\\s\([0-F]+)\\s+"); //(?:.*?)
            
            if (std::regex_match(reply, match, pattern))
            {
                // Select the second submatch
                std::ssub_match sub_match = match[2];
                downlink = sub_match.str();
                //std::cout << " Downlink: " << downlink << std::endl;
            }

            received_data = downlink;
            return true;
        }

        if(reply.find("no_free_ch") != std::string::npos)
        {
            return false;
        }
    }
    
    return false;
}

bool RN2483::send_unconfirmed(std::string & msg)
{   
    std::string received_data_dump;
    return modem_send_msg(msg, 2, false, received_data_dump);
}

bool RN2483::send_confirmed(std::string & msg, std::string & received_data)
{

    return modem_send_msg(msg, 2, true, received_data);
}