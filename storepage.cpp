#include "storepage.h"
#include "ui_storepage.h"
#include "productcard.h"
#include "Database.h"
#include "currentuser.h"


#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>
#include "addproductdialog.h"

#include <QScrollArea>

#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

StorePage::StorePage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StorePage)
{
    ui->setupUi(this);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    QWidget *scrollWidget = new QWidget(scrollArea);
    scrollArea->setWidget(scrollWidget);

    productGridLayout = new QVBoxLayout(scrollWidget);
    scrollWidget->setLayout(productGridLayout);

    addProductButton = ui->addProductButton;
    connect(addProductButton, &QPushButton::clicked, this, &StorePage::onAddProductButtonClicked);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(scrollArea);
    mainLayout->addWidget(addProductButton);
    setLayout(mainLayout);

    addProductButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #82bfc7;"
        "   font-weight: bold;"
        "   border-radius: 5px;"
        "   height: 50px;"
        "   width: 150px;"
        "   border: 0px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #0b5572;"
        "   color: white;"
        "}"
    );

    updateProductGrid();
}

void StorePage::clearLayout(QLayout *layout) {
    if (!layout)
        return;

    while (QLayoutItem *item = layout->takeAt(0)) {
        if (QWidget *widget = item->widget()) {
            widget->deleteLater();
        }
        if (QLayout *childLayout = item->layout()) {
            clearLayout(childLayout);
        }
        delete item;
    }
}

void StorePage::updateProductGrid()
{
    int id = CurrentUser::getInstance().getUser().getId();
    QString userType = CurrentUser::getInstance().getUser().getUserType();

    clearLayout(productGridLayout);

    QList<Product> products = Database::getInstance().getAllProducts(id, -1);

    if (products.isEmpty()) {
        QLabel *noProductsLabel = new QLabel("No products added yet", this);
        noProductsLabel->setStyleSheet(
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

        QHBoxLayout *centeredLayout = new QHBoxLayout();
        centeredLayout->addStretch();
        centeredLayout->addWidget(noProductsLabel);
        centeredLayout->addStretch();

        productGridLayout->addLayout(centeredLayout);
    } else {
        const int cardsPerRow = 3;
        int row = 0;

        QHBoxLayout *rowLayout = nullptr;
        foreach (const Product &product, products) {
            if (row % cardsPerRow == 0) {
                rowLayout = new QHBoxLayout();
                productGridLayout->addLayout(rowLayout);
            }

            // Create a product card
            ProductCard *card = new ProductCard();
            card->setProductInfo(product, userType, false);
            card->setFixedSize(200, 300);
            rowLayout->addWidget(card);
            row++;
        }
    }
}

void StorePage::onAddProductButtonClicked()
{
    AddProductDialog dialog;
    dialog.exec();
    updateProductGrid();
}

StorePage::~StorePage()
{
    delete ui;
}
