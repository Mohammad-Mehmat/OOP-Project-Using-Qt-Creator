#include "cartpage.h"
#include "ui_cartpage.h"
#include "product.h"
#include "cart.h"
#include "currentuser.h"
#include "Database.h"

#include <QGridLayout>
#include <QLabel>
#include <QDebug>
#include <QHeaderView>
#include <QMessageBox>

CartPage::CartPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CartPage)
{
    ui->setupUi(this);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // First column: Cart Table
    QWidget *cartTableWidget = new QWidget(this);
    QVBoxLayout *cartTableLayout = new QVBoxLayout(cartTableWidget);

    cartTable = new QTableWidget(this);
    cartTable->setColumnCount(3);
    cartTable->setHorizontalHeaderLabels({"Product Name", "Price", "Remove"});
    cartTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    cartTable->verticalHeader()->setVisible(false);
    cartTable->setSelectionMode(QAbstractItemView::NoSelection);
    cartTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    cartTable->setStyleSheet(
        "QTableWidget {"
        "   border: none;"
        "   margin: 10px;"
        "   font-family: Arial, sans-serif;"
        "   font-size: 14px;"
        "   background-color: #f5f5f5;"
        "   color: #333;"
        "}"
        "QTableWidget::item {"
        "   padding: 8px;"
        "   height: 20px;"
        "}"
        "QHeaderView::section {"
        "   background-color: #0b5572;"
        "   color: white;"
        "   padding: 8px;"
        "   border: none;"
        "   font-weight: bold;"
        "}"
        "QTableWidget QTableCornerButton::section {"
        "   background-color: #f5f5f5;"
        "}"
        );

    cartTableLayout->addWidget(cartTable);
    mainLayout->addWidget(cartTableWidget);

    QWidget *cartSummaryWidget = new QWidget(this);
    QVBoxLayout *cartSummaryLayout = new QVBoxLayout(cartSummaryWidget);

    cartSummaryLabel = new QLabel(this);
    summary = new QLabel(this);
    cartSummaryLabel->setText("Cart Summary");
    cartSummaryLabel->setStyleSheet(
        "QLabel {"
        "   color: #0b5572;"
        "   font-size: 20px;"
        "   font-weight: bold;"
        "   padding: 5px;"
        "   border-radius: 5px;"
        "   height: 30px;"
        "   width: 200px;"
        "   border: 0px;"
        "   text-align: center;"
        "}"
        );

    summary->setStyleSheet(
        "QLabel {"
        "   color: #7cb7be;"
        "   font-size: 16px;"
        "   padding: 10px;"
        "}"
        );

    buyNowButton = new QPushButton("Buy Now", this);
    buyNowButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #82bfc7;"
        "   font-weight: bold;"
        "   border-radius: 5px;"
        "   height: 50px;"
        "   width: 150px;"
        "   border: 0px;"
        "} "
        "QPushButton:hover {"
        "   background-color: #0b5572;"
        "   color: white;"
        "}"
        );
    connect(buyNowButton, &QPushButton::clicked, this, &CartPage::onBuyNowClicked);

    cartSummaryLayout->addWidget(cartSummaryLabel);
    cartSummaryLayout->addWidget(summary);
    cartSummaryLayout->addWidget(buyNowButton);
    mainLayout->addWidget(cartSummaryWidget);

    setLayout(mainLayout);

    connect(&Cart::getInstance(), &Cart::cartChanged, this, &CartPage::updateCart);
}

CartPage::~CartPage()
{
    delete ui;
}

void CartPage::updateCart()
{
    populateCartTable();
    updateCartSummary();
}

void CartPage::updateCartSummary()
{
    Cart &cart = Cart::getInstance();
    const QList<Product> &products = cart.getProducts();
    double totalPrice = cart.getTotalPrice();

    QString summaryText;
    foreach (const Product &product, products) {
        summaryText += product.getProductName() + "\n";
    }
    summaryText += "\nTotal Price: Rs." + QString::number(totalPrice);

    summary->setText(summaryText);
}

void CartPage::removeProductFromCart(int row)
{
    Cart &cart = Cart::getInstance();
    const QList<Product> &products = cart.getProducts();
    if (row >= 0 && row < products.size()) {
        int productId = products.at(row).getId();
        cart.removeProduct(productId);
    }
}

void CartPage::populateCartTable()
{
    Cart &cart = Cart::getInstance();
    const QList<Product> &products = cart.getProducts();

    cartTable->clearContents();
    cartTable->setRowCount(0);

    if (products.isEmpty()) {
        QLabel *noProductsLabel = new QLabel("No products in cart", this);
        noProductsLabel->setAlignment(Qt::AlignCenter);

        cartTable->setRowCount(1);
        cartTable->setSpan(0, 0, 1, 3);
        cartTable->setCellWidget(0, 0, noProductsLabel);
    } else {
        cartTable->setRowCount(products.size());

        for (int row = 0; row < products.size(); ++row) {
            const Product &product = products.at(row);
            QTableWidgetItem *nameItem = new QTableWidgetItem(product.getProductName());
            QTableWidgetItem *priceItem = new QTableWidgetItem("Rs. " + QString::number(product.getPrice()));

            nameItem->setTextAlignment(Qt::AlignCenter);
            priceItem->setTextAlignment(Qt::AlignCenter);

            QPushButton *removeButton = new QPushButton("Remove", this);
            removeButton->setStyleSheet(
                "QPushButton {"
                "   background-color: #82bfc7;"
                "   font-weight: bold;"
                "   height: 30px;"
                "   width: 20px;"
                "   border: 0px;"
                "} "
                "QPushButton:hover {"
                "   background-color: #0b5572;"
                "   color: white;"
                "}"
                );
            connect(removeButton, &QPushButton::clicked, this, [this, row]() {
                removeProductFromCart(row);
            });
            cartTable->setRowHeight(row, 50);

            cartTable->setItem(row, 0, nameItem);
            cartTable->setItem(row, 1, priceItem);
            cartTable->setCellWidget(row, 2, removeButton);
        }
    }
}

void CartPage::onBuyNowClicked() {
    Cart& cart = Cart::getInstance();
    const QList<Product>& cartProducts = cart.getProducts();

    if (cartProducts.isEmpty()) {
        qDebug() << "Cart is empty. No products to buy.";
        return;
    }

    int currentCustomerId = CurrentUser::getInstance().getUser().getId();
    bool success = Database::getInstance().buyProductsInCart(currentCustomerId, cartProducts);

    if (success) {
        cart.clearCart();
        populateCartTable();
        QMessageBox::information(this, "Success", "Items Purchased!!");
    } else {
        QMessageBox::critical(this, "Error", "Failed to purchase items");
    }
}
