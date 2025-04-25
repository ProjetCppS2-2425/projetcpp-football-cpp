#include "match.h"
#include <QDebug>
#include <QPainter>
#include <QPdfWriter>
#include <QSqlError>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
Match::Match()
{
}


Match::Match(int id_m, QString equipe1, QString equipe2, QDate date, QString lieu, QString type, QString etat)
{
    this->id_m = id_m;
    this->equipe1 = equipe1;
    this->equipe2 = equipe2;
    this->date = date;
    this->lieu = lieu;
    this->type = type;
    this->etat = etat;
}


bool Match::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO match (id_m, equipe1, equipe2, date_M, lieu, type_M, etat) "
                  "VALUES (:id_m, :equipe1, :equipe2, :date_M, :lieu, :type_M, :etat)");
    query.bindValue(":id_m", id_m);
    query.bindValue(":equipe1", equipe1);
    query.bindValue(":equipe2", equipe2);
    query.bindValue(":date_M", date);
    query.bindValue(":lieu", lieu);
    query.bindValue(":type_M", type);
    query.bindValue(":etat", etat);


    return query.exec();

}
int Match ::nextid() {
    QSqlQuery query;
    query.prepare("SELECT MAX(id_m) FROM match");

    if (query.exec() && query.next()) {
        int idmax = query.value(0).toInt();
        return idmax + 1;
    }
    return 1;
}


QSqlQueryModel* Match::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM match");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_M"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Equipe1"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Equipe2"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date_M"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Lieu"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Type_M"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("eq1_but"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("eq2_but"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Etat"));



    return model;
}

bool Match::supprimer(int id_m)
{
    QSqlQuery query;
    query.prepare("DELETE FROM match WHERE id_m = :id_m");
    query.bindValue(":id_m", id_m);
    return query.exec();
}

bool Match::modifier(int id_m, QString equipe1, QString equipe2, QDate date, 
                    QString lieu, QString type, QString etat)
{
    QSqlQuery query;
    query.prepare("UPDATE MATCHS SET EQUIPE1=:equipe1, EQUIPE2=:equipe2, DATE_M=:date, "
                 "LIEU=:lieu, TYPE_M=:type, ETAT=:etat "
                 "WHERE ID_M=:id_m");

    query.bindValue(":id_m", id_m);
    query.bindValue(":equipe1", equipe1);
    query.bindValue(":equipe2", equipe2);
    query.bindValue(":date", date);
    query.bindValue(":lieu", lieu);
    query.bindValue(":type", type);
    query.bindValue(":etat", etat);


   /* bool success = query.exec();
    if (success) {
        this->num_equipe1 = num1;
        this->num_equipe2 = num2;  // Envoyer SMS après modification réussie
    }
    return success;*/
}
QSqlQueryModel* Match::chercherAvance(const QString& id, const QString& lieu, const QString& equipe1)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    QString queryString = "SELECT * FROM MATCH WHERE 1=1"; // permet d'ajouter des conditions facilement

    if (!id.isEmpty())
        queryString += " AND ID_M = :id";
    if (!lieu.isEmpty())
        queryString += " AND LIEU LIKE :lieu";
    if (!equipe1.isEmpty())
        queryString += " AND EQUIPE1 LIKE :equipe1";

    query.prepare(queryString);

    if (!id.isEmpty())
        query.bindValue(":id", id.toInt());
    if (!lieu.isEmpty())
        query.bindValue(":lieu", "%" + lieu + "%");
    if (!equipe1.isEmpty())
        query.bindValue(":equipe1", "%" + equipe1 + "%");

    if (query.exec()) {
        model->setQuery(std::move(query));

        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_M"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("EQUIPE1"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("EQUIPE2"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("DATE_M"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("LIEU"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("TYPE_M"));
        model->setHeaderData(6, Qt::Horizontal, QObject::tr("EQ1_BUT"));
        model->setHeaderData(7, Qt::Horizontal, QObject::tr("EQ2_BUT"));
        model->setHeaderData(8, Qt::Horizontal, QObject::tr("ETAT"));
        model->setHeaderData(9, Qt::Horizontal, QObject::tr("ID_E"));
    } else {
        qDebug() << "Erreur SQL : " << query.lastError().text();
        delete model;
        return nullptr;
    }

    return model;
}

