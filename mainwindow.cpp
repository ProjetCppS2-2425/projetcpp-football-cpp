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
#include <QtPrintSupport/QPrinter>
#include <QFileDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QLayoutItem>
#include <QLayout>

#include <QSerialPort>
#include <QSerialPortInfo>

QSerialPort *serial;







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
    ui->aff_3->setSortingEnabled(true);
    ui->aff_3->horizontalHeader()->setSortIndicatorShown(true);

    connect(ui->statistics, &QPushButton::clicked, this, [=]() {
        genererStatistiques();
        ui->stackedWidget->setCurrentWidget(ui->tab_3); // aller à la page des stats
    });
setupArduino();

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

void MainWindow::showEntraineurs() {
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
    int ID_EQ = ui->id_3->text().toInt();
    QString NOM_EQUIPE = ui->nom_3->text();
    QString NATIONALITE = ui->nationalite_7->text();
    QString EMAIL = ui->Email_3->text();
    double NOMBRE_DE_JOUEUR = ui->nombre_3->text().toDouble();
    QString RANK = ui->rank_3->currentText();

    // Vérification de l'ID (doit être un entier positif)
    if (ui->id_3->text().isEmpty() || ID_EQ <= 0) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("L'ID doit être un entier positif."), QMessageBox::Cancel);
        return;
    }




    // Vérification de la note (doit être entre 0 et 20)
    if ( NOMBRE_DE_JOUEUR < 0 || NOMBRE_DE_JOUEUR > 20) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("La nombre doit être comprise entre 0 et 20."), QMessageBox::Cancel);
        return;
    }

    // Vérification que les champs ne sont pas vides
    if (EMAIL.isEmpty() || NOM_EQUIPE.isEmpty() || NATIONALITE.isEmpty()) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("Veuillez remplir tous les champs."), QMessageBox::Cancel);
        return;
    }

    // Vérification si l'ID existe déjà
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM EQUIPES WHERE ID_EQ = :ID_EQ");
    checkQuery.bindValue(":ID_EQ",ID_EQ);
    if (checkQuery.exec() && checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("Cet ID existe déjà dans la base de données."), QMessageBox::Cancel);
        return;
    }

    // Création de l'objet et tentative d'ajout
    Equipes eq(ID_EQ, NOM_EQUIPE, NATIONALITE, EMAIL, NOMBRE_DE_JOUEUR, RANK);
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
    int ID_EQ = ui->id_3->text().toInt();
    QString NOM_EQUIPE = ui->nom_3->text();
    QString NATIONALITE = ui->nationalite_7->text();
    QString EMAIL = ui->Email_3->text();
    double NOMBRE_DE_JOUEUR = ui->nombre_3->text().toDouble();
    QString RANK = ui->rank_3->currentText(); // Changed from text() to currentText()

    Equipes eq(ID_EQ, NOM_EQUIPE, NATIONALITE, EMAIL, NOMBRE_DE_JOUEUR, RANK);
    bool test = eq.modifier(ID_EQ); // Changed from ajouter() to modifier()
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
    int ID_EQ = ui->id_3->text().toInt();

    if (ID_EQ == 0) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("Veuillez entrer un ID valide."), QMessageBox::Cancel);
        return;
    }

    Equipes eq;
    bool test = eq.supprimer(ID_EQ);
    if (test) {
        QMessageBox::information(nullptr, QObject::tr("Succès"), QObject::tr("Suppression réussie."), QMessageBox::Ok);
        on_pushButton_afficher_clicked();

        ui->id_3->clear();
        ui->nom_3->clear();
        ui->nationalite_7->clear();
        ui->Email_3->clear();
        ui->nombre_3->clear();
        ui->rank_3->setCurrentIndex(0);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Échec"), QObject::tr("Erreur lors de la suppression."), QMessageBox::Cancel);
    }
}

