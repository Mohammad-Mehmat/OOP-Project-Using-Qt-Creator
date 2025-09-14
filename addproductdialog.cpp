#include "addproductdialog.h"
#include "ui_addproductdialog.h"

#include "Database.h"
#include "currentuser.h"
#include <QMessageBox>
#include <QPixmap>
#include <QFileDialog>
#include <QBuffer>

AddProductDialog::AddProductDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddProductDialog)
{
    ui->setupUi(this);

    ui->categoryField->addItem("Smartphone");
    ui->categoryField->addItem("Power bank and Charging");
    ui->categoryField->addItem("Gear and Devices");
    ui->categoryField->addItem("Camera and Visual");
    ui->categoryField->addItem("Audio");

    connect(ui->addProductButton, &QPushButton::clicked, this, &AddProductDialog::onSaveButtonClicked);
}

AddProductDialog::~AddProductDialog()
{
    delete ui;
}

void AddProductDialog::onSaveButtonClicked()
{
    // Validate input
    QString productName = ui->productNameField->text();
    QString productDescription = ui->descriptionField->toPlainText();
    QString productCategory = ui->categoryField->currentText();
    int productPrice = ui->priceField->text().toInt();

    QByteArray imageData;
    if (productName.isEmpty() || productDescription.isEmpty() || productPrice <= 0 || productCategory.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill all fields with valid data.");
        return;
    }

    QPixmap pixmap = ui->imagePreviewLabel->pixmap();
    if (pixmap.isNull()) {
        QMessageBox::warning(this, "Input Error", "Product Image must be provided.");
        return;
    }

    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");

    // Create the product object
    Product newProduct;
    newProduct.setProductName(productName);
    newProduct.setDescription(productDescription);
    newProduct.setPrice(productPrice);
    newProduct.setImage(imageData);
    newProduct.setCategory(productCategory);
    newProduct.setSellerId(CurrentUser::getInstance().getUser().getId());

    // Save the product to the database
    bool success = Database::getInstance().insertProduct(newProduct);

    if (success) {
        QMessageBox::information(this, "Success", "Product added successfully.");
    } else {
        QMessageBox::critical(this, "Error", "Failed to add product to the database.");
    }

    this->close();
}

void AddProductDialog::on_imageButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "Select Image",
                                                    "",
                                                    "Images (*.png *.jpg *.jpeg *.bmp *.webp)");

    if (filePath.isEmpty()) {
        return;
    }

    QPixmap pixmap(filePath);
    if (pixmap.isNull()) {
        QMessageBox::warning(this, "Invalid Image", "The selected file is not a valid image.");
        return;
    }

    ui->imagePreviewLabel->setPixmap(pixmap.scaled(ui->imagePreviewLabel->size(),
                                                  Qt::KeepAspectRatio,
                                                  Qt::SmoothTransformation));
}

