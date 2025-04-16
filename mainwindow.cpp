#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Employes.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include "smtp.h"
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QTextDocument>
#include <QTextTable>
#include <QTextCursor>
#include <QDir>

void logMessage(const QString& message) {
    QFile file("debug_log.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
               << " - " << message << "\n";
        file.close();
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , employe("DefaultNom", "DefaultPrenom", "2025-04-11", "3000", "PosteDefaut", "email@example.com", "Homme", "password123", 1, 2)
{
    ui->setupUi(this);

    // Initialize database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QString dbPath = QDir::toNativeSeparators("D:/ProBracket II/probracket.db");
    db.setDatabaseName(dbPath);

    logMessage("Attempting to open database at: " + dbPath);

    if (!db.open()) {
        QString errorMsg = "Failed to open database: " + db.lastError().text();
        logMessage(errorMsg);
        QMessageBox::critical(this, "Database Error",
                              QString("Failed to connect to database at %1\nError: %2").arg(dbPath, db.lastError().text()));
        return;
    }

    logMessage("Database connection successful!");

    // Verify database structure
    QSqlQuery query;
    if (!query.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='EMPLOYES'")) {
        QString errorMsg = "Failed to check database structure: " + query.lastError().text();
        logMessage(errorMsg);
        QMessageBox::critical(this, "Database Error", "Failed to verify database structure");
        return;
    }

    if (!query.next()) {
        logMessage("EMPLOYES table not found in database");
        QMessageBox::critical(this, "Database Error", "EMPLOYES table not found in database");
        return;
    }

    // Set up UI elements
    ui->groupBox_3->setVisible(false);
    ui->tabWidget->setVisible(false);
    changePage(0);

    // Set column headers
    QStringList headers;
    headers << "Nom" << "Prenom" << "Date Embauche" << "Salaire" << "Poste" << "Email" << "Sexe" << "ID_EMP" << "Modify" << "Delete";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // Configure table widget
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->verticalHeader()->setVisible(false);

    // Add search criteria to comboBox_2
    ui->comboBox_2->clear();
    ui->comboBox_2->addItem("ID_EMP");
    ui->comboBox_2->addItem("NOM");
    ui->comboBox_2->addItem("PRENOM");

    // Connect refresh button using old-style SIGNAL/SLOT syntax
    if (ui->pushButton_Refresh) {
        connect(ui->pushButton_Refresh, SIGNAL(clicked()), this, SLOT(on_pushButton_Refresh_clicked()));
        logMessage("Refresh button connected successfully");
    } else {
        logMessage("Warning: Refresh button not found in UI");
        QMessageBox::warning(this, "UI Error", "Refresh button not found in UI");
    }

    // Connect other signals
    connect(ui->Login, &QPushButton::clicked, this, [=]() { changePage(1); });
    connect(ui->Employes_2, &QPushButton::clicked, this, [=]() { changePage(1); });
    connect(ui->Matches_3, &QPushButton::clicked, this, [=]() { changePage(2); });
    connect(ui->Equipes_3, &QPushButton::clicked, this, [=]() { changePage(3); });
    connect(ui->Entraineurs_3, &QPushButton::clicked, this, [=]() { changePage(4); });
    connect(ui->Billets_3, &QPushButton::clicked, this, [=]() { changePage(5); });
    connect(ui->Sponsors_3, &QPushButton::clicked, this, [=]() { changePage(6); });

    connect(ui->pushButton_11, &QPushButton::clicked, this, &MainWindow::on_pushButton_11_clicked);
    connect(ui->pushButton_12, &QPushButton::clicked, this, &MainWindow::on_pushButton_12_clicked);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::on_pushButton_4_clicked);

    connect(ui->tableWidget, &QTableWidget::itemClicked, this, &MainWindow::onTableItemClicked);

    ui->tableWidget->setSortingEnabled(true);

    // Set Qt-compatible stylesheet
    QString styleSheet = "QPushButton {"
                         "    background-color: #4CAF50;"
                         "    color: white;"
                         "    border: none;"
                         "    padding: 8px 16px;"
                         "    margin: 4px 2px;"
                         "    border-radius: 4px;"
                         "}"
                         "QPushButton:hover {"
                         "    background-color: #45a049;"
                         "}"
                         "QPushButton:pressed {"
                         "    background-color: #3d8b40;"
                         "}"
                         "QTableWidget {"
                         "    border: 1px solid #ddd;"
                         "    gridline-color: #ddd;"
                         "    selection-background-color: #e0e0e0;"
                         "    selection-color: black;"
                         "}"
                         "QHeaderView::section {"
                         "    background-color: #f8f9fa;"
                         "    padding: 4px;"
                         "    border: 1px solid #ddd;"
                         "    font-weight: bold;"
                         "}";

    this->setStyleSheet(styleSheet);

    // Now that everything is set up, load the employees
    loadAllEmployees();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changePage(int index)
{
    if (index == 0) {
        ui->stackedWidget->setCurrentIndex(0);
        ui->groupBox_3->setVisible(false);
        ui->tabWidget->setVisible(false);
    } else {
        ui->stackedWidget->setCurrentIndex(index);
        ui->groupBox_3->setVisible(true);
        ui->tabWidget->setVisible(true);
    }
}

void MainWindow::on_Valider_clicked()
{
    QString nom = ui->nom->text();
    QString prenom = ui->prenom->text();
    QString dateEmbauche = ui->date_emb->date().toString("yyyy-MM-dd");
    QString salaire = ui->salaire->text();
    QString poste = ui->poste->currentText();
    QString email = ui->email->text();
    QString sexe = ui->homme->isChecked() ? "Homme" : "Femme";

    QString password = Employes::generateUniquePassword();

    // Assuming you have these values ready, for example:
    int idEntraineur = 1; // Replace with actual value
    int idM = 2;          // Replace with actual value

    qDebug() << "Nom: " << nom;
    qDebug() << "Prenom: " << prenom;
    qDebug() << "Date: " << dateEmbauche;
    qDebug() << "Salaire: " << salaire;
    qDebug() << "Poste: " << poste;
    qDebug() << "Email: " << email;
    qDebug() << "Sexe: " << sexe;
    qDebug() << "Password: " << password;
    qDebug() << "ID Entraineur: " << idEntraineur;
    qDebug() << "ID M: " << idM;

    if (!nom.isEmpty() && !prenom.isEmpty() && !dateEmbauche.isEmpty() && !salaire.isEmpty() && !email.isEmpty()) {
        Employes emp(nom, prenom, dateEmbauche, salaire, poste, email, sexe, password, idEntraineur, idM);

        if (emp.addToDatabase()) {
            // Get the ID_EMP after successful insertion
            QSqlQuery query;
            query.prepare("SELECT ID_EMP FROM EMPLOYES WHERE NOM = :nom AND PRENOM = :prenom AND EMAIL = :email");
            query.bindValue(":nom", nom);
            query.bindValue(":prenom", prenom);
            query.bindValue(":email", email);

            QString idEmp;
            if (query.exec() && query.next()) {
                idEmp = query.value(0).toString();
            }

            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);

            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(nom));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(prenom));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(dateEmbauche));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(salaire));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(poste));
            ui->tableWidget->setItem(row, 5, new QTableWidgetItem(email));
            ui->tableWidget->setItem(row, 6, new QTableWidgetItem(sexe));
            ui->tableWidget->setItem(row, 7, new QTableWidgetItem(idEmp)); // Display ID_EMP instead of password

            QPushButton *modifyButton = new QPushButton("Modify");
            connect(modifyButton, &QPushButton::clicked, this, [=]() { modifyRow(row); });
            ui->tableWidget->setCellWidget(row, 8, modifyButton);

            QPushButton *deleteButton = new QPushButton("Delete");
            connect(deleteButton, &QPushButton::clicked, this, [=]() { deleteRow(row); });
            ui->tableWidget->setCellWidget(row, 9, deleteButton);

            // Send welcome email to the new employee
            Smtp* smtp = new Smtp("selimsayeh@gmail.com", "xalz jrhg ourl necm", "smtp.gmail.com", 465);
            connect(smtp, &Smtp::status, this, &MainWindow::mailSent);

            QString emailBody = QString(
                                    "Bonjour %1 %2,\n\n"
                                    "Bienvenue chez probracket ! Nous sommes ravis de vous compter parmi nous.\n\n"
                                    "Voici vos informations de connexion :\n"
                                    "Email : %3\n"
                                    "Mot de passe : %4\n\n"
                                    "Cordialement,\n"
                                    "L'équipe probracket"
                                    ).arg(prenom, nom, email, password);

            smtp->sendMail(
                "selimsayeh@gmail.com",
                email,
                "Bienvenue chez Sodacoo",
                emailBody
                );

            QMessageBox::information(this, "Success", "Employee added successfully and welcome email sent!");
        }
    } else {
        qDebug() << "Form fields are incomplete. Row not added.";
        QMessageBox::warning(this, "Warning", "Please fill in all required fields.");
    }
}

