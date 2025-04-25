#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connection.h"
#include <QMessageBox>
#include <QMessageBox>
#include <QDate>
#include <QVBoxLayout>
#include "match.h"
#include <QPainter>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QGraphicsScene>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSqlQueryModel* modelInitial = tmpMatch.afficher();
    if (modelInitial) {
        ui->tableView->setModel(modelInitial);
        ui->tableView->resizeColumnsToContents();
        ui->tableView->resizeRowsToContents();
    }



    Connection conn;
    if(!conn.createconnect())
        QMessageBox::critical(nullptr, QObject::tr("Database Error"),
                              QObject::tr("Connexion à la base de données échouée.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);


    moisAffiche = QDate::currentDate();
    genererCalendrier(Match::getAllMatches(), moisAffiche);
    updateMoisLabel(); // <-- très important
  //  connect(ui->checkDescendant, &QCheckBox::stateChanged, this, &MainWindow::on_tri_clicked);
    ui->btnMoisSuivant->setCursor(Qt::PointingHandCursor);
    ui->btnMoisPrecedent->setCursor(Qt::PointingHandCursor);
    ui->btnAnneeSuivante->setCursor(Qt::PointingHandCursor);
    ui->btnAnneePrecedente->setCursor(Qt::PointingHandCursor);

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




void MainWindow::on_recherche_button_clicked(QString output )
{
    QString id = ui->lineEdit_id_2->text();           // champ de recherche pour ID
    QString lieu = ui->lineEdit_lieu_2->text();       // champ pour le lieu
    QString equipe1 = ui->lineEdit_equipe1_2->text(); // champ pour l'équipe 1

    QSqlQueryModel* sortedModel = tmpMatch.chercherAvance(id, lieu, equipe1);

    if (sortedModel) {
        ui->tableView->setModel(sortedModel);
        QMessageBox msgBox;
        msgBox.setStyleSheet("QMessageBox { background-color: white; color: black; }"
                             "QLabel { color: black; }"
                             "QPushButton { background-color: lightgray; color: black; }");

        msgBox.setWindowTitle("Recherche");
        msgBox.setText("Recherche effectuée avec succès.");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();

    } else {
        QMessageBox msgBox;
        msgBox.setStyleSheet("QMessageBox { background-color: white; color: black; }"
                             "QLabel { color: black; }"
                             "QPushButton { background-color: lightgray; color: black; }");

        msgBox.setWindowTitle("Erreur");
        msgBox.setText("Une erreur est survenue.");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();

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




void MainWindow::on_exporter_clicked()
{
    bool test=tmpMatch.PDF();
    if (test)
    {

        QMessageBox::information(nullptr, QObject::tr("OK"), QObject::tr("pdf effectué.\nClick Cancel to exit."), QMessageBox::Cancel);
    }
    else
        QMessageBox::critical(nullptr, QObject::tr("Not OK"), QObject::tr("pdf non effectué.\nClick Cancel to exit."), QMessageBox::Cancel);
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


void MainWindow::on_stat_clicked()
{
    // Créer un modèle de requête SQL avec comptage des types de match
    QSqlQuery query;
    int totalamical = 0, totalchampionnat = 0 , totalcoupe = 0;

    // Exécuter la requête SQL
    if (query.exec("SELECT TYPE_M, COUNT(*) FROM MATCH GROUP BY TYPE_M")) {
        while (query.next()) {
            QString type_m = query.value(0).toString().trimmed().toLower();
            int count = query.value(1).toInt();

            qDebug() << "TYPE_M reçu de la base :" << type_m << " - Count :" << count;

            if (type_m == "amical") {
                totalamical = count;
            } else if (type_m == "championnat") {
                totalchampionnat = count;
            } else if (type_m == "coupe") {
                totalcoupe = count;
            }
        }

        qDebug() << "Totaux:";
        qDebug() << "Amical:" << totalamical;
        qDebug() << "Championnat:" << totalchampionnat;
        qDebug() << "Coupe:" << totalcoupe;
    } else {
        qDebug() << "Erreur SQL:" << query.lastError().text();
        return;
    }


    // Vérifier que des données ont été récupérées
    if (totalamical == 0 && totalchampionnat == 0 && totalcoupe == 0) {
        qDebug() << "Aucune donnée à afficher.";
        return;
    }

    // Calculer la somme totale des matchs
    int totalMatches = totalamical + totalchampionnat + totalcoupe;

    // Créer un Pie Chart
    QPieSeries *series = new QPieSeries();

    // Ajouter des tranches avec les données
    QPieSlice *sliceAmical = series->append("Amical", totalamical);
    QPieSlice *sliceChampionnat = series->append("Championnat", totalchampionnat);
    QPieSlice *sliceCoupe = series->append("Coupe", totalcoupe);

    // Personnalisation des couleurs
    sliceAmical->setBrush(QColor(0, 0, 255));  // Bleu pour Amical
    sliceChampionnat->setBrush(QColor(255, 105, 180)); // Rose pour Championnat
    sliceCoupe->setBrush(QColor(34, 139, 34));  // Vert pour Coupe

    // Ajouter les labels avec les pourcentages en tenant compte du total
    sliceAmical->setLabel(QString("%1: %2 (%3%)")
                              .arg("Amical")
                              .arg(totalamical)
                              .arg(100.0 * totalamical / totalMatches, 0, 'f', 1));

    sliceChampionnat->setLabel(QString("%1: %2 (%3%)")
                                   .arg("Championnat")
                                   .arg(totalchampionnat)
                                   .arg(100.0 * totalchampionnat / totalMatches, 0, 'f', 1));

    sliceCoupe->setLabel(QString("%1: %2 (%3%)")
                             .arg("Coupe")
                             .arg(totalcoupe)
                             .arg(100.0 * totalcoupe / totalMatches, 0, 'f', 1));

    // Exploser les tranches pour les mettre en avant
    sliceAmical->setExploded(true);
    sliceChampionnat->setExploded(true);
    sliceCoupe->setExploded(true);

    // Rendre les labels visibles
    sliceAmical->setLabelVisible(true);
    sliceChampionnat->setLabelVisible(true);
    sliceCoupe->setLabelVisible(true);

    // Créer un graphique
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des types de matchs");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Ajouter une légende et l'aligner en bas
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Créer un QChartView pour afficher le graphique
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(400, 300);

    // Créer une scène et ajouter le QChartView
    QGraphicsScene *scene = new QGraphicsScene();
    scene->addWidget(chartView);

    // Associer la scène à QGraphicsView
    ui->graphicsView->setScene(scene);
}







    void MainWindow::on_tri_clicked()
    {

        QString critere = ui->par->currentText().toLower(); // Ex: "date", "equipe", "stade"
        bool desc = ui->checkDescendant->isChecked();       // Vérifie si l'utilisateur veut un tri descendant

        // Appelle la méthode de tri avec le critère et l'ordre
        QSqlQueryModel* sortedModel = tmpMatch.trier(critere, desc);

        if (sortedModel) {
            ui->tableView->setModel(sortedModel);

            QString ordre = desc ? "décroissant" : "croissant";
            QString message = "Tri effectué par " + critere + " (" + ordre + ").";
            QMessageBox::information(this, tr("Tri"), message);
        } else {
            QMessageBox::critical(this, tr("Erreur"), tr("Échec du tri."));
        }
    }


    void MainWindow::genererCalendrier(const QList<Match>& matchs, QDate mois)
    {
        qDebug() << "Génération du calendrier pour :" << mois.toString("MMMM yyyy");
        qDebug() << "Nombre de matchs à afficher :" << matchs.size();

        // Définir la date du calendrier
        ui->calendarTable->setSelectedDate(mois);
        
        // Stocker les matchs dans une map pour un accès rapide
        QMap<QDate, QList<Match>> matchsParDate;
        for (const Match& m : matchs) {
            QDate dateMatch = m.getDate();
            if (dateMatch.month() == mois.month() && dateMatch.year() == mois.year()) {
                matchsParDate[dateMatch].append(m);
            }
        }

        // Connecter le signal de sélection de date
        connect(ui->calendarTable, &QCalendarWidget::clicked, this, [this, matchsParDate](const QDate& date) {
            QString matchDetails;
            bool hasMatches = false;

            if (matchsParDate.contains(date)) {
                hasMatches = true;
                for (const Match& m : matchsParDate[date]) {
                    matchDetails += QString("Match: %1 vs %2\n").arg(m.getEquipe1(), m.getEquipe2());
                    matchDetails += QString("Lieu: %1\n").arg(m.getLieu());
                    matchDetails += QString("Type: %1\n").arg(m.getType());
                    matchDetails += QString("Etat: %1\n\n").arg(m.getEtat());
                }
            }

            if (!hasMatches) {
                matchDetails = "Aucun match prévu pour cette date.";
            }

            ui->txt_match_details->setText(matchDetails);
        });
    }

    void MainWindow::on_calendarButton_clicked()
    {

        ui->stackedWidget->setCurrentIndex(7);
        QDate moisActuel = QDate::currentDate();
        QList<Match> liste = Match::getAllMatches();
        genererCalendrier(liste, moisActuel);
        updateMoisLabel();
    }

    void MainWindow::updateMoisLabel()
    {
        QString moisAnnee = moisAffiche.toString("MMMM yyyy");
        moisAnnee[0] = moisAnnee[0].toUpper();
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

    void MainWindow::on_btnAujourdhui_clicked()
    {
        moisAffiche = QDate::currentDate();
        genererCalendrier(Match::getAllMatches(), moisAffiche);
        updateMoisLabel();
    }
   /*
        void MainWindow::on_btnAujourdhui_clicked()
    {
        moisAffiche = QDate::currentDate();
        genererCalendrier(Match::getAllMatches(), moisAffiche);
        updateMoisLabel();
    }*/


    /*void MainWindow::on_btnAfficherQR_clicked()
    {
        QModelIndex index = ui->tableView->currentIndex();
        if (!index.isValid()) return;

        int row = index.row();
        QString eq1 = ui->tableView->model()->index(row, 1).data().toString();
        QString eq2 = ui->tableView->model()->index(row, 2).data().toString();
        QDate date = ui->tableView->model()->index(row, 3).data().toDate();
        QString lieu = ui->tableView->model()->index(row, 4).data().toString();
        QString type = ui->tableView->model()->index(row, 5).data().toString();

        Match m(eq1, eq2, date, lieu, type, "prévu","gfhvb","hgcvgv");
        afficherQRCodePourMatch(m);
    }*/






    void MainWindow::on_sms_clicked()
    {
        QString num ="+21697347595";
        QString txt = "uhjbuvb";
        tmpMatch.sendSMS(num,txt);

    }


    void MainWindow::on_micropush_clicked()
    {
        QProcess *process = new QProcess(this);

        // Chemin vers le script Python
        QString scriptPath = "C:/Users/MSI/Downloads/projet/reco_vocale.py";

        // Chemin vers l'exécutable Python
        QString pythonPath = "C:/Users/MSI/AppData/Local/Programs/Python/Python310/python.exe";

        // Lancer le script
        process->start(pythonPath, QStringList() << scriptPath);

        // Attente de fin du script
        if (!process->waitForFinished(8000)) {
            qDebug() << "Erreur lors de l'exécution du script : " << process->errorString();
            return;
        }

        // Lire la sortie du script
        QString output = process->readAllStandardOutput().trimmed();
        qDebug() << "Texte brut reçu : [" << output << "]";

        // Nettoyage du texte
        output.remove("Parlez...");
        output.remove("\r");
        output.remove("\n");
        output = output.trimmed();

        qDebug() << "Texte nettoyé : [" << output << "]";

        // Afficher le texte dans le QLineEdit
        ui->micro->setText(output);

        // Appeler la fonction de recherche avec le texte
        on_recherche_button_clicked(output);


    }
