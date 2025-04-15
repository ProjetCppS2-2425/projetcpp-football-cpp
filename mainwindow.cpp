#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTextDocument>
#include <QPrinter>
#include <QFileDialog>
#include <QFileInfo>
#include <QTextStream>
#include <QPainter>
#include <QChartView>
#include <QPieSlice>
#include <QPieSeries>
#include <QList>
#include <QSqlQuery>
#include <QIntValidator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set integer validators for input fields
    QIntValidator *intValidator = new QIntValidator(0, 100000, this);
    ui->id_billet->setValidator(intValidator);
    ui->id_match->setValidator(intValidator);

    ui->prix->setValidator(intValidator);

    // Initialize the table view model
    ui->table_Billets->setModel(b.afficher());
    stat();
}

MainWindow::~MainWindow()
{
    delete ui;
}






void MainWindow::on_bt_tri_clicked()
{
    QString choix = ui->choix_tri->currentText();
    QString asc = ui->asc->currentText();
    ui->table_Billets->setModel(b.tri(choix, asc));

}

void MainWindow::on_pdf_clicked()
{
    QString strStream;
    QTextStream out(&strStream);
    const int rowCount = ui->table_Billets->model()->rowCount();
    const int columnCount = ui->table_Billets->model()->columnCount();

    // HTML header
    out << "<html>\n"
           "<head>\n"
           "<meta Content=\"Text/html; charset=Windows-1251\">\n"
           "<title>Liste des formations</title>\n"
           "<style>\n"
           "table {\n"
           "    width: 100%;\n"
           "    border-collapse: collapse;\n"
           "}\n"
           "th, td {\n"
           "    padding: 8px;\n"
           "    text-align: left;\n"
           "    border-bottom: 1px solid #ddd;\n"
           "}\n"
           "tr:nth-child(even) {\n"
           "    background-color: #f2f2f2;\n"
           "}\n"
           "</style>\n"
           "</head>\n"
           "<body bgcolor=#ffffff link=#5000A0>\n"
           "<center><H1>Liste des Billets</H1></center><br/><br/>\n"
           "<table>\n";

    // Table headers
    out << "<thead><tr bgcolor=#f0f0f0><th>Numero</th>";
    for (int column = 0; column < columnCount; column++) {
        if (!ui->table_Billets->isColumnHidden(column)) {
            out << QString("<th>%1</th>").arg(ui->table_Billets->model()->headerData(column, Qt::Horizontal).toString());
        }
    }
    out << "</tr></thead>\n";

    // Table data
    for (int row = 0; row < rowCount; row++) {
        out << "<tr><td>" << row + 1 << "</td>";
        for (int column = 0; column < columnCount; column++) {
            if (!ui->table_Billets->isColumnHidden(column)) {
                QString data = ui->table_Billets->model()->data(ui->table_Billets->model()->index(row, column)).toString().simplified();
                out << QString("<td>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
            }
        }
        out << "</tr>\n";
    }

    // Save as PDF
    QString fileName = QFileDialog::getSaveFileName(this, "Sauvegarder en PDF", QString(), "*.pdf");
    if (QFileInfo(fileName).suffix().isEmpty()) {
        fileName.append(".pdf");
    }

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize::A4);
    printer.setOutputFileName(fileName);

    QTextDocument doc;
    doc.setHtml(strStream);
    doc.print(&printer);
}

void MainWindow::stat()
{
    // Clear previous widgets
    QList<QWidget*> childWidgets = ui->label_stat->findChildren<QWidget*>();
    for (QWidget* childWidget : childWidgets) {
        childWidget->deleteLater();
    }
    ui->label_stat->clear();
    ui->label_stat->hide();

    // Statistics calculation
    int s0 = b.countType("amical");
    int s1 = b.countType("normal");
    int total = s0 + s1;

    float x = (total != 0) ? (s0 * 100.0f) / total : 0.0f;
    float x1 = (total != 0) ? (s1 * 100.0f) / total : 0.0f;

    // Adjust percentages to ensure they sum up to 100%
    float totalPercentage = x + x1;
    if (totalPercentage != 100.0f && total != 0) {
        float correction = 100.0f - totalPercentage;
        x += correction;
    }

    QString ch1 = QString("amical %1%").arg(QString::number(x, 'f', 2));
    QString ch2 = QString("normal %2%").arg(QString::number(x1, 'f', 2));

    // Create pie chart
    QPieSeries *series = new QPieSeries();
    series->setHoleSize(0.35);



    QPieSlice *slice= series->append(ch1,x);
    slice->setLabelVisible();
    slice->setLabelColor(QColor(Qt::white));
        slice->setBrush(QColor(Qt::blue));

    QPieSlice *slice1= series->append(ch2,x1);
    slice1->setLabelVisible();
    slice1->setLabelColor(QColor(Qt::white));
    slice1->setBrush(QColor(Qt::white));

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setBackgroundBrush(QBrush(QColor(187, 93, 87, 0)));

    QChartView *chartview = new QChartView(chart);
    chartview->setRenderHint(QPainter::Antialiasing);
    chartview->setFixedSize(ui->label_stat->size());
    chartview->setParent(ui->label_stat);
    ui->label_stat->setStyleSheet("background:transparent; color:white;");
    ui->label_stat->show();
}



