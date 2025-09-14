#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

namespace Ui {
class LoginForm;
}

class LoginForm : public QWidget
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = nullptr);
    ~LoginForm();

signals:
    void loginSuccessful();
    void backToMain();

private slots:
    void on_loginButton_clicked();
    void on_backButton_clicked();

private:
    Ui::LoginForm *ui;
    QLineEdit* passwordFieldVar;
    QPushButton* toggleButton;
};

#endif // LOGINFORM_H
