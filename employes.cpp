#include "Employes.h"
#include <QUuid>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QRandomGenerator>
#include <QDate>

Employes::Employes(QString nom, QString prenom, QString dateEmbauche, QString salaire,
                   QString poste, QString email, QString sexe, QString password,
                   int idEntraineur, int idM)
    : nom(nom), prenom(prenom), dateEmbauche(dateEmbauche),
    salaire(salaire), poste(poste), email(email), sexe(sexe),
    password(password), idEntraineur(idEntraineur), idM(idM) {}

bool Employes::addToDatabase() {
    qDebug() << "------ addToDatabase() called ------";

    QSqlQuery query;
    QString idEmp = QUuid::createUuid().toString(QUuid::WithoutBraces);
    qDebug() << "ID généré :" << idEmp;

    QDate date = QDate::fromString(dateEmbauche, "yyyy-MM-dd");
    if (!date.isValid()) {
        qDebug() << "Invalid date format!";
        return false;
    }

    query.prepare("INSERT INTO EMPLOYES (ID_EMP, NOM, PRENOM, DATE_EMBAUCHE, SALAIRE, POSTE, EMAIL, SEXE, PSW, ID_ENTRAINEUR, ID_M) "
                  "VALUES (:idEmp, :nom, :prenom, :dateEmbauche, :salaire, :poste, :email, :sexe, :password, :idEntraineur, :idM)");

    query.bindValue(":idEmp", idEmp);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":dateEmbauche", date);
    query.bindValue(":salaire", salaire);
    query.bindValue(":poste", poste);
    query.bindValue(":email", email);
    query.bindValue(":sexe", sexe);
    query.bindValue(":password", password);
    query.bindValue(":idEntraineur", idEntraineur);
    query.bindValue(":idM", idM);

    if (query.exec()) {
        qDebug() << "Employé ajouté avec succès.";
        return true;
    } else {
        qDebug() << "Erreur lors de l'ajout de l'employé:" << query.lastError().text();
        return false;
    }
}

bool Employes::deleteFromDatabase(const QString &nom, const QString &prenom) {
    QSqlQuery query;
    query.prepare("DELETE FROM EMPLOYES WHERE NOM = :nom AND PRENOM = :prenom");
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    return query.exec();
}

QSqlQuery Employes::getAllEmployes() {
    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYES");
    query.exec();
    return query;
}

QString Employes::generateUniquePassword() {
    const QString chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    QString password;
    for (int i = 0; i < 8; ++i) {
        int index = QRandomGenerator::global()->bounded(chars.length());
        password.append(chars.at(index));
    }
    return password;
}

// Getters
QString Employes::getNom() const { return nom; }
QString Employes::getPrenom() const { return prenom; }
QString Employes::getDateEmbauche() const { return dateEmbauche; }
QString Employes::getSalaire() const { return salaire; }
QString Employes::getPoste() const { return poste; }
QString Employes::getEmail() const { return email; }
QString Employes::getSexe() const { return sexe; }
QString Employes::getPassword() const { return password; }
int Employes::getIdEntraineur() const { return idEntraineur; }
int Employes::getIdM() const { return idM; }
