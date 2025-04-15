#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connection.h"
#include <QMessageBox>
#include <QMessageBox>
#include <QDate>
#include <QVBoxLayout>


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


    moisAffiche = QDate::currentDate();
    genererCalendrier(Match::getAllMatches(), moisAffiche);
    updateMoisLabel(); // <-- très important

}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::on_ajouterMatch_clicked()
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
        QMessageBox msgBox;
        msgBox.setStyleSheet("QLabel { color: white; } "
                             "QMessageBox { background-color: #2c2c2c; } "
                             "QPushButton { background-color: #e45638; color: white; border-radius: 5px; padding: 5px; }");

        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle("Ajout");
        msgBox.setText("Ajout effectué avec succès !");
        msgBox.exec();
    } else {
        QMessageBox::critical(this, "Erreur", "Une erreur est survenue lors de l'ajout du match.");

    }
    qApp->setStyleSheet(R"(
    QMessageBox {
        background-color: white;
    }
    QMessageBox QLabel {
        color: black;
        font-size: 14px;
    }
    QMessageBox QPushButton {
        background-color: #e45638;
        color: white;
        padding: 5px;
        border-radius: 5px;
    }
)");

}




void MainWindow::on_recherche_button_clicked()
{
    int id=ui->recherche->text().toInt();
    QSqlQueryModel* sortedModel = tmpMatch.chercher(id);
    if (sortedModel) {

        ui->tableView->setModel(sortedModel);

        QMessageBox::information(nullptr, QObject::tr("OK"), QObject::tr("recherche effectué.\nClick Cancel to exit."), QMessageBox::Cancel);
    } else {

        QMessageBox::critical(nullptr, QObject::tr("Not OK"), QObject::tr("recherche non effectué.\nClick Cancel to exit."), QMessageBox::Cancel);
    }
}




void MainWindow::on_exporter_clicked()
{
    bool test=tmpMatch.PDF();
    if (test)
    {

        QMessageBox::information(nullptr, QObject::tr("OK"), QObject::tr("pdf effectué.\nClick Cancel to exit."), QMessageBox::Cancel);
    }
    else
        QMessageBox::critical(nullptr, QObject::tr("Not OK"), QObject::tr("pdf non effectué.\nClick Cancel to exit."), QMessageBox::Cancel);
}



void MainWindow::on_stat_clicked()
    {
            QChartView* chartView =tmpMatch.stat(); // Récupère le graphique

            // Vérifie si un layout existe déjà dans statContainer
            if (chartView) {
                // Créer une boîte de dialogue pour afficher le graphique
                QDialog* statsDialog = new QDialog(this);
                statsDialog->setWindowTitle("Statistiques des clients");
                statsDialog->resize(600, 400);

                QVBoxLayout* layout = new QVBoxLayout(statsDialog);
                layout->addWidget(chartView);
                statsDialog->setLayout(layout);

                statsDialog->exec();
            }
    }






    void MainWindow::on_tri_clicked()
    {
        QString critere = ui->par->currentText().toLower(); // récupère le critère sélectionné (date, equipe, stade)
        QSqlQueryModel* sortedModel = tmpMatch.trier(critere);

        if (sortedModel) {
            ui->tableView->setModel(sortedModel);
            QString message = "Tri effectué par " + critere + ".";
            QMessageBox::information(nullptr, QObject::tr("OK"), message, QMessageBox::Cancel);
        } else {
            QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("Échec du tri."), QMessageBox::Cancel);
        }

    }

    void MainWindow::genererCalendrier(const QList<Match>& matchs, QDate mois)
    {
        qDebug() << "Génération du calendrier pour :" << mois.toString("MMMM yyyy");
        qDebug() << "Nombre de matchs :" << matchs.size();

        // Définir les en-têtes
        ui->calendarTable->clear();
        ui->calendarTable->setColumnCount(7);
        ui->calendarTable->setHorizontalHeaderLabels({"Lun", "Mar", "Mer", "Jeu", "Ven", "Sam", "Dim"});

        QDate premierJour(mois.year(), mois.month(), 1);
        int jourSemaine = premierJour.dayOfWeek(); // 1 (lundi) à 7 (dimanche)
        int joursDansMois = mois.daysInMonth();
        int lignes = qCeil((jourSemaine - 1 + joursDansMois) / 7.0);
        ui->calendarTable->setRowCount(lignes);

        // Création initiale des cellules avec numéro du jour
        for (int jour = 1; jour <= joursDansMois; ++jour) {
            QDate date = QDate(mois.year(), mois.month(), jour);
            int index = (jourSemaine - 2) + (jour - 1);
            int row = index / 7;
            int col = index % 7;

            QTableWidgetItem* item = new QTableWidgetItem(QString::number(jour));
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            ui->calendarTable->setItem(row, col, item);

        }

        // Ajout des matchs
        for (const Match& m : matchs) {
            QDate dateMatch = m.getDate();

            if (dateMatch.month() != mois.month() || dateMatch.year() != mois.year())
                continue;

            int jour = dateMatch.day();
            int index = (jourSemaine - 2) + (jour - 1);
            int row = index / 7;
            int col = index % 7;

            QTableWidgetItem* item = ui->calendarTable->item(row, col);
            if (!item) {
                item = new QTableWidgetItem(QString::number(jour));
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                ui->calendarTable->setItem(row, col, item);
            }

            QString text = item->text();
            text += "\n - " + m.getEquipe1() + " vs " + m.getEquipe2();

            item->setText(text);
            item->setBackground(QColor("#2E86C1"));  // bleu léger
            item->setForeground(QColor("white"));    // texte blanc

        }

        // Mise à jour de la taille
        ui->calendarTable->resizeColumnsToContents();
        ui->calendarTable->resizeRowsToContents();
    }


    void MainWindow::on_calendarButton_clicked()
    {
        QDate moisActuel = QDate::currentDate();
        QList<Match> liste = Match::getAllMatches(); // ta fonction métier
        genererCalendrier(liste, moisActuel);

    }

    void MainWindow::updateMoisLabel()
    {
        QString moisAnnee = moisAffiche.toString("MMMM yyyy");
        moisAnnee[0] = moisAnnee[0].toUpper(); // capitaliser la première lettre
        ui->lblMoisAnnee->setText(moisAnnee);
    }


    void MainWindow::on_btnMoisSuivant_clicked()
    {
        moisAffiche = moisAffiche.addMonths(1);
        genererCalendrier(Match::getAllMatches(), moisAffiche);
        updateMoisLabel();
    }

    void MainWindow::on_btnMoisPrecedent_clicked()
    {
        moisAffiche = moisAffiche.addMonths(-1);
        genererCalendrier(Match::getAllMatches(), moisAffiche);
        updateMoisLabel();
    }

    void MainWindow::on_btnAnneeSuivante_clicked()
    {
        moisAffiche = moisAffiche.addYears(1);
        genererCalendrier(Match::getAllMatches(), moisAffiche);
        updateMoisLabel();
    }

    void MainWindow::on_btnAnneePrecedente_clicked()
    {
        moisAffiche = moisAffiche.addYears(-1);
        genererCalendrier(Match::getAllMatches(), moisAffiche);
        updateMoisLabel();
    }





