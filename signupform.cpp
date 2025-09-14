#include "signupform.h"
#include "signupform.h"
#include "ui_signupform.h"
#include "Database.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QRegularExpression>
#include <QFileDialog>
#include <QBuffer>

SignupForm::SignupForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SignupForm)
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

    QSqlDatabase db = Database::getInstance().getConnection();

    if (!db.isOpen()) {
        qDebug() << "Database is not connected: " << db.lastError().text();
    } else {
        qDebug() << "Database is already connected.";
    }


    ui->userTypeField->addItem("customer");
    ui->userTypeField->addItem("seller");

    connect(ui->userTypeField, &QComboBox::currentIndexChanged, this, &SignupForm::on_userTypeField_currentIndexChanged);
    on_userTypeField_currentIndexChanged(ui->userTypeField->currentIndex());
}

SignupForm::~SignupForm()
{
    delete ui;
}

void SignupForm::on_backButton_clicked()
{
    ui->nameField->clear();
    ui->brandNameField->clear();
    ui->emailField->clear();
    ui->passwordField->clear();
    ui->logoPreviewLabel->clear();
    emit backToMain();
}

void SignupForm::on_signupButton_clicked()
{
    QString name = ui->nameField->text().trimmed();
    QString email = ui->emailField->text().trimmed();
    QString password = ui->passwordField->text().trimmed();
    QString userType = ui->userTypeField->currentText().trimmed();
    QString brandName = ui->brandNameField->text().trimmed();

    // Input validation
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Name field cannot be empty.");
        return;
    }

    if (email.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Email field cannot be empty.");
        return;
    }

    // Check for valid email format
    QRegularExpression emailRegex("^[\\w\\.\\-]+@[\\w\\-]+\\.[a-zA-Z]{2,4}$");
    if (!(emailRegex.match(email).hasMatch())) {
        QMessageBox::warning(this, "Input Error", "Invalid email format.");
        return;
    }

    if (password.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Password field cannot be empty.");
        return;
    }

    if (userType.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please select a user type.");
        return;
    }

    if (userType == "seller" && brandName.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Brand name cannot be empty for sellers.");
        return;
    }

    QByteArray logoData;
    if (userType == "seller") {
        // Get the QPixmap object directly from the QLabel
        QPixmap pixmap = ui->logoPreviewLabel->pixmap();

        // Check if the pixmap is null
        if (pixmap.isNull()) {
            QMessageBox::warning(this, "Input Error", "Logo must be provided for sellers.");
            return;
        }

        // Convert the pixmap to binary data
        QBuffer buffer(&logoData);
        buffer.open(QIODevice::WriteOnly);
        pixmap.save(&buffer, "PNG");
    }


    // Prepare the SQL query
    QSqlQuery query;
    query.prepare("INSERT INTO user (name, email, password, usertype, brandname, logo) "
                  "VALUES (:name, :email, :password, :usertype, :brandname, :logo)");
    query.bindValue(":name", name);
    query.bindValue(":email", email);
    query.bindValue(":password", password);
    query.bindValue(":usertype", userType);
    query.bindValue(":brandname", brandName.isEmpty() ? QVariant(QVariant::String) : brandName);
    query.bindValue(":logo", logoData);

    if (query.exec()) {
        QMessageBox::information(this, "Success", "User registered successfully!");
    } else {
        QMessageBox::critical(this, "Error", "Failed to register user: " + query.lastError().text());
    }

    ui->nameField->clear();
    ui->brandNameField->clear();
    ui->emailField->clear();
    ui->passwordField->clear();
    ui->logoPreviewLabel->clear();
}

void SignupForm::on_logoButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "Select Logo",
                                                    "",
                                                    "Images (*.png *.jpg *.jpeg *.bmp)");

    if (filePath.isEmpty()) {
        return;
    }

    // Load the selected image
    QPixmap pixmap(filePath);
    if (pixmap.isNull()) {
        QMessageBox::warning(this, "Invalid Image", "The selected file is not a valid image.");
        return;
    }

    // Display the image in the QLabel
    ui->logoPreviewLabel->setPixmap(pixmap.scaled(ui->logoPreviewLabel->size(),
                                                  Qt::KeepAspectRatio,
                                                  Qt::SmoothTransformation));
}


void SignupForm::on_userTypeField_currentIndexChanged(int index)
{
    QString userType = ui->userTypeField->itemText(index);

    if (userType == "seller") {
        ui->brandlbl->setVisible(true);
        ui->brandNameField->setVisible(true);
        ui->logoButton->setVisible(true);
        ui->logoPreviewLabel->setVisible(true);
    } else {
        ui->brandlbl->setVisible(false);
        ui->brandNameField->setVisible(false);
        ui->logoButton->setVisible(false);
        ui->logoPreviewLabel->setVisible(false);
    }
}
