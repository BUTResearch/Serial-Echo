#pragma once

#include <iostream>
#include <string>
#include "string.h"
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <thread>
#include <atomic>


class EchoSerial
{

public:
    EchoSerial(std::string port, uint32_t buff_size);
    ~EchoSerial();

private:
    int _port_handle{-1};
    std::string _port = "";
    int _speed{9600};
    char *_buffer;
    uint32_t _buff_len;
    std::atomic<bool> _run = {false};
    std::thread _rec_thread;

    void startReception();
    void openSerial();
    void receiveData();
};
