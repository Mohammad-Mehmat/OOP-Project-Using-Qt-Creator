#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

#include "product.h"

class Database {
public:
    // Static method to get the single instance of the database
    static Database& getInstance();

    // Method to get the QSqlDatabase connection
    QSqlDatabase getConnection();

private:
    QSqlDatabase db; // The actual database connection

    // Private constructor for singleton
    Database();

    // Delete copy constructor and assignment operator to prevent copies
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    // Private destructor
    ~Database();
public:
    bool insertProduct(const Product &product);
    bool updateProduct(const Product &product);
    bool deleteProduct(int productId);
    Product getProduct(int productId);
    QList<Product> getAllProducts(int sellerId = -1, int customerId  = -1);
    bool buyProductsInCart(int, const QList<Product>&);
    QList<Product> getPurchasedProducts(int id, bool isCustomer);
    bool rateProduct(int productId, int rating);
    QMap<int, int> getSellerRatings(int sellerId);
    QMap<QString, int> getProductOverview(int sellerId);


};

#endif // DATABASE_H
