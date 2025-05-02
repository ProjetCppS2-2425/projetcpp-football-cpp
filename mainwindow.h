#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QMap>
#include <QPixmap>
#include <QIcon>
#include "arduino.h"


namespace Ui {
class MainWindow;
}



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void showEquipes();
    void showMatches();
    void showEmployes();
    void showBillets();
    void showSponsors();
    void showEntraineurs();



    void on_pushButton_afficher_clicked();
    void on_Valider_3_clicked();
    void on_modifier_3_clicked();
    void on_supprimer_3_clicked();
    void on_export_3_clicked();
    void on_recherche_3_clicked();
    void on_tri_3_clicked();
    void on_aff_3_cellClicked(int row, int column);
    void genererStatistiques();
    void setupArduino();
    void onSerialData();




private:
    Ui::MainWindow *ui;
    QMap<QPushButton*, int> pageMap;
    Arduino arduino;
};

#endif // MAINWINDOW_H
