#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QDate>
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

    void on_bt_tri_clicked();
    void on_pdf_clicked();
    void on_table_Billets_clicked(const QModelIndex &index);

    void on_chercher_textChanged(const QString &arg1);

    void on_ajouter_clicked();

    void on_modifier_clicked();

    void on_supprimer_clicked();


private:
    Ui::MainWindow *ui;
    QSqlTableModel *model;
    void clearInputs();
    void stat();
    billets b;
};

#endif // MAINWINDOW_H
