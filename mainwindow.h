#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "match.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_supprimer_clicked();
    void on_pushButton_modifier_clicked();
    void on_tableView_clicked(const QModelIndex &index);

    void on_ajouterMatch_clicked();


    void on_recherche_button_clicked();

    void on_exporter_clicked();

    void on_stat_clicked();


    void on_tri_clicked();

    void genererCalendrier(const QList<Match>& matchs, QDate mois);

    void on_calendarButton_clicked();
    void updateMoisLabel();
    void on_btnMoisSuivant_clicked();

    void on_btnMoisPrecedent_clicked();

    void on_btnAnneeSuivante_clicked();

    void on_btnAnneePrecedente_clicked();

private:
    Ui::MainWindow *ui;
    Match tmpMatch;
    void refreshTable();
    void clearInputFields();
    QDate moisAffiche;
};
#endif // MAINWINDOW_H
