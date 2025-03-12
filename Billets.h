#ifndef BILLETS_H
#define BILLETS_H

#include <QString>
#include <QSqlQueryModel>
#include <QDate>

class Billets
{
private:
    int CIN;
    int nombreDeBillets;
    QDate dateEmission;
    int siege;
    QString type;
    double prix;
    int idM;


public:
    Billets();
    Billets(int CIN, int nombreDeBillets, QDate dateEmission, int siege, QString type, double prix, int idM);

    // Getters
    int getCIN();
    int getNombreDeBillets();
    QDate getDateEmission();
    int getSiege();
    QString getType();
    double getPrix();
    int getIdM();

    // Setters
    void setCIN(int CIN);
    void setNombreDeBillets(int nombre);
    void setDateEmission(QDate date);
    void setSiege(int siege);
    void setType(QString type);
    void setPrix(double prix);
    void setIdM(int id);

    // Database operations
    bool ajouterBillet();
    bool supprimerBillet(int CIN);
    bool modifierBillet();
    QSqlQueryModel* afficherBillets();
};

#endif // BILLETS_H
