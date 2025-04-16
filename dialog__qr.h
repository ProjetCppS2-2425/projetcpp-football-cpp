#ifndef DIALOG__QR_H
#define DIALOG__QR_H

#include <QPixmap>
#include <QDialog>
#include "Employes.h"

namespace Ui {
class Dialog__qr;
}

class Dialog__qr : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog__qr(QWidget *parent = nullptr);
    ~Dialog__qr();
    void setEmployeeInfo(const Employes& emp);

private slots:
    void on_closedialog_clicked();

private:
    Ui::Dialog__qr *ui;
    void generateQRCode(const QString& data);
};

#endif // DIALOG__QR_H
