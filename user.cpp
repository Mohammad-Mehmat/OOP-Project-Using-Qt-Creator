#include "user.h"

User::User(const int id, const QString &name, const QString &email, const QString &userType, const QString &brandName, const QByteArray &logo)
    : id(id), name(name), email(email), userType(userType), brandName(brandName), logo(logo) {}

int User::getId() const {
    return id;
}

QString User::getName() const {
    return name;
}

QString User::getEmail() const {
    return email;
}

QString User::getUserType() const {
    return userType;
}

QString User::getBrandName() const {
    return brandName;
}

QByteArray User::getLogo() const {
    return logo;
}
