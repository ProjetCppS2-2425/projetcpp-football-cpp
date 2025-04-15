#include "dialog__qr.h"
#include "ui_dialog__qr.h"

dialog__qr::dialog__qr(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog__qr) // Remplacer dialog__qr par Dialog__qr
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint & ~Qt::WindowContextHelpButtonHint);
}

dialog__qr::~dialog__qr()
{
    delete ui;
}

void dialog__qr::setQrCode(QPixmap qrCode)
{
    ui->qr_code_output->setPixmap(qrCode);
}

void dialog__qr::on_closedialog_clicked()
{
    close();
}
