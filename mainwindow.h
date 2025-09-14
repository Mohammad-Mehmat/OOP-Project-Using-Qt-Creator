#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void openLoginForm();
    void openSignupForm();

private slots:
    void on_openSignup_clicked();

    void on_openLogin_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
