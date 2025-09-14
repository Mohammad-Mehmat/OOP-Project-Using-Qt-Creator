#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openSignup_clicked()
{
    emit openSignupForm(); // Notify login success
    this->close();
}


void MainWindow::on_openLogin_clicked()
{
    emit openLoginForm();
    this->close();
}

