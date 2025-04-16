#include "Employes.h"
#include <QUuid>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QRandomGenerator>

Employes::Employes(QString nom, QString prenom, QString dateEmbauche, QString salaire,
                   QString poste, QString email, QString sexe, QString password,
                   int idEntraineur, int idM)
    : nom(nom), prenom(prenom), dateEmbauche(dateEmbauche),
    salaire(salaire), poste(poste), email(email), sexe(sexe),
    password(password), idEntraineur(idEntraineur), idM(idM) {}

bool Employes::addToDatabase() {
    QSqlQuery query;

    // Générer un ID unique pour ID_EMP
    QString idEmp = QUuid::createUuid().toString(QUuid::WithoutBraces); // Format UUID sans { }

    // Requête d'insertion avec les colonnes dans l'ordre exact de la table Oracle
    query.prepare("INSERT INTO EMPLOYES (ID_EMP, NOM, PRENOM, DATE_EMBAUCHE, SALAIRE, POSTE, EMAIL, SEXE, PSW, ID_ENTRAINEUR, ID_M) "
                  "VALUES (:idEmp, :nom, :prenom, TO_DATE(:dateEmbauche, 'YYYY-MM-DD'), :salaire, :poste, :email, :sexe, :password, :idEntraineur, :idM)");

    query.bindValue(":idEmp", idEmp);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":dateEmbauche", dateEmbauche); // format attendu: "YYYY-MM-DD"
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

bool Employes::updateToDatabase(int id)
{
    QSqlQuery query;

    // First check if the employee exists
    query.prepare("SELECT COUNT(*) FROM EMPLOYES WHERE ID_EMP = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Error checking employee existence:" << query.lastError().text();
        return false;
    }

    if (!query.next()) {
        qDebug() << "No result returned when checking employee existence";
        return false;
    }

    if (query.value(0).toInt() == 0) {
        qDebug() << "Employee with ID" << id << "does not exist";
        return false;
    }

    // If employee exists, proceed with update
    query.prepare("UPDATE EMPLOYES SET "
                  "NOM = :nom, "
                  "PRENOM = :prenom, "
                  "DATE_EMBAUCHE = TO_DATE(:dateEmbauche, 'YYYY-MM-DD'), "
                  "SALAIRE = :salaire, "
                  "POSTE = :poste, "
                  "EMAIL = :email, "
                  "SEXE = :sexe, "
                  "PSW = :password, "
                  "ID_ENTRAINEUR = :idEntraineur, "
                  "ID_M = :idM "
                  "WHERE ID_EMP = :id");

    // Log the values being bound
    qDebug() << "Binding values:";
    qDebug() << "ID:" << id;
    qDebug() << "Nom:" << nom;
    qDebug() << "Prenom:" << prenom;
    qDebug() << "Date:" << dateEmbauche;
    qDebug() << "Salaire:" << salaire;
    qDebug() << "Poste:" << poste;
    qDebug() << "Email:" << email;
    qDebug() << "Sexe:" << sexe;
    qDebug() << "Password:" << password;
    qDebug() << "ID Entraineur:" << idEntraineur;
    qDebug() << "ID M:" << idM;

    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":dateEmbauche", dateEmbauche);
    query.bindValue(":salaire", salaire);
    query.bindValue(":poste", poste);
    query.bindValue(":email", email);
    query.bindValue(":sexe", sexe);
    query.bindValue(":password", password);
    query.bindValue(":idEntraineur", idEntraineur);
    query.bindValue(":idM", idM);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Error updating employee:" << query.lastError().text();
        qDebug() << "Last query:" << query.lastQuery();
        return false;
    }

    qDebug() << "Update successful for employee ID:" << id;
    return true;
}
bool Employes::deleteById(int id)
{
    QSqlQuery query;

    // First check if the employee exists
    query.prepare("SELECT COUNT(*) FROM EMPLOYES WHERE ID_EMP = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Error checking employee existence:" << query.lastError().text();
        return false;
    }

    if (!query.next() || query.value(0).toInt() == 0) {
        qDebug() << "Employee with ID" << id << "does not exist";
        return false;
    }

    // If employee exists, proceed with deletion
    query.prepare("DELETE FROM EMPLOYES WHERE ID_EMP = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Error deleting employee:" << query.lastError().text();
        return false;
    }

    qDebug() << "Employee with ID" << id << "deleted successfully";
    return true;
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
