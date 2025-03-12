#include "matchwidget.h"
#include "matchwidget.h"
#include "connection.h"
#include <QMessageBox>
#include <QDebug>

MatchWidget::MatchWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MatchWidget)
{
    ui->setupUi(this);
    Connection::getInstance()->createConnection();
    refreshTable();

    // Populate equipe1 and equipe2 ComboBoxes with some example teams
    QStringList teams = {"PSG", "Marseille", "Lyon", "Monaco", "Lille", "Bordeaux"};
    ui->equipe1->addItems(teams);
    ui->equipe2->addItems(teams);
}

MatchWidget::~MatchWidget()
{
    delete ui;
}

void MatchWidget::on_ajouterMatch_clicked()
{
    QString equipe1 = ui->equipe1->currentText();
    QString equipe2 = ui->equipe2->currentText();
    QDate date = ui->dateMatch->date();
    QTime heure = ui->heureMatch->time();
    QString stade = ui->stade->text();

    if (equipe1.isEmpty() || equipe2.isEmpty() || stade.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs.");
        return;
    }

    if (equipe1 == equipe2) {
        QMessageBox::warning(this, "Erreur", "Les deux équipes ne peuvent pas être identiques.");
        return;
    }

    Match m(0, equipe1, equipe2, date, heure, stade, "");
    bool success = m.ajouter();

    if (success) {
        QMessageBox::information(this, "Succès", "Le match a été ajouté avec succès.");
        refreshTable();
        clearInputs();
    } else {
        QMessageBox::critical(this, "Erreur", "Une erreur est survenue lors de l'ajout du match.");
    }
}

void MatchWidget::on_tableWidget_cellClicked(int row, int column)
{
    if (column == 7) { // Modifier
        int id = ui->tableWidget->item(row, 0)->text().toInt();
        QString equipe1 = ui->tableWidget->item(row, 1)->text();
        QString equipe2 = ui->tableWidget->item(row, 2)->text();
        QDate date = QDate::fromString(ui->tableWidget->item(row, 3)->text(), "yyyy-MM-dd");
        QTime heure = QTime::fromString(ui->tableWidget->item(row, 4)->text(), "hh:mm:ss");
        QString stade = ui->tableWidget->item(row, 5)->text();
        QString score = ui->tableWidget->item(row, 6)->text();

        Match m(id, equipe1, equipe2, date, heure, stade, score);

        // Here you would typically open a dialog to edit the match details
        // For simplicity, we'll just update the score
        bool ok;
        QString newScore = QInputDialog::getText(this, "Modifier le score", "Nouveau score:", QLineEdit::Normal, score, &ok);
        if (ok && !newScore.isEmpty()) {
            m.setScore(newScore);
            if (m.modifier()) {
                QMessageBox::information(this, "Succès", "Le match a été modifié avec succès.");
                refreshTable();
            } else {
                QMessageBox::critical(this, "Erreur", "Une erreur est survenue lors de la modification du match.");
            }
        }
    } else if (column == 8) { // Supprimer
        int id = ui->tableWidget->item(row, 0)->text().toInt();
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation", "Êtes-vous sûr de vouloir supprimer ce match ?",
                                                                  QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            if (tmpMatch.supprimer(id)) {
                QMessageBox::information(this, "Succès", "Le match a été supprimé avec succès.");
                refreshTable();
            } else {
                QMessageBox::critical(this, "Erreur", "Une erreur est survenue lors de la suppression du match.");
            }
        }
    }
}

void MatchWidget::on_recherche_textChanged(const QString &arg1)
{
    QSqlQueryModel *model = tmpMatch.rechercher(arg1);
    ui->tableWidget->setModel(model);
}

void MatchWidget::on_tri_currentIndexChanged(const QString &arg1)
{
    QSqlQueryModel *model = tmpMatch.trier(arg1);
    ui->tableWidget->setModel(model);
}

void MatchWidget::refreshTable()
{
    QSqlQueryModel *model = tmpMatch.afficher();
    ui->tableWidget->setModel(model);
}

void MatchWidget::clearInputs()
{
    ui->equipe1->setCurrentIndex(0);
    ui->equipe2->setCurrentIndex(0);
    ui->dateMatch->setDate(QDate::currentDate());
    ui->heureMatch->setTime(QTime::currentTime());
    ui->stade->clear();
}

