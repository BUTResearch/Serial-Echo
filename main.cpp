#include <iostream>
#include "Settings.h"
#include <unistd.h>
#include "rn2483.h"
#include "sara_n210.h"

std::string prefix = "[DEBUG    ] ";

void debug(std::string text)
{
    std::cout << prefix << text << std::endl;
}

int main(int, char**)
{
    serial.begin(9600);
    serial.setTimeout(5000);

    int curr_socket = 0;
    SARA_N210::downlink_t downlink;

    SARA_N210 modem = SARA_N210(serial);

    if(modem.isModemOk() == false)
    {
        debug("Modem check did not pass");
        return 1;
    }
    debug("Modem is OK!");


    std::string IMSI = modem.getIMSI();
    if(IMSI.length() == 0)
    {   
        std::cout << std::endl;
        debug("IMSI could not be obtained.");
        return 1;
    }
    debug("IMSI obtained: " + IMSI);


    if(modem.setCSCON(true) == false)
    {
        debug("Couldnt set the CSCON URC ON");
        return 1;
    }
    debug("CSCON URC ON success");


    if(modem.setCFUN(true) == false)
    {
        debug("Couldnt set the CFUN ON");
        return 1;
    }
    debug("CFUN ON success");


    if(modem.setPDPContext("IP", "moje.apn") == 0)
    {
        debug("Failed to set the default PDP context");
        return 1;
    }
    debug("PDP context configured successully");


    if(modem.registerToOperator("23003") == false)
    {
        debug("Could not accept the register to operator command");
        return 1;
    }
    debug("Operator register command accepted");
    debug("Waiting till the modem gets connected...");


    // Wait until the modem gets connected
    while(modem.isCSCONTrue() == false)
    {   
        modem.waitForURC();
    }
    debug("Modem is now in RRC CONNECTED state");


    auto start = std::chrono::system_clock::now();

    if(modem.createUDPSocket(60002, curr_socket) == false)
    {
        debug("Socket could not be created");
        
        // The socket might be probably already opened
        debug("To solve the problem please restart the NB-IoT emulator");       
    }
    else
    {
        debug("Socket created successfully (num: " + std::to_string(curr_socket) + ")");  
    }


    if(modem.sendPacket("127.0.0.1", 60000, "AABB") == false)
    {
        debug("The packet could not be accepted"); 
        return 1;
    }
    debug("The packet accepted successfully");
    debug("Waiting till the modem gets disconnected from active connected state..."); 
    

    bool CSCON_connected = modem.isCSCONTrue();

    // Endless loop
    while(1)
    {
        // Wait for some modem response
        modem.waitForURC();

        if(modem.isDownlinkInQueque() == true)
        {
            if(modem.readDownlink(curr_socket, downlink) == false)
            {
                debug("Downlink readout failed");
            }
            else
            {
                debug("Payload: " + downlink.payload);
                debug("socket : " + std::to_string(downlink.socket));
                debug("ip_add : " + downlink.ip_addr);
                debug("port   : " + std::to_string(downlink.port));
                debug("pay_len: " + std::to_string(downlink.payload_length));
            }
        }

        if(CSCON_connected != modem.isCSCONTrue())
        {   
            CSCON_connected = modem.isCSCONTrue();

            if(CSCON_connected == false)
            {
                debug("Modem is now in IDLE state");
            }

            else
            {
                debug("Modem is now in Connected state");
            }   

            /*
            auto end = std::chrono::system_clock::now();
            
            auto elapsed_s = std::chrono::duration_cast<std::chrono::seconds>(end - start);
            int seconds = elapsed_s.count();
            auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            int milliseconds = elapsed_ms.count();

            debug("Active CONNECTION took: " + std::to_string(seconds) + "s " + std::to_string(milliseconds - (1000*seconds)) + " ms");
            */
        }
    }
   
    debug("Program finished");

    return 0;
}
