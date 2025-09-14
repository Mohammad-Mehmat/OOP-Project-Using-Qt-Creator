#include "productcard.h"
#include "ui_productcard.h"
#include "cart.h"
#include <QMessageBox>
#include <QPropertyAnimation>

ProductCard::ProductCard(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProductCard)
{
    ui->setupUi(this);

    // Connect the button click event to a function
    connect(ui->pushButton, &QPushButton::clicked, this, &ProductCard::on_pushButton_clicked);
}

void ProductCard::setProductInfo(Product product, QString userType, bool isHomePage)
{
    this->product = product;
    ui->productNameLabel->setText(product.getProductName());
    ui->productDescriptionLabel->setText(product.getDescription());
    QImage image = QImage::fromData(product.getImage());
    if (!image.isNull()) {
        ui->productImageLabel->setPixmap(QPixmap::fromImage(image));
    }
    ui->productPriceLabel->setText("Price: Rs. " + QString::number(product.getPrice()));
    if(userType == "seller"){
        ui->pushButton->setVisible(false);
    }
    else if(userType == "customer"){
        ui->pushButton->setText("Add to Cart");
    }
}

ProductCard::~ProductCard()
{
    delete ui;
}

void ProductCard::on_pushButton_clicked()
{
    if (ui->pushButton->text() == "Add to Cart") {
        Cart& cart = Cart::getInstance();
        if (!cart.containsProduct(product.getId())) {
            cart.addProduct(product);
        }

        // Create a label to display the message
        QLabel* messageLabel = new QLabel("Product added to cart", this);
        messageLabel->setAlignment(Qt::AlignCenter);
        messageLabel->setStyleSheet(
            "QLabel {"
            "   background-color: rgba(0,0,0,150);"
            "   color: white;"
            "   padding: 10px;"
            "   border-radius: 5px;"
            "   height: 100px;"
            "   width: 100px;"
            "   font-size: 10px;"
            "}"
            );

        messageLabel->setGeometry(((this->width() - messageLabel->width()) / 2) - 25, (this->height() - messageLabel->height()) / 2, 150, 80);
        messageLabel->show();

        QPropertyAnimation* fadeOutAnimation = new QPropertyAnimation(messageLabel, "windowOpacity");
        fadeOutAnimation->setDuration(2000);
        fadeOutAnimation->setStartValue(1.0);
        fadeOutAnimation->setEndValue(0.0);
        fadeOutAnimation->start(QAbstractAnimation::DeleteWhenStopped);

        QObject::connect(fadeOutAnimation, &QPropertyAnimation::finished, messageLabel, &QLabel::deleteLater);
    }
}
