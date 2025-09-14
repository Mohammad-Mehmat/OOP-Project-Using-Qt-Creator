#include "analyticspage.h"
#include "ui_analyticspage.h"

#include "product.h"
#include "Database.h"
#include "currentuser.h"

#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChart>

AnalyticsPage::AnalyticsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnalyticsPage)
{
    ui->setupUi(this);
    ui->tableSoldItems->setStyleSheet(
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

AnalyticsPage::~AnalyticsPage()
{
    delete ui;
}

void AnalyticsPage::updateAnalytics() {
    updateSoldItemsTable();
    updateStoreInfo();
    updateRatingsChart();
    updateProductsChart();
}

void AnalyticsPage::updateSoldItemsTable()
{
    const int currentUserId = CurrentUser::getInstance().getUser().getId();
    QList<Product> items = Database::getInstance().getPurchasedProducts(currentUserId, false);

    ui->tableSoldItems->clear();
    ui->tableSoldItems->setRowCount(items.size());
    ui->tableSoldItems->setColumnCount(5);
    ui->tableSoldItems->setHorizontalHeaderLabels(QStringList() << "Image" << "Name" << "Price" << "Description" << "Rating");
    ui->tableSoldItems->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableSoldItems->verticalHeader()->setVisible(false);

    for (int row = 0; row < items.size(); ++row) {
        const QString& itemName = items[row].getProductName();
        const QString& itemDescription = items[row].getDescription();
        int itemRating = items[row].getRating();

        QLabel* itemImageLabel = new QLabel();
        QPixmap pixmap;
        pixmap.loadFromData(items[row].getImage());
        itemImageLabel->setPixmap(pixmap.scaled(30, 30, Qt::KeepAspectRatio));
        itemImageLabel->setAlignment(Qt::AlignCenter);

        QTableWidgetItem* nameItem = new QTableWidgetItem(itemName);
        nameItem->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem* descriptionItem = new QTableWidgetItem(itemDescription);
        descriptionItem->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem* priceItem = new QTableWidgetItem("Rs. " + QString::number(items[row].getPrice()));
        priceItem->setTextAlignment(Qt::AlignCenter);

        QLabel* starsWidget = new QLabel();
        starsWidget->setAlignment(Qt::AlignCenter);
        QHBoxLayout* starsLayout = new QHBoxLayout(starsWidget);
        starsLayout->setContentsMargins(0, 0, 0, 0);
        starsLayout->setSpacing(0);
        starsWidget->setLayout(starsLayout);
        if (itemRating == -1) {
            starsWidget->setText("No rating yet");
            starsWidget->setStyleSheet(
                "font-size: 8px;"
                );
        } else {
            for (int i = 0; i < 5; ++i) {
                QLabel* starLabel = new QLabel(starsWidget);
                if (i < itemRating && itemRating != -1) {
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
                    "   font-size: 10px;"
                    "}"
                    );
            }
        }

        ui->tableSoldItems->setRowHeight(row, 40);
        ui->tableSoldItems->setCellWidget(row, 0, itemImageLabel);
        ui->tableSoldItems->setItem(row, 1, nameItem);
        ui->tableSoldItems->setItem(row, 2, priceItem);
        ui->tableSoldItems->setItem(row, 3, descriptionItem);
        ui->tableSoldItems->setCellWidget(row, 4, starsWidget);
    }
}

void AnalyticsPage::updateStoreInfo()
{
    QPixmap pixmap;
    pixmap.loadFromData(CurrentUser::getInstance().getUser().getLogo());
    ui->labelStoreLogo->setPixmap(pixmap.scaled(50, 50, Qt::KeepAspectRatio));
    ui->labelStoreLogo->setAlignment(Qt::AlignCenter);
    ui->labelStoreName->setText(CurrentUser::getInstance().getUser().getBrandName());
    ui->labelStoreName->setAlignment(Qt::AlignCenter);
    ui->labelStoreName->setStyleSheet(
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
}
void AnalyticsPage::updateRatingsChart()
{
    QMap<int, int> ratings = Database::getInstance().getSellerRatings(CurrentUser::getInstance().getUser().getId());

    QPieSeries *series = new QPieSeries();
    for (auto it = ratings.begin(); it != ratings.end(); ++it) {
        QPieSlice *slice = series->append(QString::number(it.key()) + " Stars", it.value());

        switch (it.key()) {
        case 1: slice->setBrush(QColor("#d9534f")); break;
        case 2: slice->setBrush(QColor("#f0ad4e")); break;
        case 3: slice->setBrush(QColor("#f0e68c")); break;
        case 4: slice->setBrush(QColor("#5bc0de")); break;
        case 5: slice->setBrush(QColor("#82bfc7")); break;
        default: slice->setBrush(QColor("#dcdcdc")); break;
        }
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Seller Ratings");

    ui->graphicsViewRatings->setChart(chart);
}

void AnalyticsPage::updateProductsChart()
{
    QMap<QString, int> productOverview = Database::getInstance().getProductOverview(CurrentUser::getInstance().getUser().getId());

    int total = productOverview["Total"];
    int sold = productOverview["Sold"];
    int available = productOverview["Available"];

    QBarSet *set = new QBarSet("Products");
    *set << total << sold << available;

    set->setColor(QColor("#82bfc7"));

    QBarSeries *series = new QBarSeries();
    series->append(set);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Products Overview");

    QStringList categories;
    categories << "Total" << "Sold" << "Available";
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, total);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    ui->graphicsViewProducts->setChart(chart);
}

