#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("projet");//inserer le nom de la source de données
db.setUserName("projet");//inserer nom de l'utilisateur
db.setPassword("nouh1234");//inserer mot de passe de cet utilisateur

if (db.open())
test=true;
qDebug() << "Connexion avec la base de données réussie";
QSqlQuery query;
query.exec("CREATE TABLE IF NOT EXISTS matches ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
           "equipe1 TEXT, "
           "equipe2 TEXT, "
           "date DATE, "
           "heure TIME, "
           "stade TEXT, "
           "score TEXT)");

    return  test;
}
