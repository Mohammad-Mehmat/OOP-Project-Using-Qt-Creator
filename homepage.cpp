#include "homepage.h"
#include "ui_homepage.h"
#include "storepage.h"
#include "product.h"
#include "Database.h"
#include "productcard.h"
#include "currentuser.h"

#include <QGridLayout>
#include <QLabel>
#include <QDebug>

HomePage::HomePage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HomePage)
{
    ui->setupUi(this);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    QWidget *scrollWidget = new QWidget(scrollArea);
    scrollArea->setWidget(scrollWidget);

    productGridLayout = new QVBoxLayout(scrollWidget);
    scrollWidget->setLayout(productGridLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *topbar = new QHBoxLayout(this);

    // Create and add the search bar
    searchBar = new QLineEdit(this);
    searchBar->setPlaceholderText("Search...");
    connect(searchBar, &QLineEdit::textChanged, this, &HomePage::onSearchTextChanged);

    searchBar->setStyleSheet(
        "QLineEdit {"
            "   background-color: #0b5572;"
            "   color: white;"
            "   font-weight: bold;"
            "   padding: 5px;"
            "   border-radius: 5px;"
            "   height: 30px;"
            "   width: 200px;"
            "   border: 0px;"
        "}"
        );
    topbar->addWidget(searchBar);

    // Create and add the category filter
    categoryFilter = new QComboBox(this);
    categoryFilter->addItem("All Categories");
    categoryFilter->addItems({"Smartphone", "Power bank and Charging", "Gear and Devices", "Camera and Visual", "Audio"});
    connect(categoryFilter, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &HomePage::onFilterChanged);
    categoryFilter->setStyleSheet(
        "QComboBox {"
        "   background-color: #0b5572;"
        "   color: white;"
        "   font-weight: bold;"
        "   padding: 5px;"
        "   border-radius: 5px;"
        "   height: 30px;"
        "   width: 150px;"
        "   border: 0px;"
        "   margin-left: 10px;"
        "}"
        "QComboBox QAbstractItemView {"
        "   background-color: #0b5572;"
        "   color: white;"
        "   selection-background-color: #82bfc7;"
        "   selection-color: black;"
        "}"
        );
    topbar->addWidget(categoryFilter);

    mainLayout->addItem(topbar);
    mainLayout->addWidget(scrollArea);
    setLayout(mainLayout);

    updateProductGrid();
}

HomePage::~HomePage()
{
    delete ui;
}

void HomePage::clearLayout(QLayout *layout) {
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

void HomePage::updateProductGrid(const QString &searchText, const QString &category)
{
    QString userType = CurrentUser::getInstance().getUser().getUserType();
    clearLayout(productGridLayout);

    allProducts = Database::getInstance().getAllProducts(-1, -1);

    QList<Product> filteredProducts;

    foreach (const Product &product, allProducts) {
        bool matchesSearch = product.getProductName().contains(searchText, Qt::CaseInsensitive) ||
                             product.getDescription().contains(searchText, Qt::CaseInsensitive);
        bool matchesCategory = (category == "") || (category == "All Categories") || (product.getCategory() == category);

        if (matchesSearch && matchesCategory) {
            filteredProducts.append(product);
        }
    }

    if (filteredProducts.isEmpty()) {
        QLabel *noProductsLabel = new QLabel("No products available", this);
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
        foreach (const Product &product, filteredProducts) {

            if (row % cardsPerRow == 0) {
                rowLayout = new QHBoxLayout();
                productGridLayout->addLayout(rowLayout);
            }

            // Create a product card
            ProductCard *card = new ProductCard();
            card->setProductInfo(product, userType, true);
            card->setFixedSize(200, 300);
            rowLayout->addWidget(card);
            row++;
        }
    }
}

void HomePage::onSearchTextChanged(const QString &text)
{
    updateProductGrid(text, categoryFilter->currentText());
}

void HomePage::onFilterChanged(int index)
{
    QString category = categoryFilter->itemText(index);
    updateProductGrid(searchBar->text(), category);
}
