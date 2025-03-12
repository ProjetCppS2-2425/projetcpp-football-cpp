#ifndef EQUIPES_H
#define EQUIPES_H
#include <QSqlQuery>
#include <QString>
#include <QSqlQueryModel>
#include <QDate>

class Equipes
{
private:
    int Id_Eq;
    QString Nom_Equipe;
    QString Nationalité;
    QString Email;
    double Nombre_De_Joueur;
    QString Rank;

public:
    // Constructeurs
    Equipes();
    Equipes(int id, const QString &nom, const QString &nationalité, const QString &email, double nombre, const QString &rank);

    // Getters
    int getId_Eq() const;
    QString getNom_Equipe() const;
    QString getNationalité() const;
    QString getEmail() const;
    double getNombre_De_Joueur() const;
    QString getRank() const;

    // Setters
    void setId_Eq(int id);
    void setNom_Equipe(const QString &nom);
    void setNationalité(const QString &nationalité);
    void setEmail(const QString &email);
    void settNombre_De_Joueur(double nombre);
    void setRank(const QString &rank);

    // Fonctionnalités de base
    bool ajouter();
    bool modifier(int id);
    bool supprimer(int id);
    QSqlQueryModel* afficher();
};


#endif // EQUIPES_H
