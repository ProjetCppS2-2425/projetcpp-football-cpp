#include "connection.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

Connection::Connection()
{
}

bool Connection::createconnect()
{
    bool test = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("ProBracket");  // ODBC Data Source Name
    db.setUserName("ProBracket");      // Database username
    db.setPassword("foot");            // Database password

    if (db.open()) {
        test = true;
        qDebug() << "Connection successful!";
    } else {
        qDebug() << "Error: Unable to connect to the database";
        qDebug() << "Database Error: " << db.lastError().text();  // Log the error
    }

    return test;
}
