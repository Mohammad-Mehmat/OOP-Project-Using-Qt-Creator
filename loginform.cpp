#include "loginform.h"
#include "ui_loginform.h"

#include "user.h"
#include "currentuser.h"
#include "Database.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

LoginForm::LoginForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginForm)
{
    ui->setupUi(this);
    passwordFieldVar = ui->passwordField;
    passwordFieldVar->setEchoMode(QLineEdit::Password);

    toggleButton = ui->toggle;
    QIcon eyeIcon(":resources/eye_closed.png");
    toggleButton->setIcon(eyeIcon);
    toggleButton->setText("");

    connect(toggleButton, &QPushButton::clicked, this, [this]() {
        if (passwordFieldVar->echoMode() == QLineEdit::Password) {
            passwordFieldVar->setEchoMode(QLineEdit::Normal);
            QIcon eyeIcon(":resources/eye_open.png");
            toggleButton->setIcon(eyeIcon);
        } else {
            passwordFieldVar->setEchoMode(QLineEdit::Password);
            QIcon eyeIcon(":resources/eye_closed.png");
            toggleButton->setIcon(eyeIcon);
        }
    });
}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::on_loginButton_clicked()
{
    QString username = ui->nameField->text();
    QString password = ui->passwordField->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Both username and password must be provided.");
        return;
    }

    QSqlDatabase db = Database::getInstance().getConnection();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Database Error", "Failed to connect to the database.");
        return;
    }

    QSqlQuery query(db);
    query.prepare("SELECT name, email, usertype, brandname, logo, id FROM user WHERE name = :name AND password = :password");
    query.bindValue(":name", username);
    query.bindValue(":password", password);

    if (query.exec()) {
        if (query.next()) {
            QString name = query.value(0).toString();
            QString email = query.value(1).toString();
            QString userType = query.value(2).toString();
            QString brandname = query.value(3).toString();
            QByteArray logo = query.value(4).toByteArray();
            int id = query.value(5).toInt();

            User user(id, name, email, userType, brandname, logo);
            CurrentUser::getInstance().setUser(user);

            QMessageBox::information(this, "Login Successful",
                                     "Welcome " + username + " (" + userType + ")");
            emit loginSuccessful();
            this->close();
        } else {
            QMessageBox::warning(this, "Login Failed", "Invalid username or password.");
        }
    } else {
        QMessageBox::critical(this, "Query Error",
                              "Failed to execute login query: " + query.lastError().text());
    }
    ui->nameField->clear();
    ui->passwordField->clear();
}



void LoginForm::on_backButton_clicked()
{
    ui->nameField->clear();
    ui->passwordField->clear();
    emit backToMain();
}

