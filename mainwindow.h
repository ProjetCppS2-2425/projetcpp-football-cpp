#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QMessageBox>
#include "employes.h"
#include "smtp.h"     // Pour l'envoi de mail


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

    // 📧 Slots pour mailing
    void sendMail();
    void mailSent(QString status);
    void on_mailing_clicked();

private:
    int editingRow = -1;
    Ui::MainWindow *ui;

    Employes employe; // Ajout de l'instance utilisée pour récupérer les emails
};


#endif // MAINWINDOW_H
