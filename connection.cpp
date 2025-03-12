#include "connection.h"
#include <QSqlError>
#include <QDebug>

Connection::Connection() {}

bool Connection::createconnect() {
    bool test = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("ProBracket");
    db.setUserName("Maram");
    db.setPassword("1234");

    if (db.open()) {
        test = true;
        qDebug() << "Connexion réussie à la base de données!";
    } else {
        qDebug() << "Erreur de connexion: " << db.lastError().text();
    }

    return test;
}
