#ifndef CurrentUser_H
#define CurrentUser_H

#include "user.h"
#include "product.h"
#include "Database.h"

class CurrentUser {
public:
    static CurrentUser& getInstance() {
        static CurrentUser instance;
        return instance;
    }

    void setUser(const User& user) {
        m_user = user;
        m_isLoggedIn = true;
        setProducts();
    }

    void setProducts() {
        if (m_user.getUserType() == "seller") {
            qDebug() << "Fetching products for seller with ID:" << m_user.getId();
            m_myProducts = Database::getInstance().getAllProducts(m_user.getId(), -1);
        } else if (m_user.getUserType() == "customer") {
            qDebug() << "Fetching products for customer with ID:" << m_user.getId();
            m_myProducts = Database::getInstance().getAllProducts(-1, m_user.getId());
        }
    }

    QList<Product> getProducts() const {
        return m_myProducts;
    }

    const User& getUser() const {
        return m_user;
    }

    bool isLoggedIn() const {
        return m_isLoggedIn;
    }

    void logout() {
        m_isLoggedIn = false;
        m_user = User(-1, "", "", "", "", ""); // Reset user data
        m_myProducts.clear();                // Clear the product list
    }

private:
    CurrentUser() : m_user(-1, "", "", "", "", ""), m_isLoggedIn(false) {}
    ~CurrentUser() {}

    // Delete copy constructor and assignment operator to enforce singleton
    CurrentUser(const CurrentUser&) = delete;
    CurrentUser& operator=(const CurrentUser&) = delete;

    User m_user;
    bool m_isLoggedIn;
    QList<Product> m_myProducts;
};

#endif // CurrentUser_H
