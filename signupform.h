#ifndef SIGNUPFORM_H
#define SIGNUPFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

#include "QtSql/QSqlDatabase"

namespace Ui {
class SignupForm;
}

class SignupForm : public QWidget
{
    Q_OBJECT

public:
    explicit SignupForm(QWidget *parent = nullptr);
    ~SignupForm();

signals:
    void signupSuccessful();
    void backToMain();

private slots:
    void on_backButton_clicked();
    void on_signupButton_clicked();
    void on_logoButton_clicked();
    void on_userTypeField_currentIndexChanged(int index);

private:
    Ui::SignupForm *ui;
    QSqlDatabase DB_Connection;
    QLineEdit* passwordFieldVar;
    QPushButton* toggleButton;
};

#endif // SIGNUPFORM_H
