#ifndef ANALYTICSPAGE_H
#define ANALYTICSPAGE_H

#include <QWidget>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChartView>
#include <QGraphicsScene>

namespace Ui {
class AnalyticsPage;
}

class AnalyticsPage : public QWidget
{
    Q_OBJECT

public:
    explicit AnalyticsPage(QWidget *parent = nullptr);
    ~AnalyticsPage();

    void updateAnalytics();
    void updateSoldItemsTable();
    void updateStoreInfo();
    void updateRatingsChart();
    void updateProductsChart();

private:
    Ui::AnalyticsPage *ui;
};

#endif // ANALYTICSPAGE_H