void MainWindow::sendMail()
{
    QString recipientEmail = employe.getEmail();
    if (recipientEmail.isEmpty() || !recipientEmail.contains("@")) {
        QMessageBox::critical(this, "Invalid Email", "The recipient email is invalid.");
        return;
    }

    Smtp* smtp = new Smtp("selimsayeh@gmail.com", "xalz jrhg ourl necm", "smtp.gmail.com", 465);
    connect(smtp, &Smtp::status, this, &MainWindow::mailSent);

    smtp->sendMail(
        "selimsayeh@gmail.com",
        recipientEmail,
        "Sodacoo Avis",
        "Bienvenue chez Sodacoo ! Nous sommes ravis de vous compter parmi nous et avons hâte de vous servir."
        );
}

void MainWindow::mailSent(QString status)
{
    if (status == "Message sent")
        QMessageBox::information(nullptr, "mail envoyé", "Le mail d'accueil a été envoyé à " + employe.getEmail() + " !!");
}

void MainWindow::on_mailing_clicked()
{
    sendMail();
}

void MainWindow::modifyRow(int row)
{
    editingRow = row;
    ui->nom->setText(ui->tableWidget->item(row, 0)->text());
    ui->prenom->setText(ui->tableWidget->item(row, 1)->text());

    QString dateString = ui->tableWidget->item(row, 2)->text();
    QDate date = QDate::fromString(dateString, "yyyy-MM-dd");
    ui->date_emb->setDate(date);
    ui->salaire->setText(ui->tableWidget->item(row, 3)->text());
    ui->poste->setCurrentText(ui->tableWidget->item(row, 4)->text());
    ui->email->setText(ui->tableWidget->item(row, 5)->text());

    QString sexe = ui->tableWidget->item(row, 6)->text();
    ui->homme->setChecked(sexe == "Homme");
    ui->femme->setChecked(sexe == "Femme");
}

