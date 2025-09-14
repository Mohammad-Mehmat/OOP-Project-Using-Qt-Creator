#include "form.h"
#include "ui_form.h"
#include <QBarSet>
#include <QBarSeries>
#include <QChart>
#include <QBarCategoryAxis>
#include <QValueAxis>

Form::Form(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Form)
{
    ui->setupUi(this);
    updateProductsChart();
}

Form::~Form()
{
    delete ui;
}

void Form::updateProductsChart()
{
    // Dummy data for testing
    int total = 100;     // Total products
    int sold = 40;       // Sold products
    int available = 60;  // Available products

    QBarSet *set = new QBarSet("Products");
    *set << total << sold << available;

    // Set color for the bar set
    set->setColor(QColor("#82bfc7")); // Use the theme color for all bars

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
    axisY->setRange(0, total); // Assuming total is the maximum
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    ui->graphicsViewProducts->setChart(chart);
}
