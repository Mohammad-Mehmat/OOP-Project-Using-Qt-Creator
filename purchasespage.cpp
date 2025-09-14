#include "purchasespage.h"
#include "ui_purchasespage.h"

#include "Database.h"
#include "currentuser.h"

#include <QTableWidgetItem>
#include <QLabel>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHeaderView>

PurchasesPage::PurchasesPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PurchasesPage)
{
    ui->setupUi(this);
    purchasesTable = new QTableWidget(this);
    purchasesTable->setColumnCount(5);
    purchasesTable->setHorizontalHeaderLabels(QStringList() << "Image" << "Name" << "Price" << "Description" << "Rate");
    purchasesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    purchasesTable->verticalHeader()->setVisible(false);
    purchasesTable->setSelectionMode(QAbstractItemView::NoSelection);
    purchasesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    purchasesTable->setGeometry(10, 10, 780, 580);
    // Set custom styles for the table
    purchasesTable->setStyleSheet(
        "QTableWidget {"
        "   border: none;"
        "   margin: 10px;"
        "   font-family: Arial, sans-serif;"
        "   font-size: 14px;"
        "   background-color: #f5f5f5;"
        "   color: #333;"
        "} "
        "QTableWidget::item {"
        "   padding: 8px;"
        "   height: 20px;"
        "} "
        "QHeaderView::section {"
        "   background-color: #0b5572;"
        "   color: white;"
        "   padding: 8px;"
        "   border: none;"
        "   font-weight: bold;"
        "} "
        "QTableWidget QTableCornerButton::section {"
        "   background-color: #f5f5f5;"
        "}"
        );
}

void PurchasesPage::populatePurchasesTable()
{
    int currentCustomerId = CurrentUser::getInstance().getUser().getId();
    QList<Product> purchasedProducts = Database::getInstance().getPurchasedProducts(currentCustomerId, true);

    purchasesTable->clearContents();
    purchasesTable->setRowCount(0);

    if (purchasedProducts.isEmpty()) {
        QLabel *noPurchasesLabel = new QLabel("No products purchased", this);
        noPurchasesLabel->setAlignment(Qt::AlignCenter);
        purchasesTable->setRowCount(1);
        purchasesTable->setSpan(0, 0, 1, 5);
        purchasesTable->setCellWidget(0, 0, noPurchasesLabel);
    } else {
        purchasesTable->setRowCount(purchasedProducts.size());

        for (int row = 0; row < purchasedProducts.size(); ++row) {
            const Product& product = purchasedProducts.at(row);
            QTableWidgetItem* nameItem = new QTableWidgetItem(product.getProductName());
            nameItem->setTextAlignment(Qt::AlignCenter);
            QTableWidgetItem* priceItem = new QTableWidgetItem("Rs. " + QString::number(product.getPrice()));
            priceItem->setTextAlignment(Qt::AlignCenter);
            QTableWidgetItem* descriptionItem = new QTableWidgetItem(product.getDescription());
            descriptionItem->setTextAlignment(Qt::AlignCenter);
            QLabel* productImageLabel = new QLabel();
            QPixmap pixmap;
            pixmap.loadFromData(product.getImage());
            productImageLabel->setPixmap(pixmap.scaled(70, 70, Qt::KeepAspectRatio));
            productImageLabel->setAlignment(Qt::AlignCenter);

            QPushButton* rateButton = nullptr;
            QWidget* starsWidget = new QWidget(this);
            QHBoxLayout* starsLayout = new QHBoxLayout(starsWidget);
            starsLayout->setContentsMargins(0, 0, 0, 0);
            starsLayout->setSpacing(0);
            starsWidget->setLayout(starsLayout);

            if (product.getRating() == -1) {
                rateButton = new QPushButton("Rate", this);
                rateButton->setStyleSheet(
                    "QPushButton {"
                    "   background-color: #82bfc7;"
                    "   font-weight: bold;"
                    "   height: 30px;"
                    "   width: 60px;"
                    "   border: 0px; "
                    "} "
                    "QPushButton:hover {"
                    "   background-color: #0b5572;"
                    "   color: white; "
                    "}"
                    );

                connect(rateButton, &QPushButton::clicked, this, [this, product]() {
                    onRateButtonClicked(product.getId());
                });
            } else {
                int rating = product.getRating();
                for (int i = 0; i < 5; ++i) {
                    QLabel* starLabel = new QLabel(starsWidget);
                    if (i < rating && rating != -1) {
                        starLabel->setText("★");
                    } else {
                        starLabel->setText("☆");
                    }

                    starsLayout->addWidget(starLabel);
                    starLabel->setAlignment(Qt::AlignCenter);
                    starLabel->setStyleSheet(
                        "QLabel {"
                        "   margin: 0px;"
                        "   padding: 0px;"
                        "   background-color: white;"
                        "   color: #82bfc7;"
                        "   font-weight: bold;"
                        "   font-size: 18px;"
                        "}"
                        );
                }
            }

            purchasesTable->setRowHeight(row, 100);

            purchasesTable->setCellWidget(row, 0, productImageLabel);
            purchasesTable->setItem(row, 1, nameItem);
            purchasesTable->setItem(row, 2, priceItem);
            purchasesTable->setItem(row, 3, descriptionItem);
            if (rateButton) {
                purchasesTable->setCellWidget(row, 4, rateButton);
            } else {
                purchasesTable->setCellWidget(row, 4, starsWidget);
            }
        }
    }
}

PurchasesPage::~PurchasesPage()
{
    delete ui;
}

void PurchasesPage::onRateButtonClicked(int productId)
{
    bool ok;
    int rating = QInputDialog::getInt(this, "Rate Product", "Rate this product (1-5):", 1, 1, 5, 1, &ok);

    if (ok) {
        bool isUpdated = Database::getInstance().rateProduct(productId, rating);
        if (isUpdated) {
            populatePurchasesTable();
            QMessageBox::information(this, "Success", "Rated Successfully!");
        }
        else {
            QMessageBox::critical(this, "Error", "Failed to Rate");

        }
    }
}
