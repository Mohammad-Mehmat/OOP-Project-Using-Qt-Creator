#include "Database.h"
#include "product.h"
#include <QCoreApplication>

#include "QSqlQuery"

Database& Database::getInstance() {
    static Database instance;
    return instance;
}

Database::Database() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/imran/OneDrive/Documents/EE/Semester 3/OOP/project/ElectroSphere/ElectroSphereDb/electrosphere.db");

    if (!db.open()) {
        qDebug() << "Database connection failed: " << db.lastError().text();
    } else {
        qDebug() << "Database connected successfully.";
    }
}

Database::~Database() {
    if (db.isOpen()) {
        db.close();
    }
}

QSqlDatabase Database::getConnection() {
    return db;
}

bool Database::insertProduct(const Product &product) {
    QSqlQuery query;
    query.prepare("INSERT INTO product (product_name, description, category, image, price, rating, customerId, sellerId) "
                  "VALUES (:product_name, :description, :category, :image, :price, :rating, :customerId, :sellerId)");

    query.bindValue(":product_name", product.getProductName());
    query.bindValue(":description", product.getDescription());
    query.bindValue(":image", product.getImage());
    query.bindValue(":price", product.getPrice());
    query.bindValue(":rating", product.getRating());
    query.bindValue(":customerId", product.getCustomerId());
    query.bindValue(":sellerId", product.getSellerId());
    query.bindValue(":category", product.getCategory());

    if (!query.exec()) {
        qWarning() << "Failed to insert product:" << query.lastError().text();
        return false;
    }
    return true;
}

bool Database::updateProduct(const Product &product) {
    QSqlQuery query;
    query.prepare("UPDATE product SET product_name = :product_name, description = :description, "
                  "image = :image, price = :price, rating = :rating, customerId = :customerId, "
                  "sellerId = :sellerId WHERE id = :id");

    query.bindValue(":id", product.getId());
    query.bindValue(":product_name", product.getProductName());
    query.bindValue(":description", product.getDescription());
    query.bindValue(":image", product.getImage());
    query.bindValue(":price", product.getPrice());
    query.bindValue(":rating", product.getRating());
    query.bindValue(":customerId", product.getCustomerId());
    query.bindValue(":sellerId", product.getSellerId());

    if (!query.exec()) {
        qWarning() << "Failed to update product:" << query.lastError().text();
        return false;
    }
    return true;
}

bool Database::deleteProduct(int productId) {
    QSqlQuery query;
    query.prepare("DELETE FROM product WHERE id = :id");
    query.bindValue(":id", productId);

    if (!query.exec()) {
        qWarning() << "Failed to delete product:" << query.lastError().text();
        return false;
    }
    return true;
}

Product Database::getProduct(int productId) {
    Product product;
    QSqlQuery query;
    query.prepare("SELECT * FROM product WHERE id = :id");
    query.bindValue(":id", productId);

    if (query.exec() && query.next()) {
        product.setId(query.value("id").toInt());
        product.setProductName(query.value("product_name").toString());
        product.setDescription(query.value("description").toString());
        product.setImage(query.value("image").toByteArray());
        product.setPrice(query.value("price").toInt());
        product.setRating(query.value("rating").toInt());
        product.setCustomerId(query.value("customerId").toInt());
        product.setSellerId(query.value("sellerId").toInt());
    } else {
        qWarning() << "Failed to fetch product:" << query.lastError().text();
    }
    return product;
}

QList<Product> Database::getAllProducts(int sellerId, int customerId) {
    QList<Product> productList;
    QSqlQuery query;

    // Build the SQL query
    QString sqlQuery = "SELECT * FROM product WHERE customerId = -1";

    // Add sellerId filter if provided
    if (sellerId != -1) {
        sqlQuery += " AND sellerId = :sellerId";
    }

    // Add customerId filter if provided (only for customers, if needed)
    if (customerId != -1) {
        sqlQuery += " AND customerId = :customerId";
    }

    query.prepare(sqlQuery);

    // Bind parameters if they are provided
    if (sellerId != -1) {
        query.bindValue(":sellerId", sellerId);
    }

    if (customerId != -1) {
        query.bindValue(":customerId", customerId);
    }

    // Execute the query
    if (query.exec()) {
        while (query.next()) {
            Product product;
            product.setId(query.value("id").toInt());
            product.setProductName(query.value("product_name").toString());
            product.setDescription(query.value("description").toString());
            product.setImage(query.value("image").toByteArray());
            product.setPrice(query.value("price").toInt());
            product.setRating(query.value("rating").toInt());
            product.setCustomerId(query.value("customerId").toInt());
            product.setSellerId(query.value("sellerId").toInt());
            product.setCategory(query.value("category").toString());
            productList.append(product);
        }
    } else {
        qWarning() << "Failed to fetch products:" << query.lastError().text();
    }
    return productList;
}