void MainWindow::deleteRow(int row)
{
    ui->tableWidget->removeRow(row);
}

void MainWindow::on_pushButton_11_clicked()
{
    try {
        // Get the ID from the QLineEdit
        QString idStr = ui->idmodif->text();
        qDebug() << "Attempting to update employee with ID:" << idStr;

        if (idStr.isEmpty()) {
            QMessageBox::warning(this, "Warning", "Please enter an employee ID.");
            return;
        }

        bool ok;
        int id = idStr.toInt(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Warning", "Invalid ID format. Please enter a valid number.");
            return;
        }

        // Validate form fields
        QString nom = ui->nom->text();
        QString prenom = ui->prenom->text();
        QString dateEmbauche = ui->date_emb->date().toString("yyyy-MM-dd");
        QString salaire = ui->salaire->text();
        QString poste = ui->poste->currentText();
        QString email = ui->email->text();
        QString sexe = ui->homme->isChecked() ? "Homme" : "Femme";

        if (nom.isEmpty() || prenom.isEmpty() || salaire.isEmpty() || email.isEmpty()) {
            QMessageBox::warning(this, "Warning", "Please fill in all required fields.");
            return;
        }

        // Get the current password from the database
        QSqlQuery query;
        query.prepare("SELECT PSW FROM EMPLOYES WHERE ID_EMP = :id");
        query.bindValue(":id", id);

        qDebug() << "Executing password query for ID:" << id;
        if (!query.exec()) {
            qDebug() << "Database error:" << query.lastError().text();
            QMessageBox::critical(this, "Database Error",
                                  "Could not retrieve employee information.\nError: " + query.lastError().text());
            return;
        }

        QString password;
        if (query.next()) {
            password = query.value(0).toString();
            qDebug() << "Password retrieved successfully";
        } else {
            qDebug() << "No employee found with ID:" << id;
            QMessageBox::critical(this, "Error", "No employee found with ID: " + QString::number(id));
            return;
        }

        // Create a new employee object with updated values
        Employes emp(nom, prenom, dateEmbauche, salaire, poste, email, sexe, password, 1, 2);
        qDebug() << "Employee object created, attempting database update...";

        // Update the database
        if (emp.updateToDatabase(id)) {
            qDebug() << "Database update successful, updating table...";

            // Find and update the row in the table
            bool rowFound = false;
            for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
                QTableWidgetItem *idItem = ui->tableWidget->item(row, 0);
                if (idItem && idItem->text().toInt() == id) {
                    qDebug() << "Found matching row:" << row;
                    // Update the table
                    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(nom));
                    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(prenom));
                    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(dateEmbauche));
                    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(salaire));
                    ui->tableWidget->setItem(row, 4, new QTableWidgetItem(poste));
                    ui->tableWidget->setItem(row, 5, new QTableWidgetItem(email));
                    ui->tableWidget->setItem(row, 6, new QTableWidgetItem(sexe));
                    ui->tableWidget->setItem(row, 7, new QTableWidgetItem(password));
                    rowFound = true;
                    break;
                }
            }

            if (!rowFound) {
                qDebug() << "Warning: Employee updated in database but not found in table";
            }

            QMessageBox::information(this, "Success", "Employee information updated successfully!");
            ui->idmodif->clear();
        } else {
            qDebug() << "Database update failed";
            QMessageBox::critical(this, "Error", "Failed to update employee information. Please check the debug output for details.");
        }
    } catch (const std::exception& e) {
        qDebug() << "Exception caught:" << e.what();
        QMessageBox::critical(this, "Error", QString("An unexpected error occurred: %1").arg(e.what()));
    } catch (...) {
        qDebug() << "Unknown exception caught";
        QMessageBox::critical(this, "Error", "An unknown error occurred.");
    }
}

