#ifndef PRODUCTSINPUTDIALOG_H
#define PRODUCTSINPUTDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class ProductsInputDialog;
}

class ProductsInputDialog : public QDialog
{
    Q_OBJECT    
private:
    Ui::ProductsInputDialog *ui;
    int productsAmount;
    int productsWidth;
    int productsHeight;


public:
    explicit ProductsInputDialog(QWidget *parent = nullptr);

    ~ProductsInputDialog();

    int getProductsAmount() const;

    int getProductsWidth() const;

    int getProductsHeight() const;

private slots:
    void on_inputDialogButtonBox_accepted();

    void on_inputDialogButtonBox_rejected();

};

#endif // PRODUCTSINPUTDIALOG_H
