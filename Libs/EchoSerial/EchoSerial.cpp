
#include "EchoSerial.h"

EchoSerial::EchoSerial(std::string port, uint32_t buff_size){
    _port = std::move(port);
    _buff_len = buff_size;
    _buffer = new char[_buff_len];
    openSerial();
}

void EchoSerial::receiveData(){
    if (_port_handle < 0) return;
    int len = 0;
    while (_run)
    {
       len = read(_port_handle, _buffer, _buff_len);
       if (len > 0){
           write(_port_handle, _buffer, len);
       }
    }
}

void EchoSerial::startReception(){
    _run = true;
    _rec_thread = std::thread(&EchoSerial::receiveData, this);
}

void EchoSerial::openSerial(){

    struct termios tty{};
    _port_handle = open(_port.c_str(), O_RDWR);

    if (tcgetattr(_port_handle, &tty) != 0) {
        perror("Serial get attributes error");
        return;
    }

    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag |= CREAD | CLOCAL;

    cfmakeraw(&tty);

    tty.c_cc[VTIME] = 5;
    tty.c_cc[VMIN] = 0;

    int port_rate;

    switch (_speed)
    {
    case 9600:
            port_rate = B9600;
        break;
    
    default:
            port_rate = B9600;
        break;
    }

    cfsetispeed(&tty, port_rate);
    cfsetospeed(&tty, port_rate);

    if (tcsetattr(_port_handle, TCSANOW, &tty) != 0){
        perror("Serial set attribute error");
        return;
    }

    startReception();
}

EchoSerial::~EchoSerial(){
    _run = false;
    delete _buffer;
    if (_port_handle > 0){
        _rec_thread.join();
        close(_port_handle);
    }
}