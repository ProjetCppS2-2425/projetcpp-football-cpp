#include "Arduino.h"
#include <QDebug>
Arduino::Arduino() {}
Arduino::~Arduino() {
    if(serial) {
        if(serial->isOpen())
            serial->close();
        delete serial;
    }
}
QString Arduino::getarduino_port_name() {
    return arduino_port_name;
}

QSerialPort* Arduino::getserial() {
    return serial;
}

int Arduino::connect_arduino() {
    // Recherche du port Arduino via les identifiants Vendor/Product
    foreach (const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts()) {
        if (serial_port_info.hasVendorIdentifier() && serial_port_info.hasProductIdentifier()) {
            if (serial_port_info.vendorIdentifier() == arduino_uno_vendor_id &&
                serial_port_info.productIdentifier() == arduino_uno_product_id) { // Correction de la faute de frappe
                arduino_is_available = true;
                arduino_port_name = serial_port_info.portName();
                break; // Sortir de la boucle une fois le port trouvé
            }
        }
    }

    qDebug() << "Port Arduino détecté :" << arduino_port_name; // Débogage en dehors de la boucle

    if (arduino_is_available) {
        serial = new QSerialPort(); // Initialisation de l'objet série
        serial->setPortName(arduino_port_name);
        if (serial->open(QSerialPort::ReadWrite)) {
            // Configuration de la communication série
            serial->setBaudRate(QSerialPort::Baud9600);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            return 0; // Connexion réussie
        }
        return 1; // Échec de l'ouverture du port
    }
    return 2; // Aucun Arduino détecté
}
int Arduino::close_arduino() {
    if (serial->isOpen()) {
        serial->close();
        return 0; // Fermeture réussie
    }
    return 1; // Port déjà fermé
}
QByteArray Arduino::read_from_arduino() {
    if (serial->isReadable()) {
        data = serial->readAll();
        return data;
    }
    //return QByteArray(); // Retourne un QByteArray vide si non lisible
}
void Arduino::write_to_arduino(QByteArray d) {
    if (serial->isWritable()) { // Correction de "isWriteble" → "isWritable"
        serial->write(d);
    } else {
        qDebug() << "Échec d'écriture sur le port série !";
    }
}
