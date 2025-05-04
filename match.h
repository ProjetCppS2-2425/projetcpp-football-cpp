#ifndef MATCH_H
#define MATCH_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QChartView>
#include <QBarSet>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>

class Match
{
public:
    Match();

    Match(int id_m, QString equipe1, QString equipe2, QDate date, QString lieu, QString type, QString etat);
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int id_m);
    bool modifier(int , QString , QString , QDate ,
                  QString , QString , QString  );
    int nextid();
    QSqlQueryModel* chercherAvance(const QString& id, const QString& lieu, const QString& equipe1);

    bool PDF();
    QChartView* stat();
    QSqlQueryModel* trier(const QString& critere, bool desc);
    static QList<Match> getAllMatches();
    bool envoyerSMS() const;
    void sendSMS(const QString& toNumber, const QString& messageText);

    int getId() const { return id_m; }
    QString getEquipe1() const { return equipe1; }
    QString getEquipe2() const { return equipe2; }
    QDate getDate() const { return date; }
    QString getLieu() const { return lieu; }
    QString getType() const { return type; }
    QString getEtat() const { return etat; }
    void envoyerSMSViaService(const QString& numero, const QString& message);


    void setId(int id_m) { this->id_m = id_m; }
    void setEquipe1(const QString &equipe1) { this->equipe1 = equipe1; }
    void setEquipe2(const QString &equipe2) { this->equipe2 = equipe2; }
    void setDate(const QDate &date) { this->date = date; }
    void setLieu(const QString &lieu) { this->lieu = lieu; }
    void setType(const QString &type) { this->type = type; }
    void setEtat(const QString &etat) { this->etat = etat; }

private:
    int id_m;
    QString equipe1;
    QString equipe2;
    QDate date;
    QString lieu;
    QString type;
    QString etat;
    QString num_equipe1;
    QString num_equipe2;
};
Q_DECLARE_METATYPE(Match)

#endif // MATCH_H
