#include "../include/serial/SerialPort.hpp"
#include <cstring>
#include <windows.h>

SerialPort::SerialPort(const char *portName, int BAUD)
{
    connected = false;

    handler = CreateFileA(portName,
                          GENERIC_READ | GENERIC_WRITE,
                          0,
                          NULL,
                          OPEN_EXISTING,
                          FILE_ATTRIBUTE_NORMAL,
                          NULL);

    if (handler == INVALID_HANDLE_VALUE)
    {
        std::cerr << "ERROR: port " << portName << " not available\n";
        return;
    }

    DCB dcb = {0};
    if (!GetCommState(handler, &dcb))
    {
        std::cerr << "Failed to get serial parameters\n";
        return;
    }

    dcb.BaudRate = BAUD;
    dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;
    dcb.Parity   = NOPARITY;
    dcb.fDtrControl = DTR_CONTROL_ENABLE;

    if (!SetCommState(handler, &dcb))
    {
        std::cerr << "Could not set serial parameters\n";
        return;
    }

    connected = true;
    PurgeComm(handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
    Sleep(ARDUINO_WAIT_TIME);
}

SerialPort::~SerialPort()
{
    if (connected)
    {
        CloseHandle(handler);
        connected = false;
    }
}

int SerialPort::readSerialPort(unsigned char* buffer, unsigned int buf_size)
{
    DWORD bytesRead = 0;
    unsigned int toRead = 0;
    ClearCommError(handler, &errors, &status);

    if (status.cbInQue > 0)
        toRead = (status.cbInQue > buf_size) ? buf_size : status.cbInQue;

    memset(buffer, 0, buf_size);
    if (ReadFile(handler, buffer, toRead, &bytesRead, NULL))
        return static_cast<int>(bytesRead);

    return 0;
}

bool SerialPort::writeSerialPort(const unsigned char* buffer, unsigned int buf_size)
{
    DWORD bytesSent;
    if (!WriteFile(handler, buffer, buf_size, &bytesSent, NULL))
    {
        ClearCommError(handler, &errors, &status);
        return false;
    }
    return true;
}

bool SerialPort::isConnected()
{
    ClearCommError(handler, &errors, &status);
    return connected;
}

void SerialPort::closeSerial()
{
    if (handler != INVALID_HANDLE_VALUE)
    {
        CloseHandle(handler);
        connected = false;
    }
}