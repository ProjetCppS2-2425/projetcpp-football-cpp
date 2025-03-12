/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QFormLayout *formLayout;
    QLabel *label_2;
    QLineEdit *lineEdit_equipe1;
    QLabel *label_3;
    QLineEdit *lineEdit_equipe2;
    QLabel *label_4;
    QDateEdit *dateEdit_date;
    QLabel *label_5;
    QLineEdit *lineEdit_lieu;
    QLabel *label_6;
    QComboBox *comboBox_type;
    QLabel *label_7;
    QComboBox *comboBox_etat;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_ajouter;
    QPushButton *pushButton_modifier;
    QPushButton *pushButton_supprimer;
    QLabel *label;
    QLineEdit *lineEdit_id;
    QTableView *tableView;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        formLayout = new QFormLayout(groupBox);
        formLayout->setObjectName("formLayout");
        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        lineEdit_equipe1 = new QLineEdit(groupBox);
        lineEdit_equipe1->setObjectName("lineEdit_equipe1");

        formLayout->setWidget(1, QFormLayout::FieldRole, lineEdit_equipe1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName("label_3");

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        lineEdit_equipe2 = new QLineEdit(groupBox);
        lineEdit_equipe2->setObjectName("lineEdit_equipe2");

        formLayout->setWidget(2, QFormLayout::FieldRole, lineEdit_equipe2);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName("label_4");

        formLayout->setWidget(3, QFormLayout::LabelRole, label_4);

        dateEdit_date = new QDateEdit(groupBox);
        dateEdit_date->setObjectName("dateEdit_date");

        formLayout->setWidget(3, QFormLayout::FieldRole, dateEdit_date);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName("label_5");

        formLayout->setWidget(4, QFormLayout::LabelRole, label_5);

        lineEdit_lieu = new QLineEdit(groupBox);
        lineEdit_lieu->setObjectName("lineEdit_lieu");

        formLayout->setWidget(4, QFormLayout::FieldRole, lineEdit_lieu);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName("label_6");

        formLayout->setWidget(5, QFormLayout::LabelRole, label_6);

        comboBox_type = new QComboBox(groupBox);
        comboBox_type->addItem(QString());
        comboBox_type->addItem(QString());
        comboBox_type->addItem(QString());
        comboBox_type->setObjectName("comboBox_type");

        formLayout->setWidget(5, QFormLayout::FieldRole, comboBox_type);

        label_7 = new QLabel(groupBox);
        label_7->setObjectName("label_7");

        formLayout->setWidget(6, QFormLayout::LabelRole, label_7);

        comboBox_etat = new QComboBox(groupBox);
        comboBox_etat->addItem(QString());
        comboBox_etat->addItem(QString());
        comboBox_etat->addItem(QString());
        comboBox_etat->addItem(QString());
        comboBox_etat->setObjectName("comboBox_etat");

        formLayout->setWidget(6, QFormLayout::FieldRole, comboBox_etat);


        verticalLayout->addWidget(groupBox);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        pushButton_ajouter = new QPushButton(centralwidget);
        pushButton_ajouter->setObjectName("pushButton_ajouter");

        horizontalLayout->addWidget(pushButton_ajouter);

        pushButton_modifier = new QPushButton(centralwidget);
        pushButton_modifier->setObjectName("pushButton_modifier");

        horizontalLayout->addWidget(pushButton_modifier);

        pushButton_supprimer = new QPushButton(centralwidget);
        pushButton_supprimer->setObjectName("pushButton_supprimer");

        horizontalLayout->addWidget(pushButton_supprimer);


        verticalLayout->addLayout(horizontalLayout);

        label = new QLabel(centralwidget);
        label->setObjectName("label");

        verticalLayout->addWidget(label);

        lineEdit_id = new QLineEdit(centralwidget);
        lineEdit_id->setObjectName("lineEdit_id");

        verticalLayout->addWidget(lineEdit_id);

        tableView = new QTableView(centralwidget);
        tableView->setObjectName("tableView");

        verticalLayout->addWidget(tableView);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Gestion des Matchs", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Informations du Match", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\303\211quipe 1:", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\303\211quipe 2:", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Date:", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Lieu:", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Type:", nullptr));
        comboBox_type->setItemText(0, QCoreApplication::translate("MainWindow", "Amical", nullptr));
        comboBox_type->setItemText(1, QCoreApplication::translate("MainWindow", "Championnat", nullptr));
        comboBox_type->setItemText(2, QCoreApplication::translate("MainWindow", "Coupe", nullptr));

        label_7->setText(QCoreApplication::translate("MainWindow", "\303\211tat:", nullptr));
        comboBox_etat->setItemText(0, QCoreApplication::translate("MainWindow", "Programm\303\251", nullptr));
        comboBox_etat->setItemText(1, QCoreApplication::translate("MainWindow", "En cours", nullptr));
        comboBox_etat->setItemText(2, QCoreApplication::translate("MainWindow", "Termin\303\251", nullptr));
        comboBox_etat->setItemText(3, QCoreApplication::translate("MainWindow", "Annul\303\251", nullptr));

        pushButton_ajouter->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        pushButton_modifier->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        pushButton_supprimer->setText(QCoreApplication::translate("MainWindow", "Supprimer", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "ID:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
