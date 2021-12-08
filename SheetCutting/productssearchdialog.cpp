#include "productssearchdialog.h"
#include "ui_productssearchdialog.h"

ProductsSearchDialog::ProductsSearchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProductsSearchDialog)
{
    ui->setupUi(this);
    ui->productsSearchComboBox->addItem("По количеству", 1);
    ui->productsSearchComboBox->addItem("По ширине", 2);
    ui->productsSearchComboBox->addItem("По высоте", 3);
}

ProductsSearchDialog::~ProductsSearchDialog()
{
    delete ui;
}

int ProductsSearchDialog::getSearchHeight() const
{
    return searchHeight;
}

int ProductsSearchDialog::getSearchWidth() const
{
    return searchWidth;
}

int ProductsSearchDialog::getSearchAmount() const
{
    return searchAmount;
}

void ProductsSearchDialog::on_productsSearchButtonBox_accepted()
{
    if(ui->productsSearchComboBox->currentIndex() == 0) {
        searchAmount = ui->productsSearchLineEdit->text().toInt();
        searchWidth = 0;
        searchHeight = 0;
        this->close();
    }
    if(ui->productsSearchComboBox->currentIndex() == 1) {
        searchWidth = ui->productsSearchLineEdit->text().toInt();
        searchAmount = 0;
        searchHeight = 0;
        this->close();
    }
    if(ui->productsSearchComboBox->currentIndex() == 2) {
        searchHeight = ui->productsSearchLineEdit->text().toInt();
        searchAmount = 0;
        searchWidth = 0;
        this->close();
    }
}


void ProductsSearchDialog::on_productsSearchButtonBox_rejected()
{
    searchAmount = 0;
    searchWidth = 0;
    searchHeight = 0;
    this->close();
}


