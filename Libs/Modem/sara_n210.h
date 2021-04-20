#pragma once 
#include <iostream> 
#include <string> 
#include "string.h" 
#include "Serial.h" 

class SARA_N210
{
private: 
    Serial *_ser;
    bool URC_downlink;
    bool URC_cscon;

    void        debug(std::string text);
    void        checkForURC(std::string text);
    size_t      sendString(std::string command);
    std::string readString();
    std::string sendCommand(std::string command, bool debug_on);
    std::string sendCommandWithExpectedAnswer(std::string command, std::string expect, bool debug_on);
    bool        checkForOK(std::string const & text);
    bool        receiveExpectAnswer(std::string expect, std::string &reply, bool debug_on);
    std::string toBinary(int number, int min_bits_count);

public : 
    typedef struct downlink_t 
    {
        int         socket;
        std::string ip_addr;
        int         port;
        int         payload_length;
        std::string payload;
    } downlink;

    typedef enum timer_TAU_e
    {
        multiples_10m  = 0,
        multiples_1h   = 1,
        multiples_10h  = 2,
        multiples_2s   = 3,
        multiples_30s  = 4, 
        multiples_1m   = 5,
        multiples_320h = 6
    }timer_TAU;

    typedef enum timer_RAT_e
    {
        multiples_two_seconds = 0,
        multiples_minute      = 1,
        multiples_decihours   = 2,
        multiples_decativated = 7,
    }timer_RAT;

    SARA_N210(Serial & ser); 
    std::string getIMSI();
    bool        isModemOk();
    bool        setCFUN(bool on);
    bool        setCSCON(bool on);
    bool        setPDPContext(std::string pdp_type, std::string apn_name);
    bool        registerToOperator(std::string op_name);
    bool        waitForURC();
    int         createUDPSocket(int port_num, int & socket);
    bool        closeUDPSocket(int port_num);
    bool        sendPacket(std::string dest_ip_addr, int dest_port, std::string payload); 
    bool        isDownlinkInQueque(); 
    bool        readDownlink(int socket, downlink_t & downlink);
    void        resolveURC();
    bool        isCSCONTrue();
};
