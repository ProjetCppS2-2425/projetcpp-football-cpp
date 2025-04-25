#ifndef ARDUINOSERIAL_H
#define ARDUINOSERIAL_H

#include <QSerialPort>
#include <QString>

class ArduinoSerial {
public:
    ArduinoSerial();
    ~ArduinoSerial();
    bool sendCommand(const QString& command);
    QString receiveResponse();  // New method to read response

private:
    QSerialPort serial;
};

#endif // ARDUINOSERIAL_H
