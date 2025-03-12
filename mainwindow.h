#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qpushbutton.h"
#include <QMainWindow>
#include <QMap>
#include <QPixmap>
#include <QIcon>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


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
    void showEntreneurs();



    void on_pushButton_afficher_clicked();
    void on_Valider_3_clicked();
    void on_modifier_3_clicked();
    void on_supprimer_3_clicked();
    void on_aff_3_cellClicked(int row, int column);


private:
    Ui::MainWindow *ui;
    QMap<QPushButton*, int> pageMap;
};

#endif // MAINWINDOW_H
