#include "Equipes.h"
#include <QSqlQuery>
#include <QSqlQueryModel>

// Constructeur
Equipes::Equipes() {}

Equipes::Equipes(int id, const QString &nom, const QString &nationalité, const QString &email, double nombre, const QString &rank)
    : Id_Eq(id), Nom_Equipe(nom), Nationalité(nationalité), Email(email), Nombre_De_Joueur(nombre), Rank(rank) {}

// Ajouter un partenaire
bool Equipes::ajouter()
{
    QSqlQuery query;
    QString eq = QString::number(Id_Eq);
    query.prepare("INSERT INTO Equipes (ID_EQ, NOM, NATIONALITE, EMAIL, NOMBRE_DE_JOUEURS,RANK) "
                  "VALUES (:id, :nom, :nationalité, :email, :nombre, :rank)");
    query.bindValue(":id", Id_Eq);
    query.bindValue(":nom",Nom_Equipe );
    query.bindValue(":nationalité",Nationalité);
    query.bindValue(":email",Email);
    query.bindValue(":nombre",Nombre_De_Joueur);
    query.bindValue(":rank", Rank);

    return true;
}

// Modifier un partenaire
bool Equipes::modifier(int id)
{
    QSqlQuery query;
    query.prepare("UPDATE EQUIPES SET NOM_EQUIPES= :nom, NATIONALITE = :nationalité, EMAIL = :email, "
                  "NOMBRE_DE_JOUEURS = :nombre, RANK= :rank WHERE ID_EQ = :id");
    query.bindValue(":id", id);
    query.bindValue(":nom", Nom_Equipe);
    query.bindValue(":nationalité", Nationalité);
    query.bindValue(":email", Email);
    query.bindValue(":nombre", Nombre_De_Joueur);
    query.bindValue(":rank", Rank);
    return query.exec();
}

// Supprimer un partenaire
bool Equipes::supprimer(int Id_Eq)
{
    QSqlQuery query;
    query.prepare("DELETE FROM RESULTAT WHERE Id_Eq = :Id_Eq");
    query.bindValue(":Id_Eq", Id_Eq); // Modifié: utiliser directement l'entier
    return query.exec();
}

// Afficher les partenaires
QSqlQueryModel* Equipes::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM EQUIPES");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Id_Eq"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom_Equipe"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Nationalité"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Nombre_De_Joueur"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Rank"));
    return model;
}
