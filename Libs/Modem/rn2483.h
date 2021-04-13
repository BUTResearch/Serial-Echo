#pragma once 
#include <iostream> 
#include <string> 
#include "string.h" 
#include "Serial.h" 

class RN2483
{
private : 
    Serial *_ser; 
    bool        modem_set_command(const std::string layer, std::string command, std::string value, std::string expected_reply);
    std::string modem_get_command(std::string layer, std::string command, std::string value);
    bool        modem_send_msg(std::string & message, int port, bool confirmed, std::string & received_data);
public : 
    RN2483(Serial & ser); 
    bool        is_modem_modem_ok();
    std::string get_hwEUI(); 
    bool        set_NwkSKey(std::string & nwkskey);
    bool        set_AppSKey(std::string & appskey);
    bool        set_DevAddr(std::string & devaddr);
    bool        join_ABP();
    bool        set_DataRate(int dr);
    bool        mac_save();
    bool        send_unconfirmed(std::string & msg);
    bool        send_confirmed(std::string & msg, std::string & received_data);

    bool        set_ch_dc(int channel, int value);
    bool        get_ch_dc(int channel);
    bool        set_ch_freq (int channel, int freq);
};