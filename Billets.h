#ifndef BILLETS_H
#define BILLETS_H

#include <QString>
#include <QSqlQueryModel>
#include <QSharedData>

class billets : public QSharedData
{
public:
    billets();
    ~billets();

    bool addBillet(int id, int id_match, int nombre, const QDate &date, QString siege, double prix, const QString &type);

    bool modifyBillet(int id, int id_match, int nombre, const QDate &date, QString siege, double prix, const QString &type);

    bool deleteBillet(int id);

    QSqlQueryModel* displayBillets();

    QSqlQueryModel* afficher();

    int countType(const QString& TYPE);
    QSqlQueryModel* tri(QString column,QString choix);
    QSqlQueryModel* chercher(QString column,QString text);
    bool idExists(int id);



};

#endif // BILLETS_H
