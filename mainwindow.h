#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "billets.h"

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
    void on_ajouter_clicked();
    void on_supprimer_clicked();
    void on_modifier_clicked();
    void on_afficher_clicked();
    bool idExists(int id_m);          // Déclaration de idExists
    void remplirComboBoxMatchs(); // Ajoutez cette ligne
    void ajouterBillet();
private:
    Ui::MainWindow *ui;
    Billets *billets;
};

#endif // MAINWINDOW_H