void MainWindow::on_aff_3_cellClicked(int row, int column)
{
    Q_UNUSED(column); // Add this to avoid unused parameter warning

    // Récupérer les valeurs de la ligne sélectionnée
    QString ID_EQ = ui->aff_3->item(row, 0)->text();
    QString NOM_DE_EQUIPE = ui->aff_3->item(row, 1)->text();
    QString NATIONALITE = ui->aff_3->item(row, 2)->text();
    QString EMAIL = ui->aff_3->item(row, 3)->text();
    QString NOMBRE_DE_JOUEUR = ui->aff_3->item(row, 4)->text();
    QString RANK = ui->aff_3->item(row, 5)->text();

    // Remplir les QLineEdit avec les valeurs sélectionnées
    ui->id_3->setText(ID_EQ);
    ui->nom_3->setText(NOM_DE_EQUIPE);
    ui->nationalite_7->setText(NATIONALITE);
    ui->Email_3->setText(EMAIL);
    ui->nombre_3->setText(NOMBRE_DE_JOUEUR);
    ui->rank_3->setCurrentText(RANK); // Changed from setText() to setCurrentText()
}
void MainWindow::on_recherche_3_clicked()
{
    QString critere = ui->lineedit_recherche->text();
    if (critere.isEmpty()) {
        return;
    }

    Equipes eq;
    QSqlQueryModel *model = eq.rechercher(critere);

    // Nettoyer le QTableWidget
    ui->aff_3->clear();
    ui->aff_3->setRowCount(model->rowCount());
    ui->aff_3->setColumnCount(model->columnCount());

    // Définir les en-têtes
    for (int col = 0; col < model->columnCount(); ++col) {
        ui->aff_3->setHorizontalHeaderItem(col, new QTableWidgetItem(model->headerData(col, Qt::Horizontal).toString()));
    }

    // Remplir la table
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QString value = model->data(model->index(row, col)).toString();
            ui->aff_3->setItem(row, col, new QTableWidgetItem(value));
        }
    }

    delete model; // bonne pratique pour libérer la mémoire si tu ne réutilises pas le modèle
}


void MainWindow::on_tri_3_clicked()
{
    // Vérifier si le comboBox contient des éléments
    if (ui->comboBox_77->count() == 0) {
        qDebug() << "ComboBox vide, initialisation...";
        ui->comboBox_77->addItem("ID croissant", "ID_EQ ASC");
        ui->comboBox_77->addItem("ID décroissant", "ID_EQ DESC");
        ui->comboBox_77->addItem("nombre croissant", "NOMBRE_DE_JOUEUR ASC");
        ui->comboBox_77->addItem("nombre décroissant", "NOMBRE_DE_JOUEUR DESC");

        // Sélectionner le premier élément par défaut
        ui->comboBox_77->setCurrentIndex(0);
    }

    int index = ui->comboBox_77->currentIndex();
    qDebug() << "Index sélectionné:" << index;

    if (index < 0) {
        qDebug() << "Aucun élément sélectionné dans le comboBox";
        QMessageBox::warning(this, "Tri", "Veuillez sélectionner un critère de tri");
        return;
    }

    QVariant itemData = ui->comboBox_77->itemData(index);
    qDebug() << "Item data:" << itemData;

    if (!itemData.isValid()) {
        qDebug() << "Les données de l'élément ne sont pas valides";
        QMessageBox::warning(this, "Tri", "Erreur: données de tri invalides");
        return;
    }

    QString triComplet = itemData.toString();
    qDebug() << "Tri complet:" << triComplet;

    if (triComplet.isEmpty()) {
        qDebug() << "La chaîne de tri est vide";
        QMessageBox::warning(this, "Tri", "Erreur: critère de tri vide");
        return;
    }

    // Extraire la colonne et l'ordre depuis la string : "ID_EQ ASC"
    QStringList parts = triComplet.split(" ");
    qDebug() << "Parts:" << parts;

    if (parts.size() != 2) {
        qDebug() << "Format de tri incorrect";
        QMessageBox::warning(this, "Tri", "Erreur: format de tri incorrect");
        return;
    }

    QString critere = parts[0];
    QString orderStr = parts[1];
    qDebug() << "Critère:" << critere << "Ordre:" << orderStr;

    Qt::SortOrder order = (orderStr.toUpper() == "DESC") ? Qt::DescendingOrder : Qt::AscendingOrder;

    try {
        Equipes eq;
        QSqlQueryModel *model = eq.trier(critere, order);

        if (!model) {
            qDebug() << "Le modèle retourné est NULL";
            QMessageBox::warning(this, "Tri", "Erreur: échec de récupération des données triées");
            return;
        }

        qDebug() << "Nombre de lignes dans le modèle:" << model->rowCount();
        qDebug() << "Nombre de colonnes dans le modèle:" << model->columnCount();

        // Vider le tableau
        ui->aff_3->clear();
        ui->aff_3->setRowCount(model->rowCount());
        ui->aff_3->setColumnCount(model->columnCount());

        // En-têtes
        for (int col = 0; col < model->columnCount(); ++col) {
            QString header = model->headerData(col, Qt::Horizontal).toString();
            ui->aff_3->setHorizontalHeaderItem(col, new QTableWidgetItem(header));
        }

        // Remplir les données
        for (int row = 0; row < model->rowCount(); ++row) {
            for (int col = 0; col < model->columnCount(); ++col) {
                QString value = model->data(model->index(row, col)).toString();
                ui->aff_3->setItem(row, col, new QTableWidgetItem(value));
            }
        }

        delete model;
        qDebug() << "Tri terminé avec succès";
    } catch (const std::exception& e) {
        qDebug() << "Exception lors du tri:" << e.what();
        QMessageBox::warning(this, "Tri", QString("Erreur lors du tri: %1").arg(e.what()));
    } catch (...) {
        qDebug() << "Exception inconnue lors du tri";
        QMessageBox::warning(this, "Tri", "Erreur inconnue lors du tri");
    }
}


