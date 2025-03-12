#include "match.h"
#include <QDebug>

Match::Match()
{
}

Match::Match(int id_m, const QString &equipe1, const QString &equipe2, const QDate &date,
             const QString &lieu, const QString &type, const QString &etat)
    : id_m(id_m), equipe1(equipe1), equipe2(equipe2), date(date), lieu(lieu), type(type), etat(etat)
{
}
bool Match::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO match (id_m, equipe1, equipe2, date_M, lieu, type_M, etat) "
                  "VALUES (:id_m, :equipe1, :equipe2, :date_M, :lieu, :type_M, :etat)");
    query.bindValue(":id_m", id_m);
    query.bindValue(":equipe1", equipe1);
    query.bindValue(":equipe2", equipe2);
    query.bindValue(":date_M", date);
    query.bindValue(":lieu", lieu);
    query.bindValue(":type_M", type);
    query.bindValue(":etat", etat);

    return query.exec();

}
int Match ::nextid() {
    QSqlQuery query;
    query.prepare("SELECT MAX(id_m) FROM match");

    if (query.exec() && query.next()) {
        int idmax = query.value(0).toInt();
        return idmax + 1;
    }
    return 1;
}


QSqlQueryModel* Match::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM match");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_M"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Equipe1"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Equipe2"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date_M"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Lieu"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Type_M"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("eq1_but"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("eq2_but"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Etat"));



    return model;
}

bool Match::supprimer(int id_m)
{
    QSqlQuery query;
    query.prepare("DELETE FROM match WHERE id_m = :id_m");
    query.bindValue(":id_m", id_m);
    return query.exec();
}

bool Match::modifier(int id_m,QString equipe1, QString equipe2, QDate date,
                      QString lieu, QString type, QString etat)
{
    QSqlQuery query;
    query.prepare("UPDATE match SET equipe1 = :equipe1, equipe2 = :equipe2, date_m = :date, "
                  "lieu = :lieu, type_m = :type, etat = :etat WHERE id_m = :id_m");
    query.bindValue(":id_m", id_m);
    query.bindValue(":equipe1", equipe1);
    query.bindValue(":equipe2", equipe2);
    query.bindValue(":date", date);
    query.bindValue(":lieu", lieu);
    query.bindValue(":type", type);
    query.bindValue(":etat", etat);
    return query.exec();
}
