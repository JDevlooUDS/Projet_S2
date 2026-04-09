#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define NOGDI

#include "inputs.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include "../include/json.hpp"
#include "QDebug"
#include <QString>
#include <QObject>

using json = nlohmann::json;

class Jon : public QObject
{
    Q_OBJECT
private:
    QSerialPort* arduino;
    bool connected = false;

    int   lastJoyX = 500, lastJoyY = 0;
    bool  lastB1 = false, lastB2 = false, lastB3 = false, lastB4 = false;
    float lastX = 0.0, lastY = 0.0, lastZ = 0.0, lastV = 0.0;

public:
    Jon();
    ~Jon();

    bool SendBar(int bar);                    // continu, max 20/sec
    bool SendTime(float gameTime);            // ponctuel
    bool SendToSerial(bool R1, bool R2, bool R3,
        bool V1, bool V2, bool V3); // ponctuel

    bool RcvFromSerial(Inputs* inputs);
    bool isConnected();
    bool openPort();
    static Jon& getInstance();
    void clean();
public slots:
    void disconnect();
};