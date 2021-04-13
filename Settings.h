#pragma once

#include "Serial.h"
#include "EchoSerial.h"

#define SERIAL_1 "/dev/tnt0"
#define SERIAL_ECHO "/dev/tnt1"
#define BUFFER_SIZE 1000

//Defines if the port returns back the received message
//#define PORT_ECHO

Serial serial = Serial(SERIAL_1, BUFFER_SIZE);

#ifdef PORT_ECHO
EchoSerial *echo_serial = new EchoSerial(SERIAL_ECHO, BUFFER_SIZE);
#endif