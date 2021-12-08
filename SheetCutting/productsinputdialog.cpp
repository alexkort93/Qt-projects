#include "productsinputdialog.h"
#include "ui_productsinputdialog.h"


ProductsInputDialog::ProductsInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProductsInputDialog)
{
    ui->setupUi(this);
}

int ProductsInputDialog::getProductsAmount() const
{
    return productsAmount;
}

int ProductsInputDialog::getProductsWidth() const
{
    return productsWidth;
}

int ProductsInputDialog::getProductsHeight() const
{
    return productsHeight;
}

ProductsInputDialog::~ProductsInputDialog()
{
    delete ui;
}

void ProductsInputDialog::on_inputDialogButtonBox_accepted()
{
    if (!ui->productsAmountLineEdit->text().isEmpty()) {
        productsAmount = ui->productsAmountLineEdit->text().toInt();
    } else {
        QMessageBox::warning(this, "Внимание","Поле ввода количества изделий пустое!");
    }

    if (!ui->productsWidthLineEdit->text().isEmpty()) {
        productsWidth = ui->productsWidthLineEdit->text().toInt();
    } else {
        QMessageBox::warning(this, "Внимание","Поле ввода ширины изделий пустое!");
    }

    if (!ui->productsHeightLineEdit->text().isEmpty()) {
        productsHeight = ui->productsHeightLineEdit->text().toInt();
    } else {
        QMessageBox::warning(this, "Внимание","Поле ввода высоты изделий пустое!");
    }
    if (!ui->productsAmountLineEdit->text().isEmpty() && !ui->productsWidthLineEdit->text().isEmpty() && !ui->productsHeightLineEdit->text().isEmpty()) {
        this->close();
    }
}


void ProductsInputDialog::on_inputDialogButtonBox_rejected()
{
    productsAmount = 0;
    productsWidth = 0;
    productsHeight = 0;
    this->close();
}
