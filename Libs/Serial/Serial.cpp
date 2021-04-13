
#include "Serial.h"

Serial::Serial(std::string port, uint32_t buff_size){
    _port = std::move(port);
    _buff_len = buff_size;
    _buffer = new char[_buff_len];
}

void Serial::begin(int speed){
    _speed = speed;
    _port_handle = open(_port.c_str(), O_RDWR);

    if (tcgetattr(_port_handle, &_tty) != 0) {
        perror("Serial get attributes error");
        return;
    }

    _tty.c_cflag &= ~PARENB;
    _tty.c_cflag &= ~CSTOPB;
    _tty.c_cflag |= CS8;
    _tty.c_cflag &= ~CRTSCTS;
    _tty.c_cflag |= CREAD | CLOCAL;

    cfmakeraw(&_tty);

    _tty.c_cc[VTIME] = _timeout / 100;
    _tty.c_cc[VMIN] = 0;

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

    cfsetispeed(&_tty, port_rate);
    cfsetospeed(&_tty, port_rate);

    if (tcsetattr(_port_handle, TCSANOW, &_tty) != 0){
        perror("Serial set attribute error");
        return;
    }
}

void Serial::closeSerial(){
    if (_port_handle > 0){
        close(_port_handle);
    }
}

void Serial::checkSerial(){
    if (_port_handle < 0){
        std::cerr << "Port is closed" << std:: endl;
        exit(EXIT_FAILURE);
    }
}

size_t Serial::print(std::string data){
    checkSerial();
    size_t len = data.length();
    size_t ret_len = 0;
    strcpy(_buffer, data.c_str());
    ret_len = write(_port_handle, _buffer, len);
    return ret_len;
}

size_t Serial::println(std::string data){
    checkSerial();
    data = data + "\r\n";
    size_t len = data.length();
    size_t ret_len = 0;
    strcpy(_buffer, data.c_str());
    ret_len = write(_port_handle, _buffer, len);
    return ret_len;
}

int Serial::available(){
    int nread;
    ioctl(_port_handle, FIONREAD, &nread);
    return nread;
}

int Serial::readChar(){
    checkSerial();
    char ret_char;

    ssize_t len = read(_port_handle, &ret_char, 1);
    if (len == -1){
        std::cerr << "Can not read from serial" << std::endl;
    } else if (len == 0) {
        std::cerr << "No data received" << std::endl;
    } else {
        return ret_char;
    }
    return 0;
}

void Serial::setTimeout(uint32_t timeout){
    checkSerial();
    _tty.c_cc[VTIME] = timeout / 100;

    if (tcsetattr(_port_handle, TCSANOW, &_tty) != 0){
        perror("Serial set attribute error");
        return;
    }
}

std::string Serial::readString() {
    checkSerial();
    ssize_t len = read(_port_handle, _buffer, _buff_len);
    if (len == -1){
        std::cerr << "Can not read from serial" << std::endl;
    } else if (len == 0) {
        //std::cerr << "No data received" << std::endl;
    } else {
        _buffer[len] = '\0';
        std::string ret_string(_buffer);
        return ret_string;
    }
    return "";
}

std::string Serial::readStringUntil(char terminator){
    checkSerial();
    int len = 0;
    char ret_char = 0;

    while (ret_char != terminator)
    {
        ret_char = readChar();
        if (ret_char == 0) break;
        _buffer[len] = ret_char;
        len++;
    }
    if (ret_char == 0){
        std::cerr << "Terminator char was not found" << std::endl;
    }
    _buffer[len] = 0;
    std::string ret_string(_buffer);
    return ret_string;
}

Serial::~Serial(){
    delete _buffer;
    if (_port_handle > 0){
        close(_port_handle);
    }
}