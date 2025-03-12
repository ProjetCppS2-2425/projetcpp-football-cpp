#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Employes.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->groupBox_3->setVisible(false);
    ui->tabWidget->setVisible(false);
    changePage(0);

    connect(ui->Login, &QPushButton::clicked, this, [=]() { changePage(1); });
    connect(ui->Employes_2, &QPushButton::clicked, this, [=]() { changePage(1); });
    connect(ui->Matches_3, &QPushButton::clicked, this, [=]() { changePage(2); });
    connect(ui->Equipes_3, &QPushButton::clicked, this, [=]() { changePage(3); });
    connect(ui->Entraineurs_3, &QPushButton::clicked, this, [=]() { changePage(4); });
    connect(ui->Billets_3, &QPushButton::clicked, this, [=]() { changePage(5); });
    connect(ui->Sponsors_3, &QPushButton::clicked, this, [=]() { changePage(6); });

    connect(ui->Valider, &QPushButton::clicked, this, &MainWindow::on_Valider_clicked);
    ui->tableWidget->setSortingEnabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changePage(int index)
{
    if (index == 0) {
        ui->stackedWidget->setCurrentIndex(0);
        ui->groupBox_3->setVisible(false);
        ui->tabWidget->setVisible(false);
    } else {
        ui->stackedWidget->setCurrentIndex(index);
        ui->groupBox_3->setVisible(true);
        ui->tabWidget->setVisible(true);
    }
}

void MainWindow::on_Valider_clicked()
{
    QString nom = ui->nom->text();
    QString prenom = ui->prenom->text();
    QString dateEmbauche = ui->date_emb->date().toString("yyyy-MM-dd");
    QString salaire = ui->salaire->text();
    QString poste = ui->poste->currentText();
    QString email = ui->email->text();
    QString sexe = ui->homme->isChecked() ? "Homme" : "Femme";

    QString password = Employes::generateUniquePassword(); // Fix constructor issue

    // Assuming you have these values ready, for example:
    int idEntraineur = 1; // Replace with actual value
    int idM = 2;          // Replace with actual value

    if (!nom.isEmpty() && !prenom.isEmpty() && !dateEmbauche.isEmpty() && !salaire.isEmpty() && !email.isEmpty()) {
        Employes emp(nom, prenom, dateEmbauche, salaire, poste, email, sexe, password, idEntraineur, idM); // Now using 10 arguments

        if (emp.addToDatabase()) { // Fixed function name
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);

            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(nom));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(prenom));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(dateEmbauche));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(salaire));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(poste));
            ui->tableWidget->setItem(row, 5, new QTableWidgetItem(email));
            ui->tableWidget->setItem(row, 6, new QTableWidgetItem(sexe));
            ui->tableWidget->setItem(row, 7, new QTableWidgetItem(password));

            QPushButton *modifyButton = new QPushButton("Modify");
            connect(modifyButton, &QPushButton::clicked, this, [=]() { modifyRow(row); });
            ui->tableWidget->setCellWidget(row, 8, modifyButton);

            QPushButton *deleteButton = new QPushButton("Delete");
            connect(deleteButton, &QPushButton::clicked, this, [=]() { deleteRow(row); });
            ui->tableWidget->setCellWidget(row, 9, deleteButton);
        }
    } else {
        qDebug() << "Form fields are incomplete. Row not added.";
    }
}



void MainWindow::modifyRow(int row)
{
    editingRow = row;
    ui->nom->setText(ui->tableWidget->item(row, 0)->text());
    ui->prenom->setText(ui->tableWidget->item(row, 1)->text());

    QString dateString = ui->tableWidget->item(row, 2)->text();
    QDate date = QDate::fromString(dateString, "yyyy-MM-dd");
    ui->date_emb->setDate(date);
    ui->salaire->setText(ui->tableWidget->item(row, 3)->text());
    ui->poste->setCurrentText(ui->tableWidget->item(row, 4)->text());
    ui->email->setText(ui->tableWidget->item(row, 5)->text());

    QString sexe = ui->tableWidget->item(row, 6)->text();
    ui->homme->setChecked(sexe == "Homme");
    ui->femme->setChecked(sexe == "Femme");
}

void MainWindow::deleteRow(int row)
{
    ui->tableWidget->removeRow(row);
}
