#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include "product.h"

#include <QWidget>
#include <QScrollArea>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>

namespace Ui {
class HomePage;
}

class HomePage : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = nullptr);
    ~HomePage();

private:
    Ui::HomePage *ui;
    QVBoxLayout *productGridLayout;
    QLineEdit *searchBar;
    QComboBox *categoryFilter;
    QList<Product> allProducts;
public:
    void updateProductGrid(const QString &searchText = "", const QString &category = "");
    void clearLayout(QLayout*);
    void onSearchTextChanged(const QString &text);
    void onFilterChanged(int);

};

#endif // HOMEPAGE_H
