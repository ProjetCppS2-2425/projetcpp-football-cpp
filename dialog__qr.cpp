#include "dialog__qr.h"
#include "ui_dialog__qr.h"
#include "qr_code.hpp"
#include <QPainter>
#include <QDebug>

Dialog__qr::Dialog__qr(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog__qr)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint & ~Qt::WindowContextHelpButtonHint);
}

Dialog__qr::~Dialog__qr()
{
    delete ui;
}

void Dialog__qr::generateQRCode(const QString& data)
{
    try {
        // Convert QString to std::string for QR code generation
        std::string text = data.toStdString();

        // Generate QR code
        qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(text.c_str(), qrcodegen::QrCode::Ecc::MEDIUM);

        // Create a QPixmap to draw the QR code
        int size = qr.getSize();
        QPixmap pixmap(size * 4, size * 4);
        pixmap.fill(Qt::white);

        QPainter painter(&pixmap);
        painter.setPen(Qt::black);
        painter.setBrush(Qt::black);

        // Draw the QR code
        for (int y = 0; y < size; y++) {
            for (int x = 0; x < size; x++) {
                if (qr.getModule(x, y)) {
                    painter.drawRect(x * 4, y * 4, 4, 4);
                }
            }
        }

        // Set the QR code to the label
        ui->qr_code_output->setPixmap(pixmap.scaled(461, 411, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } catch (const std::exception& e) {
        qDebug() << "Error generating QR code:" << e.what();
    }
}

void Dialog__qr::setEmployeeInfo(const Employes& emp)
{
    // Create a string with employee information
    QString employeeData = QString("Employee Information:\n"
                                   "ID: %1\n"
                                   "Name: %2 %3\n"
                                   "Position: %4\n"
                                   "Email: %5\n"
                                   "Hire Date: %6\n"
                                   "Salary: %7")
                               .arg(emp.getIdM())
                               .arg(emp.getNom())
                               .arg(emp.getPrenom())
                               .arg(emp.getPoste())
                               .arg(emp.getEmail())
                               .arg(emp.getDateEmbauche())
                               .arg(emp.getSalaire());

    // Generate and display QR code
    generateQRCode(employeeData);
}

void Dialog__qr::on_closedialog_clicked()
{
    close();
}
