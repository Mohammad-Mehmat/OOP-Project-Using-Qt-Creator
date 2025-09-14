#ifndef STOREPAGE_H
#define STOREPAGE_H

#include <QWidget>
#include <QLayout>
#include <QPushButton>

namespace Ui {
class StorePage;
}

class StorePage : public QWidget
{
    Q_OBJECT

public:
    explicit StorePage(QWidget *parent = nullptr);
    ~StorePage();

private:
    Ui::StorePage *ui;
    QVBoxLayout *productGridLayout;
    QPushButton *addProductButton;
public:
    void updateProductGrid();
    void clearLayout(QLayout*);

private slots:
    void onAddProductButtonClicked();

};

#endif // STOREPAGE_H
