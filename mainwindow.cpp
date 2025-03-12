#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connection.h"
#include <QMessageBox>
#include <QMessageBox>
#include <QDate>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Connection conn;
    if(!conn.createconnect())
        QMessageBox::critical(nullptr, QObject::tr("Database Error"),
                              QObject::tr("Connexion à la base de données échouée.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_ajouter_clicked()
{

    int id_m=tmpMatch.nextid();
    QString equipe1 = ui->lineEdit_equipe1->text();
    QString equipe2 = ui->lineEdit_equipe2->text();
    QDate date = ui->dateEdit_date->date();
    QString lieu = ui->lineEdit_lieu->text();
    QString type = ui->comboBox_type->currentText();
    QString etat = ui->comboBox_etat->currentText();

    Match tmpmatch(id_m, equipe1, equipe2, date, lieu, type, etat);

        // Contrôle de saisie
        QString errorMessage;

        if (equipe1.isEmpty() || equipe2.isEmpty()) {
            errorMessage += "Les noms des équipes ne peuvent pas être vides.\n";
        }

        if (equipe1 == equipe2) {
            errorMessage += "Les deux équipes ne peuvent pas être identiques.\n";
        }

        if (date < QDate::currentDate()) {
            errorMessage += "La date du match ne peut pas être dans le passé.\n";
        }

        if (lieu.isEmpty()) {
            errorMessage += "Le lieu du match ne peut pas être vide.\n";
        }

        // Si des erreurs ont été détectées, les afficher et arrêter l'ajout
        if (!errorMessage.isEmpty()) {
            QMessageBox::warning(this, "Erreur de saisie", errorMessage);
            return;
        }

        // Si tout est correct, procéder à l'ajout
        Match m(id_m, equipe1, equipe2, date, lieu, type, etat);
        bool success = m.ajouter();

        if(success) {
            QMessageBox::information(this, "Succès", "Le match a été ajouté avec succès.");
            refreshTable();
            // Nettoyer les champs après l'ajout
            clearInputFields();
        } else {
            QMessageBox::critical(this, "Erreur", "Une erreur est survenue lors de l'ajout du match.");
        }
    }

    // Fonction pour nettoyer les champs après l'ajout
    void MainWindow::clearInputFields()
    {
        ui->lineEdit_equipe1->clear();
        ui->lineEdit_equipe2->clear();
        ui->dateEdit_date->setDate(QDate::currentDate());
        ui->lineEdit_lieu->clear();
        ui->comboBox_type->setCurrentIndex(0);
        ui->comboBox_etat->setCurrentIndex(0);
    }




 /*   if(success)
    {
        ui->tableView->setModel(tmpMatch.afficher());

        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Ajout effectué.\n"
                                             "Click Cancel to exit."), QMessageBox::Cancel);
        refreshTable();
    }
    else
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Ajout non effectué.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);

}*/

void MainWindow::on_pushButton_supprimer_clicked()
{
    ui->tableView->setModel(tmpMatch.afficher());

    int id_m = ui->lineEdit_id->text().toInt();
    bool success = tmpMatch.supprimer(id_m);

    if(success)
    {
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Suppression effectuée.\n"
                                             "Click Cancel to exit."), QMessageBox::Cancel);
        refreshTable();
    }
    else
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Suppression non effectuée.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
}

void MainWindow::on_pushButton_modifier_clicked()
{
    ui->tableView->setModel(tmpMatch.afficher());

    int id_m = ui->lineEdit_id->text().toInt();
    QString equipe1 = ui->lineEdit_equipe1->text();
    QString equipe2 = ui->lineEdit_equipe2->text();
    QDate date = ui->dateEdit_date->date();
    QString lieu = ui->lineEdit_lieu->text();
    QString type = ui->comboBox_type->currentText();
    QString etat = ui->comboBox_etat->currentText();


    bool success = tmpMatch.modifier( id_m, equipe1,  equipe2,  date,
                               lieu,  type,  etat);

    if(success)
    {
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Modification effectuée.\n"
                                             "Click Cancel to exit."), QMessageBox::Cancel);
        refreshTable();
    }
    else
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Modification non effectuée.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    int row = index.row();
    ui->lineEdit_id->setText(ui->tableView->model()->data(ui->tableView->model()->index(row, 0)).toString());
    ui->lineEdit_equipe1->setText(ui->tableView->model()->data(ui->tableView->model()->index(row, 1)).toString());
    ui->lineEdit_equipe2->setText(ui->tableView->model()->data(ui->tableView->model()->index(row, 2)).toString());
    ui->dateEdit_date->setDate(ui->tableView->model()->data(ui->tableView->model()->index(row, 3)).toDate());
    ui->lineEdit_lieu->setText(ui->tableView->model()->data(ui->tableView->model()->index(row, 4)).toString());
    ui->comboBox_type->setCurrentText(ui->tableView->model()->data(ui->tableView->model()->index(row, 5)).toString());
    ui->comboBox_etat->setCurrentText(ui->tableView->model()->data(ui->tableView->model()->index(row, 6)).toString());
}

void MainWindow::refreshTable()
{
    ui->tableView->setModel(tmpMatch.afficher());
}