bool Match::PDF() {
    // Step 1: Execute SQL query to retrieve data
    QSqlQuery query("SELECT * FROM MATCH");


    // Step 2: Create the PDF file
    QString filePath = "C:/Users/MSI/Desktop/pdf/exporter.pdf"; // Set your desired path
    QPdfWriter writer(filePath);

    writer.setPageSize(QPageSize(QPageSize::A4));

    QPainter painter(&writer);

    // Check if painter is initialized
    if (!painter.isActive()) {
        qDebug() << "Failed to initialize QPainter";
        return false;
    }

    // Set font for the PDF title
    painter.setFont(QFont("Arial", 16));
    painter.drawText(200, 50, "Liste des clients");

    int y = 500;  // Initial position for drawing data

    // Step 3: Draw headers in the PDF
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.drawText(50, y, "id_m");
    painter.drawText(650+1000, y, "equipe1");
    painter.drawText(1550+1000, y, "equipe2");
    painter.drawText(2350+1000, y, "date_m");
    painter.drawText(3300+1000, y, "lieu");
    painter.drawText(3700+1000, y, "type");
    painter.drawText(4500+1000, y, "eq1_but");
    painter.drawText(5000+1000, y, "eq2_but");
    painter.drawText(5400+1000, y, "id_e");



    y += 200;  // Move down for next data row

    // Step 4: Draw the data in the PDF
    painter.setFont(QFont("Arial", 6));
    while (query.next()) {
        painter.drawText(50, y, query.value(0).toString());
        painter.drawText(650+1000, y, query.value(1).toString());
        painter.drawText(1550+1000, y, query.value(2).toString());
        painter.drawText(2350+1000, y, query.value(3).toString());
        painter.drawText(3300+1000, y, query.value(4).toString());
        painter.drawText(3700+1000, y, query.value(5).toString());
        painter.drawText(4500+1000, y, query.value(6).toString());
        painter.drawText(5000+1000, y, query.value(7).toString());
        painter.drawText(6000+1000, y, query.value(8).toString());


        y += 200;  // Move down for next row

        if (y > writer.height() - 50) {
            writer.newPage();
            y = 100;
        }
    }

    painter.end();  // End the PDF creation process
    qDebug() << "PDF export complete! The file is saved at: " << filePath;

    return true;  // Return true if PDF is successfully created
}

QChartView* Match::stat() {
    // Création des ensembles de données
    QBarSet* setAmical = new QBarSet("Amical");
    QBarSet* setChamp = new QBarSet("Championnat");
    QBarSet* setCoupe = new QBarSet("Coupe");

    int amicalCount = 0, champCount = 0, coupeCount = 0;

    // Requête SQL pour récupérer le type de match
    QSqlQuery query;
    query.exec("SELECT type_m FROM Match");

    while (query.next()) {
        QString type = query.value(0).toString();
        // Comparaison en minuscules pour éviter les problèmes de casse
        QString lowerType = type.toLower();
        if (lowerType == "amical") {
            ++amicalCount;
        } else if (lowerType == "championnat") {
            ++champCount;
        } else if (lowerType == "coupe") {
            ++coupeCount;
        }
    }

    // Ajouter les données aux ensembles
    *setAmical << amicalCount;
    *setChamp << champCount;
    *setCoupe << coupeCount;

    // Création de la série de barres
    QBarSeries* series = new QBarSeries();
    series->append(setAmical);
    series->append(setChamp);
    series->append(setCoupe);

    // Création du graphique
    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statistiques des matchs par type"); // Titre mis à jour
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Ajout des axes
    QStringList categories;
    categories << "Matchs";
    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis* axisY = new QValueAxis();
    int maxCount = qMax(amicalCount, qMax(champCount, coupeCount));
    axisY->setRange(0, qMax(1, maxCount + 1)); // Plage minimale garantie
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Affichage de la légende
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Création de la vue du graphique
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;
}

QSqlQueryModel* Match::trier(const QString& critere, bool desc)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QString ordre = desc ? "DESC" : "ASC";

    QString query = "SELECT * FROM match ORDER BY " + critere + " " + ordre;
    model->setQuery(query);

    if (model->lastError().isValid()) {
        qDebug() << "Erreur SQL: " << model->lastError().text();
        delete model;
        return nullptr;
    }

    return model;
}


QList<Match> Match::getAllMatches()
{
    QList<Match> list;
    QSqlQuery query("SELECT equipe1, equipe2, date, lieu FROM match");

    while (query.next()) {
        Match m;
        m.setEquipe1(query.value(0).toString());
        m.setEquipe2(query.value(1).toString());
        m.setDate(query.value(2).toDate());
        m.setLieu(query.value(3).toString());
        list.append(m);
    }
    return list;
}


