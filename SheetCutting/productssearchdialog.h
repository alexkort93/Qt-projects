#ifndef PRODUCTSSEARCHDIALOG_H
#define PRODUCTSSEARCHDIALOG_H

#include <QDialog>

namespace Ui {
class ProductsSearchDialog;
}

class ProductsSearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProductsSearchDialog(QWidget *parent = nullptr);
    ~ProductsSearchDialog();

    int getSearchAmount() const;

    int getSearchWidth() const;

    int getSearchHeight() const;

private slots:
    void on_productsSearchButtonBox_accepted();

    void on_productsSearchButtonBox_rejected();

private:
    Ui::ProductsSearchDialog *ui;
    int searchAmount;
    int searchWidth;
    int searchHeight;
};

#endif // PRODUCTSSEARCHDIALOG_H