void MainWindow::on_export_3_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Exporter en PDF"), "", tr("Fichiers PDF (*.pdf)"));

    if (fileName.isEmpty())
        return;

    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive))
        fileName += ".pdf";

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));

    QPainter painter;
    if (!painter.begin(&printer)) {
        QMessageBox::warning(this, tr("Erreur"), tr("Impossible d'ouvrir le fichier PDF pour écriture."));
        return;
    }

    // Configuration police
    QFont font = painter.font();
    font.setPointSize(10);
    painter.setFont(font);
    QFontMetrics metrics(font);

    // Dimensions des cellules
    const int cellWidth = 120;
    const int cellHeight = 30;
    int x = 10;
    int y = 10;

    const int rowCount = ui->aff_3->rowCount();
    const int colCount = ui->aff_3->columnCount();

    // Titre
    QFont titleFont = font;
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(x, y + 20, "Tableau des équipes");
    painter.setFont(font);
    y += 40;

    // En-têtes de colonnes
    for (int col = 0; col < colCount; ++col) {
        QString headerText = ui->aff_3->horizontalHeaderItem(col)->text();
        QRect rect(x + col * cellWidth, y, cellWidth, cellHeight);
        painter.drawRect(rect);
        painter.drawText(rect, Qt::AlignCenter, headerText);
    }

    y += cellHeight;

    // Lignes de données
    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < colCount; ++col) {
            QTableWidgetItem* item = ui->aff_3->item(row, col);
            QString text = item ? item->text() : "";
            QRect rect(x + col * cellWidth, y + row * cellHeight, cellWidth, cellHeight);
            painter.drawRect(rect);
            painter.drawText(rect, Qt::AlignCenter, text);
        }
    }

    painter.end();

    QMessageBox::information(this, tr("Exportation terminée"), tr("Le tableau a été exporté avec succès en PDF."));
}

