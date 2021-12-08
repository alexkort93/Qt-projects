#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qApp->installEventFilter(this);
    ui->cutAlgoritmComboBox->addItem("Алгоритм Бурке вертикальный", 1);
    ui->cutAlgoritmComboBox->addItem("Алгоритм Бурке горизонтальный", 2);
    ui->sheetWidthLineEdit->setText("900");
    ui->sheetHeightLineEdit->setText("300");
    ui->productsTableWidget->setHorizontalHeaderLabels(QStringList() << "Ширина" << "Высота" << "Количество");
    ui->productsTableWidget->setColumnWidth(0, 90);
    ui->productsTableWidget->setColumnWidth(1, 90);
    ui->productsTableWidget->setColumnWidth(2, 82);
    mainScene = new QGraphicsScene();
    ui->mainGraphicsView->setScene(mainScene);
    editMode = false;
    sheetPaint = false;
    prodPaint = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *keyEvent)
{
    if (keyEvent->modifiers() & Qt::ControlModifier) {
        switch (keyEvent->key()) {
        case Qt::Key_1: {
            int rowOffset = 1;
            //3 изделия 200х100
            ui->productsTableWidget->insertRow(ui->productsTableWidget->rowCount());
            ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 0, new QTableWidgetItem(QString("200")));
            ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 1, new QTableWidgetItem(QString("100")));
            ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 2, new QTableWidgetItem(QString("3")));
            cutter.addProducts(200, 100, 3);
            //2 изделия 100х70
            ui->productsTableWidget->insertRow(ui->productsTableWidget->rowCount());
            ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 0, new QTableWidgetItem(QString("100")));
            ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 1, new QTableWidgetItem(QString("70")));
            ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 2, new QTableWidgetItem(QString("2")));
            cutter.addProducts(100, 70, 2);
            //3 изделия 70х70
            ui->productsTableWidget->insertRow(ui->productsTableWidget->rowCount());
            ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 0, new QTableWidgetItem(QString("70")));
            ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 1, new QTableWidgetItem(QString("70")));
            ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 2, new QTableWidgetItem(QString("3")));
            cutter.addProducts(70, 70, 3);
        }
        case Qt::Key_2: {
            int rowOffset = 1;
            //3 изделия 100х200
            ui->productsTableWidget->insertRow(ui->productsTableWidget->rowCount());
            ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 0, new QTableWidgetItem(QString("100")));
            ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 1, new QTableWidgetItem(QString("200")));
            ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 2, new QTableWidgetItem(QString("3")));
            cutter.addProducts(100, 200, 3);
            //3 изделия 70х100
            ui->productsTableWidget->insertRow(ui->productsTableWidget->rowCount());
            ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 0, new QTableWidgetItem(QString("70")));
            ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 1, new QTableWidgetItem(QString("100")));
            ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 2, new QTableWidgetItem(QString("3")));
            cutter.addProducts(70, 100, 3);
            //3 изделия 70х70
            ui->productsTableWidget->insertRow(ui->productsTableWidget->rowCount());
            ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 0, new QTableWidgetItem(QString("70")));
            ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 1, new QTableWidgetItem(QString("70")));
            ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 2, new QTableWidgetItem(QString("3")));
            cutter.addProducts(70, 70, 3);
        }
            //default:
            //QWidget::keyPressEvent(keyEvent);
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *paintEvent)
{
    int centerX = ui->mainGraphicsView->width() / 2;
    int centerY = ui->mainGraphicsView->height() / 2;
    if(sheetPaint) {        
        mainScene->clear();       
        mainScene->addRect(centerX - cutter.getSheetWidth() / 2, centerY - cutter.getSheetHeight() / 2, cutter.getSheetWidth(), cutter.getSheetHeight(),QPen(Qt::black,3));
        sheetPaint = false;
    }
    if(prodPaint) {        
        for (int i = 0; i < resCut.size(); i++) {
            mainScene->addRect(resCut[i].translated(centerX - cutter.getSheetWidth() / 2, centerY - cutter.getSheetHeight() / 2),
                               QPen(Qt::black), QBrush(QColor(255, 200, 100).lighter(120 - 8 * i), Qt::SolidPattern));
            prodPaint = false;            
        }
    }
}

void MainWindow::on_applySheetParametersPushButton_clicked()
{
    if (!ui->sheetWidthLineEdit->text().isEmpty()) {
        int newWidth = ui->sheetWidthLineEdit->text().toInt();
        cutter.setSheetWidth(newWidth);
    } else {
        QMessageBox::warning(this, "Внимание","Поле ввода ширины листа пустое!");
    }
    if (!ui->sheetHeightLineEdit->text().isEmpty()) {
        int newHeight = ui->sheetHeightLineEdit->text().toInt();
        cutter.setSheetHeight(newHeight);
    } else {
        QMessageBox::warning(this, "Внимание","Поле ввода высоты листа пустое!");
    }
    if(!ui->sheetWidthLineEdit->text().isEmpty() && !ui->sheetHeightLineEdit->text().isEmpty()) {
        sheetPaint = true;
        ui->mainGraphicsView->update();
    }
}


