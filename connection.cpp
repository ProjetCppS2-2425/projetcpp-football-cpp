#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("ProBracket");//
db.setUserName("maram");//
db.setPassword("1234");//

if (db.open())
test=true;





    return  test;
}
