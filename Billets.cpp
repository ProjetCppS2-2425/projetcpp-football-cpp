#include "billets.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

// Default constructor
Billets::Billets() {
    // Initialize member variables if needed
}

// Parameterized constructor
Billets::Billets(int CIN, int nombreDeBillets, QDate dateEmission, int siege, QString type, double prix, int idM)
    : CIN(CIN), nombreDeBillets(nombreDeBillets), dateEmission(dateEmission), siege(siege), type(type), prix(prix), idM(idM) {
    // Initialize member variables with provided values
}

// Getters
int Billets::getCIN() { return CIN; }
int Billets::getNombreDeBillets() { return nombreDeBillets; }
QDate Billets::getDateEmission() { return dateEmission; }
int Billets::getSiege() { return siege; }
QString Billets::getType() { return type; }
double Billets::getPrix() { return prix; }
int Billets::getIdM() { return idM; }

// Setters
void Billets::setCIN(int cin) { this->CIN = cin; }
void Billets::setNombreDeBillets(int nombre) { this->nombreDeBillets = nombre; }
void Billets::setDateEmission(QDate date) { this->dateEmission = date; }
void Billets::setSiege(int siege) { this->siege = siege; }
void Billets::setType(QString type) { this->type = type; }
void Billets::setPrix(double prix) { this->prix = prix; }
void Billets::setIdM(int id) { this->idM = id; }

// Method to add a billet to the database
bool Billets::ajouterBillet() {
    QSqlQuery query;

    // Check if the database is open
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return false;
    } else {
        qDebug() << "Database is open and connected.";
    }

    // Prepare the SQL query
    query.prepare("INSERT INTO MARAM.BILLETS (CIN, NOMBRE_DE_BILLETS, DATE_EMISSION, SIEGE, TYPE, PRIX, ID_M) "
                  "VALUES (:CIN, :NOMBRE_DE_BILLETS, TO_DATE(:DATE_EMISSION, 'YYYY-MM-DD'), :SIEGE, :TYPE, :PRIX, :ID_M)");

    // Bind values to the query
    query.bindValue(":CIN", CIN);
    query.bindValue(":NOMBRE_DE_BILLETS", nombreDeBillets);
    query.bindValue(":DATE_EMISSION", dateEmission.toString("yyyy-MM-dd")); // Ensure correct date format
    query.bindValue(":SIEGE", siege);
    query.bindValue(":TYPE", type);
    query.bindValue(":PRIX", prix);
    query.bindValue(":ID_M", idM);

    // Debug: Print the query and bound values
    qDebug() << "Executing query:" << query.lastQuery();
    qDebug() << "Bound values:" << query.boundValues();

    // Execute the query
    if (query.exec()) {
        qDebug() << "Billet added successfully!";
        return true; // Success
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                              QObject::tr("Erreur d'ajout du billet: %1").arg(query.lastError().text()), QMessageBox::Ok);
        qDebug() << "Error adding billet:" << query.lastError().text();
        return false; // Failure
    }
}
// Method to delete a billet from the database
bool Billets::supprimerBillet(int cin) {
    QSqlQuery query;
    query.prepare("DELETE FROM MARAM.BILLETS WHERE CIN = :CIN");
    query.bindValue(":CIN", cin);

    if (query.exec()) {
        qDebug() << "Billet deleted successfully!";
        return true;
    } else {
        qDebug() << "Error deleting billet:" << query.lastError().text();
        return false;
    }
}

// Method to modify a billet in the database
bool Billets::modifierBillet() {
    if (CIN <= 0) {
        qDebug() << "Invalid CIN!";
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE MARAM.BILLETS SET NOMBRE_DE_BILLETS = :NOMBRE_DE_BILLETS, DATE_EMISSION = TO_DATE(:DATE_EMISSION, 'YYYY-MM-DD'), "
                  "SIEGE = :SIEGE, TYPE = :TYPE, PRIX = :PRIX, ID_M = :ID_M WHERE CIN = :CIN");

    query.bindValue(":CIN", CIN);
    query.bindValue(":NOMBRE_DE_BILLETS", nombreDeBillets);
    query.bindValue(":DATE_EMISSION", dateEmission.toString("yyyy-MM-dd"));
    query.bindValue(":SIEGE", siege);
    query.bindValue(":TYPE", type);
    query.bindValue(":PRIX", prix);
    query.bindValue(":ID_M", idM);

    if (query.exec()) {
        qDebug() << "Billet updated successfully!";
        return true;
    } else {
        qDebug() << "Error updating billet:" << query.lastError().text();
        return false;
    }
}


QSqlQueryModel* Billets::afficherBillets()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT * FROM MARAM.BILLETS");

    if (model->lastError().isValid()) {
        qDebug() << "Error displaying billets:" << model->lastError().text();
    }

    return model;
}

