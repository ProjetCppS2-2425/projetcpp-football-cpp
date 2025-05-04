#ifndef ARDUINO_H
#define ARDUINO_H
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
class Arduino
{
public:
    Arduino();
    ~Arduino();
    int connect_arduino();               // Connecter le PC à Arduino
    int close_arduino();                 // Fermer la connexion
    void write_to_arduino(QByteArray data); // Envoyer des données vers Arduino
    QByteArray read_from_arduino();      // Recevoir des données d'Arduino
    QSerialPort* getserial();            // Accesseur pour le port série
    QString getarduino_port_name();      // Récupérer le nom du port
private:
    QSerialPort *serial;                 // Gestion de la communication série
    static const quint16 arduino_uno_vendor_id = 9025;   // Vendor ID Arduino Uno
    static const quint16 arduino_uno_product_id = 67;    // Product ID Arduino Uno
    QString arduino_port_name;
    bool arduino_is_available;
    QByteArray data;
};

#endif