void MainWindow::on_productsTableAddPushButton_clicked()
{
    editMode = false;
    ProductsInputDialog inputDialog;    
    inputDialog.exec();
    int amount = inputDialog.getProductsAmount();
    int productsWidth = inputDialog.getProductsWidth();
    int productsHeight = inputDialog.getProductsHeight();
    if (amount > 0) {
        ui->productsTableWidget->insertRow(ui->productsTableWidget->rowCount());
        ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number(productsWidth)));
        ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(productsHeight)));
        ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(amount)));
        cutter.addProducts(productsWidth, productsHeight, amount);        
    }    
}

void MainWindow::on_productsTableFindPushButton_clicked()
{
    ProductsSearchDialog searchDialog;
    searchDialog.exec();
    int lastSearchIndex = 0;
    int rowIndex = 0;
    if(searchDialog.getSearchAmount() > 0) {
        int amount = searchDialog.getSearchAmount();
        QList<QTableWidgetItem*> searchList = ui->productsTableWidget->findItems(QString::number(amount), Qt::MatchExactly);
        if(lastSearchIndex <= searchList.length() && searchList.at(lastSearchIndex)->data(Qt::DisplayRole).toInt() == previousAmount) {
            lastSearchIndex++;
        } else {
            lastSearchIndex = 0;
        }
        if(!searchList.isEmpty()) {
            rowIndex = searchList.at(lastSearchIndex)->row();
            ui->productsTableWidget->selectRow(rowIndex);
            previousAmount = searchList.at(lastSearchIndex)->data(Qt::DisplayRole).toInt();
        }
    }
    if(searchDialog.getSearchWidth() > 0) {
        int productsWidth = searchDialog.getSearchWidth();
        QList<QTableWidgetItem*> searchList = ui->productsTableWidget->findItems(QString::number(productsWidth), Qt::MatchExactly);
        if(lastSearchIndex <= searchList.length() && searchList.at(lastSearchIndex)->data(Qt::DisplayRole).toInt() == previousWidth) {
            lastSearchIndex++;
        } else {
            lastSearchIndex = 0;
        }
        if(!searchList.isEmpty()) {
            rowIndex = searchList.at(lastSearchIndex)->row();
            ui->productsTableWidget->selectRow(rowIndex);
            previousWidth = searchList.at(lastSearchIndex)->data(Qt::DisplayRole).toInt();
        }
    }
    if(searchDialog.getSearchHeight() > 0) {
        int productsHeight = searchDialog.getSearchHeight();
        QList<QTableWidgetItem*> searchList = ui->productsTableWidget->findItems(QString::number(productsHeight), Qt::MatchExactly);
        if(lastSearchIndex <= searchList.length() && searchList.at(lastSearchIndex)->data(Qt::DisplayRole).toInt() == previousHeight) {
            lastSearchIndex++;
        } else {
            lastSearchIndex = 0;
        }
        if(!searchList.isEmpty()) {
            rowIndex = searchList.at(lastSearchIndex)->row();
            ui->productsTableWidget->selectRow(rowIndex);
            previousHeight = searchList.at(lastSearchIndex)->data(Qt::DisplayRole).toInt();
        }
    }
}

void MainWindow::on_productsTableWidget_cellDoubleClicked(int row, int column)
{
    editMode = true;
    previousAmount = ui->productsTableWidget->item(row, 2)->text().toInt();
    previousWidth = ui->productsTableWidget->item(row, 0)->text().toInt();
    previousHeight = ui->productsTableWidget->item(row, 1)->text().toInt();
}

void MainWindow::on_productsTableWidget_cellChanged(int row, int column)
{
    if (editMode) {
        cutter.deleteProducts(previousWidth, previousHeight, previousAmount);
        int amount = ui->productsTableWidget->item(row, 2)->text().toInt();
        int productsWidth = ui->productsTableWidget->item(row, 0)->text().toInt();
        int productsHeight = ui->productsTableWidget->item(row, 1)->text().toInt();
        cutter.addProducts(productsWidth, productsHeight, amount);
        editMode = false;
    }
}

void MainWindow::on_productsTableDeletePushButton_clicked()
{
    int rowIndex = ui->productsTableWidget->currentRow();
    int amount = ui->productsTableWidget->item(rowIndex, 2)->text().toInt();
    int productsWidth = ui->productsTableWidget->item(rowIndex, 0)->text().toInt();    
    int productsHeight = ui->productsTableWidget->item(rowIndex, 1)->text().toInt();
    qDebug() << "Deleting row " << rowIndex;
    ui->productsTableWidget->removeRow(rowIndex);
    cutter.deleteProducts(productsWidth, productsHeight, amount);    
}


void MainWindow::on_cutPushButton_clicked()
{
    if(ui->cutAlgoritmComboBox->currentIndex() == 0) {
        resCut.clear();
        resCut = cutter.cutBurkeVertical();
        sheetPaint = true;
        prodPaint = true;
    }
    if(ui->cutAlgoritmComboBox->currentIndex() == 1) {
        resCut.clear();
        resCut = cutter.cutBurkeHorizontal();
        sheetPaint = true;
        prodPaint = true;
    }
    update();
}

void MainWindow::on_saveCutAction_triggered()
{    
    QString imagePath = QFileDialog::getSaveFileName(this,
            tr("Сохранить схему раскройки"), "",
            tr("Изображение (*.png);;Все файлы (*)"));
    if(!imagePath.isEmpty()) {
        QImage image(mainScene->sceneRect().size().toSize(), QImage::Format_ARGB32); QPainter painter(&image);
        mainScene->render(&painter);
        image.save(imagePath);
    }
}

