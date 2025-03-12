#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("ProBracket");//inserer le nom de la source de données
db.setUserName("Amine");//inserer nom de l'utilisateur
db.setPassword("foot");//inserer mot de passe de cet utilisateur

if (db.open())
test=true;





    return  test;
}
void Connection::closeConnection(){ db.close();}
