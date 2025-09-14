#ifndef ELECTROSPHERE_H
#define ELECTROSPHERE_H

#include <QStackedWidget>


namespace Ui {
class electrosphere;
}

class electrosphere : public QWidget
{
    Q_OBJECT

public:
    explicit electrosphere(QWidget *parent = nullptr);
    ~electrosphere();

signals:
    void logout();
    void userTypeInitialized();

private:
    Ui::electrosphere *ui;

    void setupNavigationBar();
    void switchPage(int index);
public:
    void onUserLogin();
    void addNavButton(const QString&, int);
    void clearWidgetsAndNavbar();
    void setupPages();
};

#endif // ELECTROSPHERE_H