bool Database::buyProductsInCart(int customerId, const QList<Product>& cartProducts) {
    QSqlQuery query;
    query.prepare("UPDATE product SET customerId = :customerId WHERE id = :id AND (customerId = -1)");

    for (const Product& product : cartProducts) {
        query.bindValue(":customerId", customerId);
        query.bindValue(":id", product.getId());

        if (!query.exec()) {
            qWarning() << "Failed to buy product:" << query.lastError().text();
            return false;
        }
    }

    return true;
}

QList<Product> Database::getPurchasedProducts(int id, bool isCustomer)
{
    QSqlQuery query;
    QList<Product> productList;

    if (isCustomer) {
        query.prepare("SELECT * FROM product WHERE customerId = :customerId");
        query.bindValue(":customerId", id);
    } else {
        query.prepare("SELECT * FROM product WHERE sellerId = :sellerId AND customerId <> -1");
        query.bindValue(":sellerId", id);
    }

    if (query.exec()) {
        while (query.next()) {
            Product product;
            product.setId(query.value("id").toInt());
            product.setProductName(query.value("product_name").toString());
            product.setDescription(query.value("description").toString());
            product.setImage(query.value("image").toByteArray());
            product.setPrice(query.value("price").toInt());
            product.setRating(query.value("rating").toInt());
            product.setCustomerId(query.value("customerId").toInt());
            product.setSellerId(query.value("sellerId").toInt());
            product.setCategory(query.value("category").toString());
            productList.append(product);
        }
    } else {
        qWarning() << "Failed to fetch purchased products:" << query.lastError().text();
    }

    return productList;
}


bool Database::rateProduct(int productId, int rating)
{
    QSqlQuery query;

    query.prepare("UPDATE product SET rating = :rating WHERE id = :id");
    query.bindValue(":rating", rating);
    query.bindValue(":id", productId);

    if (query.exec()) {
        qDebug() << "Product rating updated!";
        return true;
    } else {
        qWarning() << "Failed to update product rating:" << query.lastError().text();
        return false;
    }
}

QMap<int, int> Database::getSellerRatings(int sellerId) {
    QSqlQuery query;
    QMap<int, int> ratingsMap;

    query.prepare("SELECT rating, COUNT(*) AS count FROM product WHERE sellerId = :sellerId AND rating > 0 GROUP BY rating");
    query.bindValue(":sellerId", sellerId);

    if (query.exec()) {
        while (query.next()) {
            int rating = query.value("rating").toInt();
            int count = query.value("count").toInt();
            ratingsMap.insert(rating, count);
        }
    } else {
        qWarning() << "Failed to fetch seller ratings:" << query.lastError().text();
    }

    return ratingsMap;
}

QMap<QString, int> Database::getProductOverview(int sellerId) {
    QSqlQuery query;
    QMap<QString, int> productOverview;

    query.prepare("SELECT COUNT(*) FROM product WHERE sellerId = :sellerId");
    query.bindValue(":sellerId", sellerId);

    if (query.exec() && query.next()) {
        productOverview["Total"] = query.value(0).toInt();
    } else {
        qWarning() << "Failed to fetch total products:" << query.lastError().text();
        productOverview["Total"] = 0;
    }

    query.prepare("SELECT COUNT(*) FROM product WHERE sellerId = :sellerId AND customerId <> -1");
    query.bindValue(":sellerId", sellerId);

    if (query.exec() && query.next()) {
        productOverview["Sold"] = query.value(0).toInt();
    } else {
        qWarning() << "Failed to fetch sold products:" << query.lastError().text();
        productOverview["Sold"] = 0;
    }
    query.prepare("SELECT COUNT(*) FROM product WHERE sellerId = :sellerId AND customerId = -1");
    query.bindValue(":sellerId", sellerId);

    if (query.exec() && query.next()) {
        productOverview["Available"] = query.value(0).toInt();
    } else {
        qWarning() << "Failed to fetch available products:" << query.lastError().text();
        productOverview["Available"] = 0;
    }

    return productOverview;
}
