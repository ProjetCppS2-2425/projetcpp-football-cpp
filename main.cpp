#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"
#include <QMetaType>
#include "match.h"
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);


    MainWindow w;

    //w.show();
    //nouh meznd

    Connection c;


    bool test=c.createconnect();
    if(test)
    {w.show();
        QMessageBox::information(nullptr, QObject::tr("database is open"),
                    QObject::tr("connection successful.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);

}
    else
        QMessageBox::critical(nullptr, QObject::tr("database is not open"),
                    QObject::tr("connection failed.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);



    return a.exec();
qApp->setStyleSheet(R"(
    QMessageBox {
        background-color: white;
    }
    QMessageBox QLabel {
        color: black;
        font-size: 14px;
    }
    QMessageBox QPushButton {
        background-color: #e45638;
        color: white;
        padding: 5px;
        border-radius: 5px;
    }
)");

}

