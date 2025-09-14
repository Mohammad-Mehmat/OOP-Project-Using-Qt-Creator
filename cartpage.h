#ifndef CARTPAGE_H
#define CARTPAGE_H

#include <QWidget>
#include <QScrollArea>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>

#include <QTableWidget>
#include <QPushButton>

namespace Ui {
class CartPage;
}

class CartPage : public QWidget
{
    Q_OBJECT

public:
    explicit CartPage(QWidget *parent = nullptr);
    ~CartPage();

private slots:
    void removeProductFromCart(int row);
    void onBuyNowClicked();

private:
    Ui::CartPage *ui;

    QTableWidget *cartTable;
    QLabel *cartSummaryLabel;
    QLabel *summary;
    QPushButton *buyNowButton;
public:
    void updateCart();
    void populateCartTable();
    void updateCartSummary();
};

#endif // CARTPAGE_H
