#include "cart.h"

void Cart::addProduct(const Product& product)
{
    for (const Product& p : products) {
        if (p.getId() == product.getId()) {
            return; // Product with same ID already in cart
        }
    }
    products.append(product);
    totalPrice += product.getPrice();
    emit cartChanged(); // Emit signal when cart changes
}

bool Cart::containsProduct(int productId) const
{
    for (const Product& product : products) {
        if (product.getId() == productId) {
            return true;
        }
    }
    return false;
}

void Cart::removeProduct(int productId)
{
    for (int i = 0; i < products.size(); ++i) {
        if (products.at(i).getId() == productId) {
            totalPrice -= products.at(i).getPrice();
            products.removeAt(i);
            emit cartChanged();
            return;
        }
    }
}

void Cart::clearCart()
{
    products.clear();
    totalPrice = 0.0;
    emit cartChanged(); // Emit signal when cart changes
}