void MainWindow::on_pushButton_12_clicked()
{
    try {
        // Get the ID from the QLineEdit
        QString idStr = ui->idmodif->text();
        qDebug() << "Attempting to delete employee with ID:" << idStr;

        if (idStr.isEmpty()) {
            QMessageBox::warning(this, "Warning", "Please enter an employee ID.");
            return;
        }

        bool ok;
        int id = idStr.toInt(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Warning", "Invalid ID format. Please enter a valid number.");
            return;
        }

        // Delete from database
        if (Employes::deleteById(id)) {
            // Find and remove the row from the table
            for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
                QTableWidgetItem *idItem = ui->tableWidget->item(row, 0);
                if (idItem && idItem->text().toInt() == id) {
                    ui->tableWidget->removeRow(row);
                    break;
                }
            }

            QMessageBox::information(this, "Success", "Employee deleted successfully!");
            ui->idmodif->clear();
        } else {
            QMessageBox::critical(this, "Error", "Failed to delete employee. ID might not exist.");
        }
    } catch (const std::exception& e) {
        qDebug() << "Exception caught:" << e.what();
        QMessageBox::critical(this, "Error", QString("An unexpected error occurred: %1").arg(e.what()));
    } catch (...) {
        qDebug() << "Unknown exception caught";
        QMessageBox::critical(this, "Error", "An unknown error occurred.");
    }
}

void MainWindow::onTableItemClicked(QTableWidgetItem *item)
{
    int row = item->row();

    // Get employee information from the table
    QString nom = ui->tableWidget->item(row, 0)->text();
    QString prenom = ui->tableWidget->item(row, 1)->text();
    QString dateEmbauche = ui->tableWidget->item(row, 2)->text();
    QString salaire = ui->tableWidget->item(row, 3)->text();
    QString poste = ui->tableWidget->item(row, 4)->text();
    QString email = ui->tableWidget->item(row, 5)->text();
    QString sexe = ui->tableWidget->item(row, 6)->text();
    QString idEmp = ui->tableWidget->item(row, 7)->text();

    // Create employee object
    Employes emp(nom, prenom, dateEmbauche, salaire, poste, email, sexe, "", 1, 2);

    // Create and show QR dialog
    Dialog__qr *qrDialog = new Dialog__qr(this);
    qrDialog->setEmployeeInfo(emp);
    qrDialog->show();
}

