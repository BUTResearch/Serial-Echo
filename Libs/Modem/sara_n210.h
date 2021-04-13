#pragma once 
#include <iostream> 
#include <string> 
#include "string.h" 
#include "Serial.h" 

class SARA_N210
{
private: 
    Serial *_ser; 
    std::string send_command(std::string command, bool command_debug_on,  bool reply_debug_on);
    bool check_for_OK(std::string const & text);
   
public : 
    SARA_N210(Serial & ser); 
    bool        is_modem_modem_ok();
    std::string get_IMSI();
    bool        setCFUN(bool on);
    bool        setCSCON(bool on);
    bool        setPDPContext(std::string pdp_type, std::string apn_name);
    bool        registerToOperator(std::string op_name);
    bool        waitForConnection(int & state);
    int         createUDPSocket(int port_num, int & socket);
    bool        closeUDPSocket(int port_num);
    bool        sendPacket(std::string dest_ip_addr, int dest_port, std::string payload);




   
};

//bool        modem_set_command(const std::string layer, std::string command, std::string value, std::string expected_reply);
//std::string modem_get_command(std::string layer, std::string command, std::string value);
//bool        modem_send_msg(std::string & message, int port, bool confirmed, std::string & received_data);
//std::string get_hwEUI(); 
//bool        set_NwkSKey(std::string & nwkskey);
//bool        set_AppSKey(std::string & appskey);
//bool        set_DevAddr(std::string & devaddr);
//bool        join_ABP();
//bool        set_DataRate(int dr);
//bool        mac_save();
//bool        send_unconfirmed(std::string & msg);
//bool        send_confirmed(std::string & msg, std::string & received_data);
//bool        set_ch_dc(int channel, int value);
//bool        get_ch_dc(int channel);
//bool        set_ch_freq (int channel, int freq);