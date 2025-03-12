#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "billets.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
     // Remplir le QComboBox avec les ID_M
    remplirComboBoxMatchs();

    // Change the text color for specific widgets to white
    ui->lineEdit_CIN_Edit->setStyleSheet("color: white;");
    ui->nombre_de_billets->setStyleSheet("color: white;");
    ui->siege->setStyleSheet("color: white;");
    ui->type->setStyleSheet("color: white;");
    ui->prix->setStyleSheet("color: white;");
    ui->table_Billets->setStyleSheet("QTableView { color: white; }");
    ui->matchs->setStyleSheet("color: white;");


    billets = new Billets();
    ui->table_Billets->setModel(billets->afficherBillets());

    //connect(ui->ajouter, &QPushButton::clicked, this, &MainWindow::on_ajouter_clicked);
    //connect(ui->supprimer, &QPushButton::clicked, this, &MainWindow::on_supprimer_clicked);
   // connect(ui->modifier, &QPushButton::clicked, this, &MainWindow::on_edit_clicked);
   // connect(ui->afficher, &QPushButton::clicked, this, &MainWindow::on_afficher_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete billets;
}

void MainWindow::on_ajouter_clicked()
{
    QString cinText = ui->lineEdit_CIN_Edit->toPlainText();
    bool ok;
    int CIN = cinText.toInt(&ok);

    if (!ok) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Erreur");
        msgBox.setText("Veuillez entrer un CIN valide.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QLabel { color : black; }");
        msgBox.exec();
        return;
    }

    int nombreDeBillets = ui->nombre_de_billets->text().toInt();
    QDate dateEmission = ui->date->date();
    int siege = ui->siege->text().toInt();
    QString type = ui->type->currentText();
    double prix = ui->prix->text().toDouble();

    billets->setCIN(CIN);
    billets->setNombreDeBillets(nombreDeBillets);
    billets->setDateEmission(dateEmission);
    billets->setSiege(siege);
    billets->setType(type);
    billets->setPrix(prix);

    if (billets->ajouterBillet())
    {
        QMessageBox::information(nullptr, QObject::tr("Succès"),
                                 QObject::tr("Le billet a été ajouté avec succès.\n"), QMessageBox::Ok);
        ui->table_Billets->setModel(billets->afficherBillets());
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                              QObject::tr("Impossible d'ajouter le billet.\n"), QMessageBox::Ok);
    }
}

void MainWindow::on_supprimer_clicked()
{
    QString cinText = ui->lineEdit_CIN_Edit->toPlainText();
    bool ok;
    int CIN = cinText.toInt(&ok);

    if (!ok) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Erreur");
        msgBox.setText("Veuillez entrer un CIN valide.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QLabel { color : black; }");
        msgBox.exec();
        return;
    }

    if (billets->supprimerBillet(CIN))
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Succès");
        msgBox.setText("Le billet a été supprimé avec succès.\n");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet("QLabel { color : black; }"); // Texte en blanc
        msgBox.exec();
       ui->table_Billets->setModel(billets->afficherBillets());
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Erreur");
        msgBox.setText("Impossible de supprimer ce billet.\n");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStyleSheet("QLabel { color : black; }"); // Texte en blanc
        msgBox.exec();
    }
}

void MainWindow::on_modifier_clicked()
{
    QString cinText = ui->lineEdit_CIN_Edit->toPlainText();
    bool ok;
    int CIN = cinText.toInt(&ok);

    if (!ok) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Erreur");
        msgBox.setText("Veuillez entrer un CIN valide.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QLabel { color : black; }");
        msgBox.exec();
        return;
    }

    int nombreDeBillets = ui->nombre_de_billets->text().toInt();
    QDate dateEmission = ui->date->date();
    int siege = ui->siege->text().toInt();
    QString type = ui->type->currentText();
    double prix = ui->prix->text().toDouble();

    billets->setCIN(CIN);
    billets->setNombreDeBillets(nombreDeBillets);
    billets->setDateEmission(dateEmission);
    billets->setSiege(siege);
    billets->setType(type);
    billets->setPrix(prix);



        if (billets->modifierBillet())
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Succès");
            msgBox.setText("Le billet a été modifié avec succès.");
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStyleSheet("QLabel { color : black; }"); // Appliquer le style
            msgBox.exec();
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Erreur");
            msgBox.setText("Impossible de modifier ce billet.");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setStyleSheet("QLabel { color : black; }");
            msgBox.exec();
        }
    }

void MainWindow::on_afficher_clicked()
{
    ui->table_Billets->setModel(billets->afficherBillets());
}
bool MainWindow::idExists(int id_m) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM MATCHS WHERE ID_M = :id_m");
    query.bindValue(":id_m", id_m);

    if (query.exec()) {
        if (query.next()) {
            int count = query.value(0).toInt();
            return count > 0;
        }
    } else {
        QMessageBox::critical(this, "Erreur", query.lastError().text());
    }
    return false;
}
void MainWindow::remplirComboBoxMatchs() {
    QSqlQuery query("SELECT ID_M FROM MATCHS");
    while (query.next()) {
        int id_m = query.value(0).toInt();
        ui->matchs->addItem(QString::number(id_m), id_m);
    }
}

void MainWindow::ajouterBillet() {
    int id_m = ui->matchs->currentData().toInt();

    if (!idExists(id_m)) {
        QMessageBox::warning(this, "Erreur", "ID_M n'existe pas dans MATCHS.");
        return;
    }

    // Code pour insérer le billet
    QSqlQuery query;
    query.prepare("INSERT INTO BILLETS (CIN, NOMBRE_DE_BILLETS, DATE_EMISSION, SIEGE, TYPE, PRIX, ID_M) "
                  "VALUES (:cin, :nb_billets, :date_emission, :siege, :type, :prix, :id_m)");
    query.bindValue(":cin", ui->lineEdit_CIN_Edit->toPlainText());
    query.bindValue(":nb_billets", ui->nombre_de_billets->text().toInt());
    query.bindValue(":date_emission", ui->date->date());
    query.bindValue(":siege", ui->siege->text().toInt());
    query.bindValue(":type", ui->type->currentText());
    query.bindValue(":prix", ui->prix->text().toDouble());
    query.bindValue(":id_m", id_m);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", query.lastError().text());
    } else {
        QMessageBox::information(this, "Succès", "Billet ajouté avec succès.");
    }
}
