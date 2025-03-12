#ifndef MATCHWIDGET_H
#define MATCHWIDGET_H

#include <QWidget>
#include "match.h"

namespace Ui {
class MatchWidget;
}

class MatchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MatchWidget(QWidget *parent = nullptr);
    ~MatchWidget();

private slots:
    void on_ajouterMatch_clicked();
    void on_tableWidget_cellClicked(int row, int column);
    void on_recherche_textChanged(const QString &arg1);
    void on_tri_currentIndexChanged(const QString &arg1);

private:
    Ui::MatchWidget *ui;
    Match tmpMatch;
    void refreshTable();
    void clearInputs();
};

#endif // MATCHWIDGET_H

