#ifndef DIALOG__QR_H
#define DIALOG__QR_H

#include <QPixmap>
#include <QDialog>

namespace Ui {
class Dialog__qr; // Remplacer dialog__qr par Dialog__qr
}

class dialog__qr : public QDialog
{
    Q_OBJECT

public:
    explicit dialog__qr(QWidget *parent = nullptr);
    ~dialog__qr();
    void setQrCode(QPixmap qrCode);

private slots:
    void on_closedialog_clicked();

private:
    Ui::Dialog__qr *ui; // Remplacer dialog__qr par Dialog__qr
};

#endif // DIALOG__QR_H
