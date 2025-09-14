#include "product.h"

Product::Product(int id, const QString &productName, const QString &description, const QString &category,
                 const QByteArray &image, int price, int rating, int customerId, int sellerId)
    : m_id(id), m_productName(productName), m_description(description), m_category(category) ,m_image(image),
    m_price(price), m_rating(rating), m_customerId(customerId), m_sellerId(sellerId) {}

int Product::getId() const { return m_id; }
void Product::setId(int id) { m_id = id; }

QString Product::getProductName() const { return m_productName; }
void Product::setProductName(const QString &productName) { m_productName = productName; }

QString Product::getDescription() const { return m_description; }
void Product::setDescription(const QString &description) { m_description = description; }

QString Product::getCategory() const { return m_category; }
void Product::setCategory(const QString &category) { m_category = category; }

QByteArray Product::getImage() const { return m_image; }
void Product::setImage(const QByteArray &image) { m_image = image; }

int Product::getPrice() const { return m_price; }
void Product::setPrice(int price) { m_price = price; }

int Product::getRating() const { return m_rating; }
void Product::setRating(int rating) { m_rating = rating; }

int Product::getCustomerId() const { return m_customerId; }
void Product::setCustomerId(int customerId) { m_customerId = customerId; }

int Product::getSellerId() const { return m_sellerId; }
void Product::setSellerId(int sellerId) { m_sellerId = sellerId; }
