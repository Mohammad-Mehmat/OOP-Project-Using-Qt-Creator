#include "mainwindow.h"
#include "loginform.h"
#include "signupform.h"
#include "electrosphere.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mainWindow;
    LoginForm login;
    SignupForm signup;
    electrosphere main;

    QObject::connect(&mainWindow, &MainWindow::openLoginForm, [&]() {
        login.show();
    });

    QObject::connect(&mainWindow, &MainWindow::openSignupForm, [&]() {
        signup.show();
    });

    QObject::connect(&login, &LoginForm::backToMain, [&]() {
        login.close();
        mainWindow.show();
    });

    QObject::connect(&signup, &SignupForm::backToMain, [&]() {
        signup.close();
        mainWindow.show();
    });

    QObject::connect(&login, &LoginForm::loginSuccessful, [&]() {
        login.close();
        main.onUserLogin();
        main.show();
    });

    QObject::connect(&main, &electrosphere::logout, [&]() {
        main.close();
        mainWindow.show();
    });

    mainWindow.show();

    return app.exec();
}
