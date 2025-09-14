#ifndef PRODUCTCARD_H
#define PRODUCTCARD_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QImage>
#include <QPixmap>

#include "product.h"

namespace Ui {
class ProductCard;
}

class ProductCard : public QWidget
{
    Q_OBJECT

public:
    explicit ProductCard(QWidget *parent = nullptr);
    void setProductInfo(Product product, QString usertype, bool isHomePage);
    ~ProductCard();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ProductCard *ui;
    Product product;
};

#endif // PRODUCTCARD_H
