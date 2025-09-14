#ifndef USER_H
#define USER_H

#include <QString>

class User {
public:
    User(const int id, const QString &name, const QString &email, const QString &userType, const QString &brandName = QString(), const QByteArray &logo = QByteArray());

    int getId() const;
    QString getName() const;
    QString getEmail() const;
    QString getUserType() const;
    QString getBrandName() const;
    QByteArray getLogo() const;

private:
    int id;
    QString name;
    QString email;
    QString userType;
    QString brandName;
    QByteArray logo;
};

#endif // USER_H
