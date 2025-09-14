#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>
#include <QByteArray>

class Product {
public:

    Product(int id = -1, const QString &productName = "", const QString &description = "", const QString &category = "",
            const QByteArray &image = QByteArray(), int price = 0, int rating = -1,
            int customerId = -1, int sellerId = -1);

    // Getter and Setter methods
    int getId() const;
    void setId(int id);

    QString getProductName() const;
    void setProductName(const QString &productName);

    QString getDescription() const;
    void setDescription(const QString &description);

    QString getCategory() const;
    void setCategory(const QString &category);

    QByteArray getImage() const;
    void setImage(const QByteArray &image);

    int getPrice() const;
    void setPrice(int price);

    int getRating() const;
    void setRating(int rating);

    int getCustomerId() const;
    void setCustomerId(int customerId);

    int getSellerId() const;
    void setSellerId(int sellerId);

private:
    int m_id;
    QString m_productName;
    QString m_description;
    QString m_category;
    QByteArray m_image;
    int m_price;
    int m_rating;
    int m_customerId;
    int m_sellerId;
};

#endif // PRODUCT_H
