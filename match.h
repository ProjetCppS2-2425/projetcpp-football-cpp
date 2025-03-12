#ifndef MATCH_H
#define MATCH_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Match
{
public:
    Match();
    Match(int id_m, const QString &equipe1, const QString &equipe2, const QDate &date,
          const QString &lieu, const QString &type, const QString &etat);

    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int id_m);
    bool modifier(int ,QString , QString , QDate ,
                  QString , QString , QString );
    int nextid();
    int getId() const { return id_m; }
    QString getEquipe1() const { return equipe1; }
    QString getEquipe2() const { return equipe2; }
    QDate getDate() const { return date; }
    QString getLieu() const { return lieu; }
    QString getType() const { return type; }
    QString getEtat() const { return etat; }

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
};

#endif // MATCH_H
