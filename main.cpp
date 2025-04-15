#include "mainwindow.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Connection c; // Ensure the class name is correct

    // Verify the createconnect method exists and works as expected
    if (!c.createconnect()) {
        QMessageBox::critical(nullptr, "Database Connection Error", "Failed to connect to the database.");
        return -1;
    }

    MainWindow w;
    w.show();

    int result = a.exec();

    // Removed call to closeconnect as it does not exist
    return result;
}
