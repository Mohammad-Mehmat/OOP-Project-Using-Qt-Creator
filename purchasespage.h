#ifndef PURCHASESPAGE_H
#define PURCHASESPAGE_H

#include <QWidget>
#include <QTableWidgetItem>
#include <QTableWidget>
#include <QPushButton>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
class PurchasesPage;
}

class PurchasesPage : public QWidget
{
    Q_OBJECT

public:
    explicit PurchasesPage(QWidget *parent = nullptr);
    ~PurchasesPage();

private:
    Ui::PurchasesPage *ui;
    QTableWidget *purchasesTable;
public:
    void populatePurchasesTable();
    void onRateButtonClicked(int productId);
};

#endif // PURCHASESPAGE_H
