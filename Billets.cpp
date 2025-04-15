#include "billets.h"

#include <utility>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QDebug>
#include <QDate>
class billetsData : public QSharedData
{
public:
};

billets::billets() {} // Default constructor

billets::~billets() {} // Destructor

// Add a billet to the database
bool billets::addBillet(int id, int id_match, int nombre, const QDate &date, QString siege, double prix, const QString &type)
{

    QSqlQuery query;
    query.prepare("INSERT INTO BILLETS (id, id_match,nombre, date_billet, siege, prix, type) "
                  "VALUES (:id, :id_match, :nombre, :date_billet, :siege, :prix, :type)");
    query.bindValue(":id", id);
    query.bindValue(":id_match", id_match);
    query.bindValue(":nombre", nombre);
    query.bindValue(":date_billet", date);
    query.bindValue(":siege", siege);
    query.bindValue(":prix", prix);
    query.bindValue(":type", type);

    if (!query.exec()) {
        qDebug() << "Add Billet Error:" << query.lastError().text();
        return false;
    }
    return true;
}

// Update a billet in the database
bool billets::modifyBillet(int id, int id_match, int nombre, const QDate &date, QString siege, double prix, const QString &type)
{
    QSqlQuery query;
    query.prepare("UPDATE BILLETS SET id_match = :id_match, nombre = :nombre, date_billet = :date_billet, "
                  "siege = :siege,prix= :prix, type = :type WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":id_match", id_match);
    query.bindValue(":nombre", nombre);
    query.bindValue(":date_billet", date);
    query.bindValue(":siege", siege);
    query.bindValue(":prix", prix);
    query.bindValue(":type", type);

    if (!query.exec()) {
        qDebug() << "Modify Billet Error:" << query.lastError().text();
        return false;
    }
    return true;
}

// Delete a billet from the database
bool billets::deleteBillet(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM BILLETS WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Delete Billet Error:" << query.lastError().text();
        return false;
    }
    return true;
}

// Display all billets
QSqlQueryModel* billets::displayBillets()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM billets");
    return model;
}

// Get all billets
QSqlQueryModel* billets::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM billets");
    if (!query.exec()) {
        qDebug() << "Get All Billets Error:" << query.lastError().text();
    }
    model->setQuery(query);
    return model;
}

int billets::countType(const QString& TYPE)
{
    int count = 0;

    QSqlQuery query;
    query.prepare("SELECT COUNT(TYPE) FROM BILLETS WHERE TYPE = :TYPE");
    query.bindValue(":TYPE", TYPE);

    if (query.exec() && query.next()) {
        count = query.value(0).toInt();
    }

    return count;
}


QSqlQueryModel* billets::tri(QString column,QString choix)
{
QSqlQueryModel* model = new QSqlQueryModel();
model->setQuery("SELECT * FROM BILLETS ORDER BY "+column +" "+choix);


return model;
}

QSqlQueryModel* billets::chercher(QString column,QString text)
{
QSqlQueryModel* model = new QSqlQueryModel();
model->setQuery("SELECT * FROM BILLETS WHERE "+column+" LIKE '%" + text + "%' ");


return model;
}

bool billets::idExists(int id)
{
QSqlQuery query;
query.prepare("SELECT COUNT(*) FROM BILLETS WHERE ID = :id");
query.bindValue(":id", id);

if (query.exec() && query.next()) {
    int count = query.value(0).toInt();
    return count > 0;
}

return false;
}
