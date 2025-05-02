#include "Equipes.h"
#include <QSqlQuery>
#include <QSqlQueryModel>

// Constructeur
Equipes::Equipes() {}

Equipes::Equipes(int ID_EQ, const QString &NOM_EQUIPE, const QString &NATIONALITE, const QString &EMAIL, double NOMBRE_DE_JOUEUR, const QString &RANK)
    : ID_EQ(ID_EQ), NOM_EQUIPE(NOM_EQUIPE), NATIONALITE(NATIONALITE), EMAIL(EMAIL), NOMBRE_DE_JOUEUR(NOMBRE_DE_JOUEUR), RANK(RANK) {}

// Ajouter un partenaire
bool Equipes::ajouter()
{
    QSqlQuery query;
    QString eq = QString::number(ID_EQ);
    query.prepare("INSERT INTO Equipes (ID_EQ, NOM_EQUIPE, NATIONALITE, EMAIL, NOMBRE_DE_JOUEUR,RANK) "
                  "VALUES (:ID_EQ,:NOM_EQUIPE,:NATIONALITE,:EMAIL,:NOMBRE_DE_JOUEUR,:RANK)");
    query.bindValue(":ID_EQ",ID_EQ);
    query.bindValue(":NOM_EQUIPE",NOM_EQUIPE );
    query.bindValue(":NATIONALITE",NATIONALITE);
    query.bindValue(":EMAIL",EMAIL);
    query.bindValue(":NOMBRE_DE_JOUEUR",NOMBRE_DE_JOUEUR);
    query.bindValue(":RANK",RANK);

    return query.exec();
}

// Modifier un partenaire
bool Equipes::modifier(int ID_EQ)
{
    QSqlQuery query;
    query.prepare("UPDATE EQUIPES SET NOM_EQUIPE= :NOM_EQUIPE , NATIONALITE = :NATIONALITE, EMAIL = :EMAIL, "
                  "NOMBRE_DE_JOUEUR = :NOMBRE_DE_JOUEUR, RANK= :RANK WHERE ID_EQ = :ID_EQ");
    query.bindValue(":ID_EQ", ID_EQ);
    query.bindValue(":NOM_EQUIPE", NOM_EQUIPE);
    query.bindValue(":NATIONALITE", NATIONALITE);
    query.bindValue(":EMAIL", EMAIL);
    query.bindValue(":NOMBRE_DE_JOUEUR", NOMBRE_DE_JOUEUR);
    query.bindValue(":RANK", RANK);
    return query.exec();
}

// Supprimer un partenaire
bool Equipes::supprimer(int ID_EQ)
{
    QSqlQuery query;
    query.prepare("DELETE FROM EQUIPES WHERE ID_EQ = :ID_EQ");
    query.bindValue(":ID_EQ", ID_EQ); // Modifié: utiliser directement l'entier
    return query.exec();
}

// Afficher les partenaires
QSqlQueryModel* Equipes::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM EQUIPES");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_EQ"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM_EQUIPE"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("NATIONALITE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("EMAIL"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("NOMBRE_DE_JOUEUR"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("RANK"));
    return model;
    }

// Rechercher des partenaires
    QSqlQueryModel* Equipes::rechercher(const QString& critere)
    {
        QSqlQueryModel *model = new QSqlQueryModel();
        model->setQuery("SELECT * FROM EQUIPES WHERE "
                        "NOM_EQUIPE LIKE '%" + critere + "%' OR "
                                    "NATIONALITE LIKE '%" + critere + "%' OR "
                                    "EMAIL LIKE '%" + critere + "%' OR "
                                    "RANK LIKE '%" + critere + "%'");

        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_EQ"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM_EQUIPE"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("NATIONALITE"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("EMAIL"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("NOMBRE_DE_JOUEUR"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("RANK"));

        return model;
    }

QSqlQueryModel* Equipes::trier(const QString& critere, Qt::SortOrder order)
{
    QString orderDirection = (order == Qt::AscendingOrder) ? "ASC" : "DESC";

    qDebug() << "][EQUIPES::trier] Sorting EQUIPES by:" << critere
             << "in order:" << orderDirection;

    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM EQUIPES ORDER BY " + critere + " " + orderDirection);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_EQ"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM_EQUIPE"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("NATIONALITE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("EMAIL"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("NOMBRE_DE_JOUEUR"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("RANK"));

    qDebug() << "][EQUIPES::trier] Sorted" << model->rowCount() << "EQUIPES";

    return model;
}
