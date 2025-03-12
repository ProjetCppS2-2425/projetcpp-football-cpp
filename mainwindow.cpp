#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsTextItem>
#include <QPainter>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTableWidget>
#include <QMap>
#include "Equipes.h"
#include <QMessageBox>
#include <QRadioButton>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->aff_3, &QTableWidget::cellClicked, this, &MainWindow::on_aff_3_cellClicked);
    // ui->logo->setPixmap(QPixmap("C:\\Users\\dhibm\\Desktop\\workshift\\assets\\workshift-removebg-preview.png"));
    pageMap[ui->Employes_2] = ui->stackedWidget->indexOf(ui->page1);
    pageMap[ui->Matches_3] = ui->stackedWidget->indexOf(ui->page2);
    pageMap[ui->Equipes_3] = ui->stackedWidget->indexOf(ui->page3);
    pageMap[ui->Entraineurs_3] = ui->stackedWidget->indexOf(ui->page4);
    pageMap[ui->Billets_3] = ui->stackedWidget->indexOf(ui->page5);
    pageMap[ui->Sponsors_3] = ui->stackedWidget->indexOf(ui->page6);

    // Connecter chaque bouton dynamiquement
    for (auto it = pageMap.begin(); it != pageMap.end(); ++it) {
        connect(it.key(), &QPushButton::clicked, this, [=]() {
            ui->stackedWidget->setCurrentIndex(it.value());
        });
    }
}
void MainWindow::showEmployes() {
    ui->stackedWidget->setCurrentIndex(1); // Page Employe
}

void MainWindow::showMatches() {
    ui->stackedWidget->setCurrentIndex(2); // Page Client
}

void MainWindow::showEquipes() {
    ui->stackedWidget->setCurrentIndex(3); // Page Conseil
}

void MainWindow::showEntreneurs() {
    ui->stackedWidget->setCurrentIndex(4); // Page Transaction
}

void MainWindow::showBillets() {
    ui->stackedWidget->setCurrentIndex(5); // Page Partenaires
}
void MainWindow::showSponsors() {
    ui->stackedWidget->setCurrentIndex(6); // Page Partenaires
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_pushButton_afficher_clicked()
{
    Equipes eq;
    QSqlQueryModel *model = eq.afficher();

    ui->aff_3->setRowCount(model->rowCount());
    ui->aff_3->setColumnCount(model->columnCount());

    for (int i = 0; i < model->rowCount(); ++i) {
        for (int j = 0; j < model->columnCount(); ++j) {
            QString value = model->data(model->index(i, j)).toString();
            ui->aff_3->setItem(i, j, new QTableWidgetItem(value));
        }
    }
}
void MainWindow::on_Valider_3_clicked()
{
    int id = ui->id_3->text().toInt();
    QString nom = ui->nom_3->text();
    QString nationalité = ui->nationalite_7->text();
    QString email = ui->Email_3->text();
    double nombre = ui->nombre_3->text().toDouble();
    QString rank = ui->rank_3->currentText();

    // Vérification de l'ID (doit être un entier positif)
    if (ui->id_3->text().isEmpty() || id <= 0) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("L'ID doit être un entier positif."), QMessageBox::Cancel);
        return;
    }




    // Vérification de la note (doit être entre 0 et 20)
    if (nombre < 0 || nombre > 20) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("La nombre doit être comprise entre 0 et 20."), QMessageBox::Cancel);
        return;
    }

    // Vérification que les champs ne sont pas vides
    if (email.isEmpty() || nom.isEmpty() || nationalité.isEmpty()) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("Veuillez remplir tous les champs."), QMessageBox::Cancel);
        return;
    }

    // Vérification si l'ID existe déjà
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM EQUIPES WHERE id = :Id_Eq");
    checkQuery.bindValue(":Id_Eq",id);
    if (checkQuery.exec() && checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("Cet ID existe déjà dans la base de données."), QMessageBox::Cancel);
        return;
    }

    // Création de l'objet et tentative d'ajout
    Equipes eq(id, nom, nationalité, email, nombre, rank);
    bool test = eq.ajouter();
    if (test) {
        QMessageBox::information(nullptr, QObject::tr("Succès"), QObject::tr("Ajout réussi."), QMessageBox::Ok);
        on_pushButton_afficher_clicked(); // Actualiser l'affichage

        // Réinitialiser les champs après l'ajout
        ui->id_3->clear();
        ui->nom_3->clear();
        ui->nationalite_7->clear();
        ui->Email_3->clear();
        ui->nombre_3->clear();
        ui->rank_3->clear();
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Échec"), QObject::tr("Erreur lors de l'ajout."), QMessageBox::Cancel);
    }
}