void MainWindow::on_pushButton_6_clicked()
{
    // Create a printer object
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPageSize::A4);
    printer.setPageOrientation(QPageLayout::Landscape);

    // Open file dialog to choose save location
    QString fileName = QFileDialog::getSaveFileName(this, "Export PDF", QString(), "*.pdf");
    if (fileName.isEmpty())
        return;

    printer.setOutputFileName(fileName);

    // Create a QTextDocument
    QTextDocument doc;
    doc.setDefaultFont(QFont("Arial", 12));

    // Create HTML content
    QString htmlContent = "<html><body>";

    // Add title
    htmlContent += "<h1 style='text-align: center; font-size: 24pt; margin-bottom: 20px;'>Employee Information Report</h1>";

    // Add date
    htmlContent += "<p style='margin-bottom: 20px;'>Generated on: " +
                   QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "</p>";

    // Start table
    htmlContent += "<table border='1' cellspacing='0' cellpadding='10' width='100%' style='border-collapse: collapse;'>";

    // Add table headers
    htmlContent += "<tr style='background-color: #f0f0f0;'>";
    QStringList headers = {"Nom", "Prenom", "Date Embauche", "Salaire", "Poste", "Email", "Sexe", "ID_EMP"};
    for (const QString& header : headers) {
        htmlContent += "<th style='font-size: 14pt; padding: 12px; text-align: left;'>" + header + "</th>";
    }
    htmlContent += "</tr>";

    // Add table content
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        htmlContent += QString("<tr style='background-color: %1;'>").arg(row % 2 == 0 ? "#ffffff" : "#f9f9f9");
        for (int col = 0; col < headers.size(); ++col) {
            QTableWidgetItem *item = ui->tableWidget->item(row, col);
            QString cellContent = item ? item->text() : "";
            htmlContent += "<td style='font-size: 12pt; padding: 10px;'>" + cellContent + "</td>";
        }
        htmlContent += "</tr>";
    }

    // Close table and HTML
    htmlContent += "</table></body></html>";

    // Set the HTML content
    doc.setHtml(htmlContent);

    // Set page size for the document using QPageLayout
    QPageLayout pageLayout = printer.pageLayout();
    doc.setPageSize(QSizeF(pageLayout.paintRect(QPageLayout::Point).size()));

    // Print to PDF
    doc.print(&printer);

    QMessageBox::information(this, "Success", "PDF exported successfully!");
}

