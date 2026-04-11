#include "Jon.h"
#include <iostream>
#include <chrono>

Jon::Jon()
{
    arduino = new QSerialPort();
    arduino->setPortName("COM3");
    arduino->setBaudRate(QSerialPort::Baud115200);
    arduino->setDataBits(QSerialPort::Data8);
    arduino->setParity(QSerialPort::NoParity);
    arduino->setStopBits(QSerialPort::OneStop);
    arduino->setFlowControl(QSerialPort::NoFlowControl);
    openPort();
    connect(arduino, &QSerialPort::errorOccurred, this, &Jon::disconnect);   
}

Jon::~Jon()
{
    if (arduino != nullptr) {
        arduino->close();
        delete arduino;
        arduino = nullptr;
    }
}

Jon& Jon::getInstance() {
    static Jon instance;
    return instance;
}

void Jon::clean() {
    if (arduino != nullptr) {
        arduino->close();
        delete arduino;
        arduino = nullptr;
    }
}

bool Jon::openPort() {
    if (arduino->open(QIODevice::ReadWrite)) {
        serialBuffer.clear();
        arduino->clear();
        connected = true;
        qDebug() << "Arduino connected!";
    }
    else {
        connected = false;
        qDebug() << "Arduino NOT connected:" << arduino->errorString();
    }
    return connected;
}

// Envoi continu : bar seulement, max 20 fois/sec
bool Jon::SendBar(int bar)
{
    static auto lastSend = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>
        (now - lastSend).count();

    if (elapsed < 50) return true;
    lastSend = now;

    json j;
    j["bar"] = bar;

    std::string msg = j.dump() + "\n";
    return arduino->write(msg.c_str());
}

// Envoi ponctuel : time seulement
bool Jon::SendTime(float gameTime)
{
    json j;
    j["time"] = gameTime;

    std::string msg = j.dump() + "\n";
    return arduino->write(msg.c_str());
}

// Envoi ponctuel : LEDs seulement
bool Jon::SendToSerial(bool R1, bool R2, bool R3,
    bool V1, bool V2, bool V3)
{
    json j;
    j["R1"] = R1; j["R2"] = R2; j["R3"] = R3;
    j["V1"] = V1; j["V2"] = V2; j["V3"] = V3;

    std::string msg = j.dump() + "\n";

    // Envoyer 3 fois pour garantir la réception
    bool ok = false;
    for (int i = 0; i < 10; i++) {
        ok = arduino->write(msg.c_str());
    }
    return ok;
}

bool Jon::RcvFromSerial(Inputs* inputs)
{
    serialBuffer += arduino->readAll();
    size_t pos;
    while ((pos = serialBuffer.indexOf('\n')) != -1)
    {
        QString line = serialBuffer.left(pos);
        serialBuffer.remove(0, pos + 1);

        try
        {
            json j = json::parse(line.toStdString());

            lastJoyX = j.value("joyX", lastJoyX);
            lastJoyY = j.value("joyY", lastJoyY);
            lastB1 = j.value("b1", lastB1);
            lastB2 = j.value("b2", lastB2);
            lastB3 = j.value("b3", lastB3);
            lastB4 = j.value("b4", lastB4);
            lastX = j.value("X", lastX);
            lastY = j.value("Y", lastY);
            lastZ = j.value("Z", lastZ);
            lastV = j.value("v", lastV);

            if (lastJoyX > 800) inputs->isLeftPressed = true;
            else inputs->isLeftPressed = false;
            if (lastJoyX < 200) inputs->isRightPressed = true;
            else inputs->isRightPressed = false;
            if (lastJoyY > 800) inputs->isUpPressed = true;
            else inputs->isUpPressed = false;
            if (lastJoyY < 200) inputs->isDownPressed = true;
            else inputs->isDownPressed = false;
            if (lastB1) inputs->isSpacePressed = true;
            else inputs->isSpacePressed = false;
            if (lastB2) inputs->isDashPressed = true;
            else inputs->isDashPressed = false;
            if (abs(lastX) > 50 || abs(lastY) > 50 || abs(lastZ) > 50) inputs->isAccelerated = true;
            else inputs->isAccelerated = false;
            if (lastB3) inputs->isSelectPressed = true;
            else inputs->isSelectPressed = false;
            if (lastB4) inputs->isPausePressed = true;
            else inputs->isPausePressed = false;
            inputs->volume = lastV/100; // à tester les valeurs
            int nombre = rand() % 60;
            if (nombre == 59) inputs->muon = true;
            else inputs->muon = false;

            return true;
        }
        catch (const std::exception&) {
            return false;
        }
    }
}

bool Jon::isConnected() {
    return connected;
}

void Jon::disconnect(QSerialPort::SerialPortError error) {
    if (error == QSerialPort::ResourceError || error == QSerialPort::DeviceNotFoundError) {
        qDebug() << "Controller disconnected\n";
        connected = false;
        arduino->close();
        arduino->deleteLater();
        arduino = nullptr;
    }
}