void MainWindow::genererStatistiques() {
    QPieSeries *series = new QPieSeries();

    QSqlQuery query;
    // Récupérer seulement les nationalités ciblées
    if (!query.exec("SELECT NATIONALITE, SUM(NOMBRE_DE_JOUEUR) "
                    "FROM EQUIPES "
                    "WHERE NATIONALITE IN ('eng', 'esp', 'ita', 'neth', 'fra', 'ger') "
                    "GROUP BY NATIONALITE")) {
        QMessageBox::warning(this, "Erreur SQL", "Impossible de charger les statistiques par nationalité.");
        return;
    }

    int totalJoueurs = 0;
    QList<QPair<QString, int>> data;
    while (query.next()) {
        QString nationalite = query.value(0).toString();
        int nbJoueurs = query.value(1).toInt();
        data.append(qMakePair(nationalite, nbJoueurs));
        totalJoueurs += nbJoueurs;
    }

    for (const auto &entry : data) {
        QString nationalite = entry.first;
        int nbJoueurs = entry.second;

        double pourcentage = (totalJoueurs == 0) ? 0 : (nbJoueurs * 100.0 / totalJoueurs);

        QPieSlice *slice = new QPieSlice(QString("%1 (%2%)")
                                             .arg(nationalite.toUpper())
                                             .arg(QString::number(pourcentage, 'f', 1)),
                                         nbJoueurs);
        slice->setLabelVisible(true);

        // 🔴 Couleur conditionnelle
        if (pourcentage < 4.0) {
            slice->setBrush(QColor("#e74c3c"));  // Rouge
        } else {
            slice->setBrush(QColor("#2ecc71"));  // Vert
        }

        series->append(slice);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des joueurs par nationalité");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QWidget *pageStats = ui->tab_3;
    QLayout *oldLayout = pageStats->layout();
    if (oldLayout) {
        QLayoutItem *item = nullptr;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            if (item->widget()) delete item->widget();
            delete item;
        }
        delete oldLayout;
    }

    QVBoxLayout *layout = new QVBoxLayout(pageStats);
    layout->addWidget(chartView);
    pageStats->setLayout(layout);
}

void MainWindow::setupArduino() {
    qDebug() << "Ports série disponibles :";
    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) {
        qDebug() << "  PortName:" << info.portName()
        << "  VendorID:" << info.vendorIdentifier()
        << "  ProductID:"<< info.productIdentifier();
    }
    int ret = arduino.connect_arduino();
    if (ret != 0) {
        QMessageBox::warning(this, "Arduino",
                             "Impossible de se connecter à l'Arduino");

        return;
    }


    // Dès qu’on reçoit une trame, appeler onSerialData()
    connect(arduino.getserial(), &QSerialPort::readyRead,
            this, &MainWindow::onSerialData);

}

void MainWindow::onSerialData()
{
    static QByteArray buffer;
    static QStringList ids;

    buffer += arduino.read_from_arduino();

    while (buffer.contains('\n')) {
        int endIndex = buffer.indexOf('\n');
        QByteArray line = buffer.left(endIndex).trimmed();
        buffer.remove(0, endIndex + 1);

        if (!line.isEmpty()) {
            QString id = QString::fromUtf8(line);
            qDebug() << "ID reçu :" << id;

            ids << id;

            if (ids.size() == 2) {
                QString nom1 = "Inconnu";
                QString nom2 = "Inconnu";

                QSqlQuery query;
                query.prepare("SELECT nom_equipe FROM equipe WHERE id = :id");

                query.bindValue(":id", ids[0]);
                if (query.exec() && query.next())
                    nom1 = query.value(0).toString();

                query.prepare("SELECT nom_equipe FROM equipe WHERE id = :id");
                query.bindValue(":id", ids[1]);
                if (query.exec() && query.next())
                    nom2 = query.value(0).toString();

                QString toSend = "NAMES:" + nom1 + ":" + nom2 + "\n";
                qDebug() << "Envoi vers Arduino :" << toSend.trimmed();
                arduino.write_to_arduino(toSend.toUtf8());

                ids.clear();
            }
        }
    }
}



