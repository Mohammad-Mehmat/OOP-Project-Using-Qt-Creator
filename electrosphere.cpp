#include "electrosphere.h"
#include "ui_electrosphere.h"

#include "currentuser.h"
#include "homepage.h"
#include "cartpage.h"
#include "purchasespage.h"
#include "storepage.h"
#include "analyticspage.h"
#include "cart.h"

#include <QPushButton>
#include <QDebug>


electrosphere::electrosphere(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::electrosphere)
{
    ui->setupUi(this);
    qDebug() << "Electrosphere Initialized";

    connect(this, &electrosphere::userTypeInitialized, this, &electrosphere::setupNavigationBar);
}

electrosphere::~electrosphere() {
    delete ui;
}

void electrosphere::addNavButton(const QString &label, int pageIndex) {

    while (ui->stackedWidget->count() > 0) {
        QWidget *widget = ui->stackedWidget->widget(0);
        ui->stackedWidget->removeWidget(widget);
        delete widget;
    }
    ui->stackedWidget->addWidget(new HomePage(this));
    ui->stackedWidget->addWidget(new CartPage(this));
    ui->stackedWidget->addWidget(new PurchasesPage(this));
    ui->stackedWidget->addWidget(new StorePage(this));
    ui->stackedWidget->addWidget(new AnalyticsPage(this));
    ui->stackedWidget->setCurrentIndex(0);

    QPushButton *button = new QPushButton(label);
    connect(button, &QPushButton::clicked, [this, pageIndex]() {
        switchPage(pageIndex);
    });
    button->setStyleSheet(
        "QPushButton {"
        "   font-family: Arial, sans-serif;"
        "   font-size: 14px;"
        "   background-color: #0b5572;"
        "   color: white;"
        "   border: 1px solid #0b5572;"
        "   padding: 10px;"
        "   margin: 5px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #82bfc7;"
        "   color: white;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #0b5572;"
        "   color: white;"
        "   border: 1px solid #82bfc7;"
        "}"
    );
    ui->navBarLayout->addWidget(button);
}

void electrosphere::setupNavigationBar() {
    QLayoutItem *item;
    while ((item = ui->navBarLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    addNavButton("Home", 0);
    QString userType = CurrentUser::getInstance().getUser().getUserType();

    if (userType == "customer") {
        addNavButton("Cart", 1);
        addNavButton("Purchases", 2);
    } else if (userType == "seller") {
        addNavButton("My Store", 3);
        addNavButton("Analytics", 4);
    }

    StorePage* storePage = qobject_cast<StorePage*>(ui->stackedWidget->widget(3));
    if (storePage) {
        storePage->updateProductGrid();
    }

    HomePage* homePage = qobject_cast<HomePage*>(ui->stackedWidget->widget(0));
    if (homePage) {
        homePage->updateProductGrid();
    }

    CartPage* cartPage = qobject_cast<CartPage*>(ui->stackedWidget->widget(1));
    if (cartPage) {
        cartPage->updateCart();
    }

    PurchasesPage* purchasesPage = qobject_cast<PurchasesPage*>(ui->stackedWidget->widget(2));
    if (purchasesPage) {
        purchasesPage->populatePurchasesTable();
    }

    AnalyticsPage* analyticsPage = qobject_cast<AnalyticsPage*>(ui->stackedWidget->widget(4));
    if (analyticsPage) {
        analyticsPage->updateAnalytics();
    }

    QPushButton *logoutButton = new QPushButton("Logout");
    logoutButton->setStyleSheet(
        "QPushButton {"
        "   font-family: Arial, sans-serif;"
        "   font-size: 14px;"
        "   background-color: #0b5572;"
        "   color: white;"
        "   border: 1px solid #0b5572;"
        "   padding: 10px;"
        "   margin: 5px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #82bfc7;"
        "   color: white;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #0b5572;"
        "   color: white;"
        "   border: 1px solid #82bfc7;"
        "}"
    );
    connect(logoutButton, &QPushButton::clicked, [this]() {
        CurrentUser::getInstance().logout();
        Cart::getInstance().clearCart();
        emit logout();
    });
    ui->navBarLayout->addWidget(logoutButton);
}

void electrosphere::switchPage(int index) {
    if (index >= 0 && index < ui->stackedWidget->count()) {
        if (index == 0){
            HomePage* homePage = qobject_cast<HomePage*>(ui->stackedWidget->widget(0));
            if (homePage) {
                homePage->updateProductGrid();
            }
        }

        if (index == 1){
            CartPage* cartPage = qobject_cast<CartPage*>(ui->stackedWidget->widget(1));
            if (cartPage) {
                cartPage->updateCart();
            }
        }
        if (index == 2){
            PurchasesPage* purchasesPage = qobject_cast<PurchasesPage*>(ui->stackedWidget->widget(2));
            if (purchasesPage) {
                purchasesPage->populatePurchasesTable();
            }
        }

        if (index == 3) {
            StorePage* storePage = qobject_cast<StorePage*>(ui->stackedWidget->widget(3));
            if (storePage) {
                storePage->updateProductGrid();
            }
        }

        if (index == 4) {
            AnalyticsPage* analyticsPage = qobject_cast<AnalyticsPage*>(ui->stackedWidget->widget(4));
            if (analyticsPage) {
                analyticsPage->updateAnalytics();
            }
        }

        ui->stackedWidget->setCurrentIndex(index);
        qDebug() << "Switched to page index:" << index;
    } else {
        qWarning() << "Invalid page index: " << index;
    }
}

void electrosphere::onUserLogin() {
    CurrentUser::getInstance().getUser();
    emit userTypeInitialized();
}

void electrosphere::clearWidgetsAndNavbar() {
    while (ui->stackedWidget->count() > 0) {
        QWidget *widget = ui->stackedWidget->widget(0);
        ui->stackedWidget->removeWidget(widget);
        delete widget;
    }

    QLayoutItem *item;
    while ((item = ui->navBarLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
}
