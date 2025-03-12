#include "Employes.h"

// Update the constructor to accept the extra parameters for idEntraineur and idM
Employes::Employes(QString nom, QString prenom, QString dateEmbauche, QString salaire,
                   QString poste, QString email, QString sexe, QString password,
                   int idEntraineur, int idM) // Added missing parameters
    : nom(nom), prenom(prenom), dateEmbauche(dateEmbauche),
    salaire(salaire), poste(poste), email(email), sexe(sexe), password(password),
    idEntraineur(idEntraineur), idM(idM) {} // Initialize idEntraineur and idM

bool Employes::addToDatabase() {
    QSqlQuery query;
    query.prepare("INSERT INTO EMPLOYES (NOM, PRENOM, DATE_EMBAUCHE, SALAIRE, POSTE, EMAIL, SEXE, PSW, ID_ENTRAINEUR, ID_M)"
                  "VALUES (:nom, :prenom, :dateEmbauche, :salaire, :poste, :email, :sexe, :password, :idEntraineur, :idM)");
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":dateEmbauche", dateEmbauche);
    query.bindValue(":salaire", salaire);
    query.bindValue(":poste", poste);
    query.bindValue(":email", email);
    query.bindValue(":sexe", sexe);
    query.bindValue(":password", password);
    query.bindValue(":idEntraineur", idEntraineur);  // Bind idEntraineur
    query.bindValue(":idM", idM);  // Bind idM
    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Error adding employee: " << query.lastError().text();
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

// **Added Missing Function**
QString Employes::generateUniquePassword() {
    const QString chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    QString password;
    for (int i = 0; i < 8; ++i) {
        int index = QRandomGenerator::global()->bounded(chars.length());
        password.append(chars.at(index));
    }
    return password;
}
