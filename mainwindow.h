#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "Employes.h" // Include the Employes class

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

private:
    int editingRow = -1;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