void MainWindow::loadAllEmployees()
{
    logMessage("Starting loadAllEmployees()");

    // Clear the table first
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    // Get database connection
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid()) {
        logMessage("Database connection is not valid");
        QMessageBox::critical(this, "Database Error", "Database connection is not valid");
        return;
    }

    if (!db.isOpen()) {
        logMessage("Database is not open");
        QMessageBox::critical(this, "Database Error", "Database is not open");
        return;
    }

    // Prepare and execute query
    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYES ORDER BY ID_EMP DESC");

    logMessage("Executing query: " + query.lastQuery());

    if (!query.exec()) {
        QString errorMsg = "Failed to execute query: " + query.lastError().text();
        logMessage(errorMsg);
        QMessageBox::critical(this, "Database Error", errorMsg);
        return;
    }

    logMessage("Query executed successfully");

    int rowCount = 0;
    while (query.next()) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        rowCount++;

        // Get values from query
        QString idEmp = query.value("ID_EMP").toString();
        QString nom = query.value("NOM").toString();
        QString prenom = query.value("PRENOM").toString();
        QString dateEmbauche = query.value("DATE_EMBAUCHE").toString();
        QString salaire = query.value("SALAIRE").toString();
        QString poste = query.value("POSTE").toString();
        QString email = query.value("EMAIL").toString();
        QString sexe = query.value("SEXE").toString();

        logMessage(QString("Loading employee: ID=%1, Name=%2 %3").arg(idEmp, nom, prenom));

        // Set values in table
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(nom));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(prenom));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(dateEmbauche));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(salaire));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(poste));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(email));
        ui->tableWidget->setItem(row, 6, new QTableWidgetItem(sexe));
        ui->tableWidget->setItem(row, 7, new QTableWidgetItem(idEmp));

        // Add modify button
        QPushButton *modifyButton = new QPushButton("Modify");
        connect(modifyButton, &QPushButton::clicked, this, [=]() { modifyRow(row); });
        ui->tableWidget->setCellWidget(row, 8, modifyButton);

        // Add delete button
        QPushButton *deleteButton = new QPushButton("Delete");
        connect(deleteButton, &QPushButton::clicked, this, [=]() { deleteRow(row); });
        ui->tableWidget->setCellWidget(row, 9, deleteButton);
    }

    logMessage(QString("Finished loading employees. Total rows loaded: %1").arg(rowCount));

    if (rowCount == 0) {
        logMessage("No employees found in database");
        QMessageBox::information(this, "Information", "No employees found in database");
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    // First check if database is connected
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid() || !db.isOpen()) {
        logMessage("Database connection is not valid or not open");
        QMessageBox::critical(this, "Database Error", "Database connection is not valid or not open");
        return;
    }

    QString searchField = ui->comboBox_2->currentText();
    QString searchText;
    QString queryStr;
    QVariant value;

    // Get search text based on selected field
    if (searchField == "ID_EMP") {
        searchText = ui->cat->text().trimmed(); // Use catid specifically for ID search
        if (searchText.isEmpty()) {
            QMessageBox::information(this, "Search", "Please enter an ID to search for.");
            return;
        }
    } else {
        searchText = ui->cat->text().trimmed(); // Use cat for other searches
        if (searchText.isEmpty()) {
            loadAllEmployees();
            return;
        }
    }

    // Build query based on search field
    if (searchField == "ID_EMP") {
        logMessage("Starting ID search for: " + searchText);

        // For ID search, we'll do exact match
        queryStr = "SELECT ID_EMP, NOM, PRENOM, DATE_EMBAUCHE, SALAIRE, POSTE, EMAIL, SEXE, PSW, ID_ENTRAINEUR, ID_M FROM EMPLOYES WHERE ID_EMP = :id";
        value = searchText;
        logMessage("Searching database with ID: " + value.toString());

    } else if (searchField == "NOM") {
        queryStr = "SELECT ID_EMP, NOM, PRENOM, DATE_EMBAUCHE, SALAIRE, POSTE, EMAIL, SEXE, PSW, ID_ENTRAINEUR, ID_M FROM EMPLOYES WHERE NOM LIKE :nom";
        value = "%" + searchText + "%";
        logMessage("Searching for NOM: " + value.toString());
    } else if (searchField == "PRENOM") {
        queryStr = "SELECT ID_EMP, NOM, PRENOM, DATE_EMBAUCHE, SALAIRE, POSTE, EMAIL, SEXE, PSW, ID_ENTRAINEUR, ID_M FROM EMPLOYES WHERE PRENOM LIKE :prenom";
        value = "%" + searchText + "%";
        logMessage("Searching for PRENOM: " + value.toString());
    }

    // Clear and prepare table
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    // Execute search query
    QSqlQuery query;
    if (!query.prepare(queryStr)) {
        QString errorMsg = "Failed to prepare query: " + query.lastError().text();
        logMessage(errorMsg);
        QMessageBox::critical(this, "Query Error", errorMsg);
        return;
    }

    // Bind the appropriate parameter based on search field
    if (searchField == "ID_EMP") {
        query.bindValue(":id", value);
    } else if (searchField == "NOM") {
        query.bindValue(":nom", value);
    } else if (searchField == "PRENOM") {
        query.bindValue(":prenom", value);
    }

    logMessage("Executing search query: " + queryStr + " with value: " + value.toString());

    if (query.exec()) {
        int rowCount = 0;
        while (query.next()) {
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);
            rowCount++;

            // Get values from query using column names
            QString idEmp = query.value("ID_EMP").toString();
            QString nom = query.value("NOM").toString();
            QString prenom = query.value("PRENOM").toString();
            QString dateEmbauche = query.value("DATE_EMBAUCHE").toString();
            QString salaire = query.value("SALAIRE").toString();
            QString poste = query.value("POSTE").toString();
            QString email = query.value("EMAIL").toString();
            QString sexe = query.value("SEXE").toString();

            logMessage(QString("Found employee: ID=%1, Name=%2 %3").arg(idEmp, nom, prenom));

            // Set values in table
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(nom));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(prenom));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(dateEmbauche));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(salaire));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(poste));
            ui->tableWidget->setItem(row, 5, new QTableWidgetItem(email));
            ui->tableWidget->setItem(row, 6, new QTableWidgetItem(sexe));
            ui->tableWidget->setItem(row, 7, new QTableWidgetItem(idEmp));

            // Add modify button
            QPushButton *modifyButton = new QPushButton("Modify");
            connect(modifyButton, &QPushButton::clicked, this, [=]() { modifyRow(row); });
            ui->tableWidget->setCellWidget(row, 8, modifyButton);

            // Add delete button
            QPushButton *deleteButton = new QPushButton("Delete");
            connect(deleteButton, &QPushButton::clicked, this, [=]() { deleteRow(row); });
            ui->tableWidget->setCellWidget(row, 9, deleteButton);
        }

        if (rowCount == 0) {
            logMessage("No employees found matching search criteria");
            QMessageBox::information(this, "Search Result", "No matching employees found.");
        } else {
            logMessage(QString("Search complete. Found %1 employees").arg(rowCount));
        }
    } else {
        QString errorMsg = "Failed to search employees: " + query.lastError().text();
        logMessage(errorMsg);
        QMessageBox::warning(this, "Search Error", errorMsg);
    }
}

