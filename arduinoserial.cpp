#include "arduinoserial.h"
#include <QDebug>

ArduinoSerial::ArduinoSerial() {
    serial.setPortName("COM3");  // Change to the correct port name
    serial.setBaudRate(QSerialPort::Baud9600);
    if (!serial.open(QIODevice::ReadWrite)) {
        qDebug() << "Failed to open serial port:" << serial.errorString();
    } else {
        qDebug() << "Serial port opened successfully on" << serial.portName();
    }
}

bool ArduinoSerial::sendCommand(const QString& command) {
    if (!serial.isOpen()) {
        qDebug() << "Serial port is not open.";
        return false;
    }
    serial.write(command.toUtf8() + "\n");
    serial.flush();
    qDebug() << "Command sent: " << command;
    return true;
}

QString ArduinoSerial::receiveResponse() {
    if (serial.isOpen()) {
        if (serial.waitForReadyRead(100)) {  // Wait for response
            QByteArray response = serial.readAll();
            return QString(response);
        }
    }
    return QString();
}

ArduinoSerial::~ArduinoSerial() {
    if (serial.isOpen()) {
        serial.close();
        qDebug() << "Serial port closed.";
    }
}
