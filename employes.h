#ifndef EMPLOYES_H
#define EMPLOYES_H

#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QRandomGenerator>

class Employes {
public:
    Employes(QString nom, QString prenom, QString dateEmbauche, QString salaire,
             QString poste, QString email, QString sexe, QString password,
             int idEntraineur, int idM); // Updated constructor with idEntraineur and idM

    bool addToDatabase();
    bool updateToDatabase(int id);
    static bool deleteById(int id);    // Function to add data to the database

    static bool deleteFromDatabase(const QString &nom, const QString &prenom);
    static QSqlQuery getAllEmployes();
    static QString generateUniquePassword(); // Function to generate a unique password

    // Getters
    QString getIdEmp() const;
    QString getNom() const;
    QString getPrenom() const;
    QString getDateEmbauche() const;
    QString getSalaire() const;
    QString getPoste() const;
    QString getEmail() const;
    QString getSexe() const;
    QString getPassword() const;

    // Getters for new attributes
    int getIdEntraineur() const;
    int getIdM() const;

private:
    QString idEmp;
    QString nom;
    QString prenom;
    QString dateEmbauche;
    QString salaire;
    QString poste;
    QString email;
    QString sexe;
    QString password;
    int idEntraineur; // New attribute for idEntraineur
    int idM; // New attribute for idM
};

#endif // EMPLOYES_H