// Fonction pour modifier un résultat
void MainWindow::on_modifier_3_clicked()
{
    int id = ui->id_3->text().toInt();
    QString nom = ui->nom_3->text();
    QString nationalité = ui->nationalite_7->text();
    QString email = ui->Email_3->text();
    double nombre = ui->nombre_3->text().toDouble();
    QString rank = ui->rank_3->currentText(); // Changed from text() to currentText()

    Equipes eq(id, nom, nationalité, email, nombre, rank);
    bool test = eq.modifier(id); // Changed from ajouter() to modifier()
    if (test) {
        QMessageBox::information(nullptr, QObject::tr("Succès"), QObject::tr("Modification réussie."), QMessageBox::Ok);
        on_pushButton_afficher_clicked();
        ui->id_3->clear();
        ui->nom_3->clear();
        ui->nationalite_7->clear();
        ui->Email_3->clear();
        ui->nombre_3->clear();
        // ui->rank_3->clear(); // QComboBox doesn't have clear()
        ui->rank_3->setCurrentIndex(0); // Reset to first item
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Échec"), QObject::tr("Erreur lors de la modification."), QMessageBox::Cancel);
    }
}

// Fonction pour supprimer un résultat
void MainWindow::on_supprimer_3_clicked()
{
    int Id_Eq = ui->id_3->text().toInt();

    if (Id_Eq == 0) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("Veuillez entrer un ID valide."), QMessageBox::Cancel);
        return;
    }

    Equipes eq;
    bool test = eq.supprimer(Id_Eq);
    if (test) {
        QMessageBox::information(nullptr, QObject::tr("Succès"), QObject::tr("Suppression réussie."), QMessageBox::Ok);
        on_pushButton_afficher_clicked();

        ui->id_3->clear();
        ui->nom_3->clear();
        ui->nationalite_7->clear();
        ui->Email_3->clear();
        ui->nombre_3->clear();
        ui->rank_3->clear();
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Échec"), QObject::tr("Erreur lors de la suppression."), QMessageBox::Cancel);
    }
}

void MainWindow::on_aff_3_cellClicked(int row, int column)
{
    Q_UNUSED(column); // Add this to avoid unused parameter warning

    // Récupérer les valeurs de la ligne sélectionnée
    QString ID_Eq = ui->aff_3->item(row, 0)->text();
    QString Nom_De_Equipe = ui->aff_3->item(row, 1)->text();
    QString Nationalité = ui->aff_3->item(row, 2)->text();
    QString Email = ui->aff_3->item(row, 3)->text();
    QString Nombre_De_Joueur = ui->aff_3->item(row, 4)->text();
    QString Rank = ui->aff_3->item(row, 5)->text();

    // Remplir les QLineEdit avec les valeurs sélectionnées
    ui->id_3->setText(ID_Eq);
    ui->nom_3->setText(Nom_De_Equipe);
    ui->nationalite_7->setText(Nationalité);
    ui->Email_3->setText(Email);
    ui->nombre_3->setText(Nombre_De_Joueur);
    ui->rank_3->setCurrentText(Rank); // Changed from setText() to setCurrentText()
}

