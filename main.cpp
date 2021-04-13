#include <iostream>
#include "Settings.h"
#include <unistd.h>
#include "rn2483.h"
#include "sara_n210.h"

int main(int, char**)
{
    serial.begin(9600);
    serial.setTimeout(5000);

    std::string modem_prefix = "[DEBUG    ] ";

    SARA_N210 modem = SARA_N210(serial);


    if(modem.is_modem_modem_ok() == false)
    {
        std::cout << modem_prefix << "Modem check did not pass" << std::endl;
        return 1;
    }
    std::cout << modem_prefix << "Modem is OK!" << std::endl;


    std::string IMSI = modem.get_IMSI();
    if(IMSI.length() == 0)
    {   
        std::cout << std::endl;
        std::cout << modem_prefix << "IMSI could not be obtained." << std::endl;
        return 1;
    }
    std::cout << std::endl << modem_prefix << "IMSI obtained: " << IMSI << std::endl;


    if(modem.setCSCON(true) == false)
    {
        std::cout << modem_prefix << "Couldnt set the CSCON URC ON" << std::endl;
        return 1;
    }
    std::cout << modem_prefix << "CSCON URC ON success" << std::endl;


    if(modem.setCFUN(true) == false)
    {
        std::cout << modem_prefix << "Couldnt set the CFUN ON" << std::endl;
        return 1;
    }
    std::cout << modem_prefix << "CFUN ON success" << std::endl;

    if(modem.setPDPContext("IP", "moje.apn") == 0)
    {
        std::cout << modem_prefix << "Failed to set the default PDP context" << std::endl;
        return 1;
    }
    std::cout << modem_prefix << "PDP context configured successully" << std::endl;

    if(modem.registerToOperator("23003") == false)
    {
        std::cout << modem_prefix << "Could not accept the register to operator command" << std::endl;
        return 1;
    }
    std::cout << modem_prefix << "Operator register command accepted" << std::endl;
    std::cout << modem_prefix << "Waiting till the modem gets connected..." << std::endl;

    int connection;
    if(modem.waitForConnection(connection) == false)
    {
        if(connection != 1)
        {
            std::cout << modem_prefix << "Waiting for the +CSCON:1 unsuccessfull" << std::endl;
            return 1;
        }
    }
    std::cout << modem_prefix << "+CSCON:1 received" << std::endl;
    auto start = std::chrono::system_clock::now();

    int socket = 0;
    if(modem.createUDPSocket(60002, socket) == false)
    {
        std::cout << modem_prefix << "Socket could not be created" << std::endl;
        
        // The socket might be probably already opened
        std::cout << modem_prefix << "To solve the problem please restart the NB-IoT emulator" << std::endl;        
    }
    else
    {
        std::cout << modem_prefix << "Socket created successfully (num: " << socket << ")" << std::endl;    
    }
    
    if(modem.sendPacket("127.0.0.1", 60000, "AABB") == false)
    {
        std::cout << modem_prefix << "The packet could not be accepted" << std::endl;
        return 1;
    }
    std::cout << modem_prefix << "The packet accepted successfully" << std::endl;
    std::cout << modem_prefix << "Waiting till the modem gets disconnected from active connected state..." << std::endl;

    if(modem.waitForConnection(connection) == false)
    {
        if(connection != 0)
        {
            std::cout << modem_prefix << "Waiting for the +CSCON:0 unsuccessfull" << std::endl;
            return 1;
        }
    }
    std::cout << modem_prefix << "+CSCON:0 received" << std::endl;

    auto end = std::chrono::system_clock::now();
    
    auto elapsed_s = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    int seconds = elapsed_s.count();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    int milliseconds = elapsed_ms.count();

    std::cout << modem_prefix << "Active CONNECTION took: " << seconds << "s " << milliseconds - (1000*seconds) << " ms" << std::endl;
    std::cout << modem_prefix << "Program finished" << std::endl; 

    return 0;
}
