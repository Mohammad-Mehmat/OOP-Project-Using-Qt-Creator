#ifndef CART_H
#define CART_H

#include <QObject>
#include <QList>
#include "product.h"

class Cart : public QObject
{
    Q_OBJECT

public:
    static Cart& getInstance()
    {
        static Cart instance;
        return instance;
    }

    void addProduct(const Product& product);
    bool containsProduct(int productId) const;
    void removeProduct(int productId);
    const QList<Product>& getProducts() const { return products; }
    double getTotalPrice() const { return totalPrice; }
    void clearCart();

signals:
    void cartChanged();

private:
    Cart() : totalPrice(0.0) {}
    ~Cart() {}
    Cart(const Cart&) = delete;
    Cart& operator=(const Cart&) = delete;

    QList<Product> products;
    double totalPrice;
};

#endif // CART_H
