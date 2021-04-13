#pragma once

#include <iostream>
#include <string>
#include "string.h"
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

class Serial
{
private:
    std::string _port = "";
    int _speed{9600};
    uint32_t _timeout{500};
    int _port_handle{-1};
    struct termios _tty{};
    char *_buffer;
    uint32_t _buff_len; 

    void closeSerial();
    void checkSerial();

public:
    /*
    Serial class constructor
    std::string port - serial interface path
    uint32_t buff_size - I/O buffer size
    */
    Serial(std::string port, uint32_t buff_size);

    /*
    Function which opens serial port
    int speed - communication baud rate
    */
    void begin(int speed);

    /*
    Prints data to the serial port as human-readable ASCII text
    std::string data - input data
    */
    size_t print(std::string data);

    /*
    Prints data to the serial port as human-readable ASCII text followed by a carriage return character (ASCII 13, or '\r') and a newline character (ASCII 10, or '\n')
    std::string data - input data
    */
    size_t println(std::string data);

    /*
    Get the number of bytes (characters) available for reading from the serial port.
    */
    int available();

    /*
    Reads a single byte incomming on serial port
    */
    int readChar();

    /*
    Sets the maximum milliseconds to wait for serial data. It defaults to 500 milliseconds.
    uint32_t timeout - timeout duration in milliseconds
    */
    void setTimeout(uint32_t timeout);

    /*
    Reads characters from the serial buffer into a String. The function terminates if it times out
    */
    std::string readString();

    /*
    Reads characters from the serial buffer into a String. The function terminates if it times out or the requested character is found
    char terminator - the character to search for
    */
    std::string readStringUntil(char terminator);
    ~Serial();
};
