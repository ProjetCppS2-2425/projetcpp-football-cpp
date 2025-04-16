#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QMessageBox>
#include <QTableWidget>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "Employes.h"
#include "Smtp.h"     // Pour l'envoi de mail
#include "dialog__qr.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void changePage(int index);
    void on_Valider_clicked();
    void modifyRow(int row);
    void deleteRow(int row);
    void on_pushButton_6_clicked();  // PDF export function

    // 📧 Slots pour mailing
    void sendMail();
    void mailSent(QString status);
    void on_mailing_clicked();
    void on_pushButton_11_clicked();
    void on_pushButton_12_clicked();
    void on_pushButton_4_clicked();
    void onTableItemClicked(QTableWidgetItem *item);
    void loadAllEmployees();
    void on_pushButton_Refresh_clicked();

private:
    int editingRow = -1;
    Ui::MainWindow *ui;
    Employes employe; // Ajout de l'instance utilisée pour récupérer les emails
};

#endif // MAINWINDOW_H