void MainWindow::on_table_Billets_clicked(const QModelIndex &index)
{
    QAbstractItemModel* model = ui->table_Billets->model();
    int row = index.row();

    // Populate input fields with selected row data
    ui->id_billet->setText(model->data(model->index(row, 0)).toString());
    ui->id_match->setText(model->data(model->index(row, 1)).toString());
    ui->nombre_de_billets->setText(model->data(model->index(row, 2)).toString());
    ui->date->setDate(model->data(model->index(row, 3)).toDate());
    ui->siege->setText(model->data(model->index(row, 4)).toString());
    ui->prix->setText(model->data(model->index(row, 5)).toString());
    ui->type->setCurrentText(model->data(model->index(row, 6)).toString());
}

void MainWindow::clearInputs()
{
    ui->id_billet->clear();
    ui->id_match->clear();
    ui->nombre_de_billets->clear();
    ui->date->setDate(QDate::currentDate()); // Reset to current date or set to a specific date
    ui->siege->clear();
    ui->prix->clear();
    ui->type->setCurrentIndex(0); // Reset to the first item in the combo box
}

void MainWindow::on_chercher_textChanged(const QString &text)
{
    QString choix = ui->choix_tri->currentText();
    ui->table_Billets->setModel(b.chercher(choix, text));


}

void MainWindow::on_ajouter_clicked()
{
    int id_billet = ui->id_billet->text().toInt();
    int id_match = ui->id_match->text().toInt();
    int nombre_de_billets = ui->nombre_de_billets->text().toInt();
    QDate date = ui->date->date();
    QString siege = ui->siege->text();
    int prix = ui->prix->text().toInt();
    QString type = ui->type->currentText();

    // Check if the billet ID already exists
    if (b.idExists(id_billet)) {
        QMessageBox::critical(this, "Error", "ID billet déjà existe.", QMessageBox::Cancel);
        return;
    }

    // Check for empty fields
    if (ui->id_billet->text().isEmpty() || ui->id_match->text().isEmpty() ||
        ui->nombre_de_billets->text().isEmpty() || siege.isEmpty() ||
        ui->prix->text().isEmpty()) {
        QMessageBox::critical(this, "Error", "Tu dois remplir tous les champs.", QMessageBox::Cancel);
        return;
    }

    // Add the billet
    bool test = b.addBillet(id_billet, id_match, nombre_de_billets, date, siege, prix, type);
    if (test) {
        QMessageBox::information(this, "Success", "Ajout avec succès.", QMessageBox::Cancel);
        ui->table_Billets->setModel(b.afficher()); // Refresh the table model
        clearInputs(); // Clear the input fields
        stat(); // Update statistics
    } else {
        QMessageBox::critical(this, "Error", "Échec d'ajout.", QMessageBox::Cancel);
    }
}

void MainWindow::on_modifier_clicked()
{
    int id_billet = ui->id_billet->text().toInt();
    int id_match = ui->id_match->text().toInt();
    int nombre_de_billets = ui->nombre_de_billets->text().toInt();
    QDate date = ui->date->date();
    QString siege = ui->siege->text();
    int prix = ui->prix->text().toInt();
    QString type = ui->type->currentText();

    // Check if the billet ID exists
    if (!b.idExists(id_billet)) {
        QMessageBox::critical(this, "Error", "ID n'existe pas.", QMessageBox::Cancel);
        return;
    }

    // Check for empty fields
    if (ui->id_billet->text().isEmpty() || ui->id_match->text().isEmpty() ||
        ui->nombre_de_billets->text().isEmpty() || siege.isEmpty() ||
        ui->prix->text().isEmpty()) {
        QMessageBox::critical(this, "Error", "Tu dois remplir tous les champs.", QMessageBox::Cancel);
        return;
    }

    // Modify the billet
    bool test = b.modifyBillet(id_billet, id_match, nombre_de_billets, date, siege, prix, type);
    if (test) {
        QMessageBox::information(this, "Success", "Modification avec succès.", QMessageBox::Cancel);
        ui->table_Billets->setModel(b.afficher()); // Refresh the table model
        clearInputs(); // Clear the input fields
        stat(); // Update statistics
    } else {
        QMessageBox::critical(this, "Error", "Échec de modification.", QMessageBox::Cancel);
    }
}

void MainWindow::on_supprimer_clicked()
{
    int id = ui->id_billet->text().toInt();
    if (!b.idExists(id)) {
        QMessageBox::critical(this, "Error", "ID n'existe pas.", QMessageBox::Cancel);
        return;
    }

    bool test = b.deleteBillet(id);
    if (test) {
        QMessageBox::information(this, "Success", "Suppression avec succès.", QMessageBox::Cancel);
        ui->table_Billets->setModel(b.afficher());
        stat();
    } else {
        QMessageBox::critical(this, "Error", "Échec de suppression.", QMessageBox::Cancel);
    }
}

