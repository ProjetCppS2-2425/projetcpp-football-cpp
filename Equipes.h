#ifndef EQUIPES_H
#define EQUIPES_H
#include <QSqlQuery>
#include <QString>
#include <QSqlQueryModel>
#include <QDate>

class Equipes
{
private:
    int ID_EQ;
    QString NOM_EQUIPE;
    QString NATIONALITE;
    QString EMAIL;
    double NOMBRE_DE_JOUEUR;
    QString RANK;

public:
    // Constructeurs
    Equipes();
    Equipes(int Id_EQ, const QString &NOM_EQUIPE, const QString &NATIONALITE, const QString &EMAIL, double NOMBRE_DE_JOUEUR, const QString &RANK);

    // Getters
    int getID_EQ() const;
    QString getNOM_EQUIPE() const;
    QString getNATIONALITE() const;
    QString getEMAIL() const;
    double getNOMBRE_DE_JOUEUR() const;
    QString getRANK() const;

    // Setters
    void setId_EQ(int ID_EQ);
    void setNOM_EQUIPE(const QString &NOM_EQUIPE);
    void setNATIONALITE(const QString &NATIONALITE);
    void setEMAIL(const QString &EMAIL);
    void setNOMBRE_DE_JOUEUR(double NOMBRE_DE_JOUEUR);
    void setRANK(const QString &RANK);

    // Fonctionnalités de base
    bool ajouter();
    bool modifier(int ID_EQ);
    bool supprimer(int ID_EQ);
    QSqlQueryModel* afficher();
    QSqlQueryModel* rechercher(const QString& critere);
    QSqlQueryModel* trier(const QString& critere, Qt::SortOrder order = Qt::AscendingOrder);

};


#endif // EQUIPES_H
