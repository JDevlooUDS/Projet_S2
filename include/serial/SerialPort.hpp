#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define NOGDI
#include <windows.h>
#include <iostream>

#define ARDUINO_WAIT_TIME 2000
#define MAX_DATA_LENGTH 255

class SerialPort
{
private:
    HANDLE handler;
    bool connected;
    COMSTAT status;
    DWORD errors;

public:
    explicit SerialPort(const char *portName, int BAUD);
    ~SerialPort();

    int readSerialPort(unsigned char* buffer, unsigned int buf_size);
    bool writeSerialPort(const unsigned char* buffer, unsigned int buf_size);
    bool isConnected();
    void closeSerial();
};