void MainWindow::on_pushButton_Refresh_clicked()
{
    logMessage("Refreshing employee list...");

    // Clear the table first
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    // Get database connection
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid() || !db.isOpen()) {
        logMessage("Database connection is not valid or not open");
        QMessageBox::critical(this, "Database Error", "Database connection is not valid or not open");
        return;
    }

    // Prepare and execute query
    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYES ORDER BY ID_EMP DESC");

    logMessage("Executing refresh query: " + query.lastQuery());

    if (!query.exec()) {
        QString errorMsg = "Failed to execute refresh query: " + query.lastError().text();
        logMessage(errorMsg);
        QMessageBox::critical(this, "Database Error", errorMsg);
        return;
    }

    logMessage("Refresh query executed successfully");

    int rowCount = 0;
    while (query.next()) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        rowCount++;

        // Get values from query
        QString idEmp = query.value("ID_EMP").toString();
        QString nom = query.value("NOM").toString();
        QString prenom = query.value("PRENOM").toString();
        QString dateEmbauche = query.value("DATE_EMBAUCHE").toString();
        QString salaire = query.value("SALAIRE").toString();
        QString poste = query.value("POSTE").toString();
        QString email = query.value("EMAIL").toString();
        QString sexe = query.value("SEXE").toString();

        logMessage(QString("Loading employee: ID=%1, Name=%2 %3").arg(idEmp, nom, prenom));

        // Set values in table
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(nom));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(prenom));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(dateEmbauche));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(salaire));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(poste));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(email));
        ui->tableWidget->setItem(row, 6, new QTableWidgetItem(sexe));
        ui->tableWidget->setItem(row, 7, new QTableWidgetItem(idEmp));

        // Add modify button
        QPushButton *modifyButton = new QPushButton("Modify");
        connect(modifyButton, &QPushButton::clicked, this, [=]() { modifyRow(row); });
        ui->tableWidget->setCellWidget(row, 8, modifyButton);

        // Add delete button
        QPushButton *deleteButton = new QPushButton("Delete");
        connect(deleteButton, &QPushButton::clicked, this, [=]() { deleteRow(row); });
        ui->tableWidget->setCellWidget(row, 9, deleteButton);
    }

    logMessage(QString("Refresh complete. Total rows loaded: %1").arg(rowCount));

    if (rowCount == 0) {
        logMessage("No employees found in database during refresh");
        QMessageBox::information(this, "Information", "No employees found in database");
    }
}
