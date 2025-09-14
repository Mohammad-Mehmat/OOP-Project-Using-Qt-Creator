QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += charts

CONFIG += c++17


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Database.cpp \
    addproductdialog.cpp \
    analyticspage.cpp \
    cart.cpp \
    cartpage.cpp \
    currentuser.cpp \
    electrosphere.cpp \
    form.cpp \
    homepage.cpp \
    loginform.cpp \
    main.cpp \
    mainwindow.cpp \
    product.cpp \
    productcard.cpp \
    purchasespage.cpp \
    signupform.cpp \
    storepage.cpp \
    user.cpp

HEADERS += \
    Database.h \
    addproductdialog.h \
    analyticspage.h \
    cart.h \
    cartpage.h \
    currentuser.h \
    electrosphere.h \
    form.h \
    homepage.h \
    loginform.h \
    mainwindow.h \
    product.h \
    productcard.h \
    purchasespage.h \
    signupform.h \
    storepage.h \
    user.h

FORMS += \
    addproductdialog.ui \
    analyticspage.ui \
    cartpage.ui \
    electrosphere.ui \
    electrosphere.ui \
    form.ui \
    homepage.ui \
    loginform.ui \
    mainwindow.ui \
    productcard.ui \
    purchasespage.ui \
    signupform.ui \
    storepage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ProjectResources.qrc
