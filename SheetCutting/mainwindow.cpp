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
    switch (keyEvent->key()) {
    case Qt::Key_1:
        if (keyEvent->modifiers() & Qt::ControlModifier) {
            this->makeFirstExample();
        }
    case Qt::Key_2:
        if (keyEvent->modifiers() & Qt::ControlModifier) {
            this->makeSecondExample();
        }
    default:
        QWidget::keyPressEvent(keyEvent);
    }
}

void MainWindow::paintEvent(QPaintEvent *paintEvent)
{
    int centerX = ui->mainGraphicsView->width() / 2;
    int centerY = ui->mainGraphicsView->height() / 2;
    if(sheetPaint) {        
        mainScene->clear();
        mainScene->addRect(centerX - cutter.getSheetWidth() / 2, centerY - cutter.getSheetHeight() / 2, cutter.getSheetWidth(), cutter.getSheetHeight());
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
    if (amount != NULL) {
        ui->productsTableWidget->insertRow(ui->productsTableWidget->rowCount());
        ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number(productsWidth)));
        ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(productsHeight)));
        ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(amount)));
        cutter.addProducts(productsWidth, productsHeight, amount);
        //productsTableWidgetUpdate();
    }    
}


void MainWindow::on_productsTableFindPushButton_clicked()
{
    ProductsInputDialog inputDialog(nullptr, "Поиск изделий", "Введите параметры для поиска", true);
    inputDialog.exec();
    int rowIndex = 0;
    if(inputDialog.getProductsAmount()!= NULL) {
        int amount = inputDialog.getProductsAmount();
        QList<QTableWidgetItem*> searchList = ui->productsTableWidget->findItems(QString::number(amount), Qt::MatchExactly);
        if(!searchList.isEmpty()) {
            rowIndex = searchList.at(0)->row();
            ui->productsTableWidget->selectRow(rowIndex);
        }
    }
    else if(inputDialog.getProductsWidth()!= NULL) {
        int productsWidth = inputDialog.getProductsWidth();
        QList<QTableWidgetItem*> searchList = ui->productsTableWidget->findItems(QString::number(productsWidth), Qt::MatchExactly);
        if(!searchList.isEmpty()) {
            rowIndex = searchList.at(0)->row();
            ui->productsTableWidget->selectRow(rowIndex);
        }
    }
    else if(inputDialog.getProductsHeight()!= NULL) {
        int productsHeight = inputDialog.getProductsHeight();
        QList<QTableWidgetItem*> searchList = ui->productsTableWidget->findItems(QString::number(productsHeight), Qt::MatchExactly);
        if(!searchList.isEmpty()) {
            rowIndex = searchList.at(0)->row();
            ui->productsTableWidget->selectRow(rowIndex);
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

void MainWindow::productsTableWidgetUpdate()
{
    int amount = 0;
    int width = 0;
    int height = 0;
    QList<QRect> prodList = cutter.getProducts();
    width = prodList.begin()->width();
    height = prodList.begin()->height();
    ui->productsTableWidget->setRowCount(0);
    foreach(QRect prod, prodList) {
        if (prod.width() == width && prod.height() == height) {
            amount++;
        } else {
            ui->productsTableWidget->insertRow(ui->productsTableWidget->rowCount());
            ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number(width)));
            ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(height)));
            ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(amount)));
            width = prod.width();
            height = prod.height();
            amount = 0;
        }
    }
}

void MainWindow::on_productsTableWidget_cellChanged(int row, int column)
{
    if (editMode) {
        cutter.deleteProducts(previousWidth, previousHeight, previousAmount);
        int amount = ui->productsTableWidget->item(row, 2)->text().toInt();
        int productsWidth = ui->productsTableWidget->item(row, 0)->text().toInt();
        int productsHeight = ui->productsTableWidget->item(row, 1)->text().toInt();
        cutter.addProducts(productsWidth, productsHeight, amount);
    }
}

void MainWindow::on_productsTableDeletePushButton_clicked()
{
    int rowIndex = ui->productsTableWidget->currentRow();
    int amount = ui->productsTableWidget->item(rowIndex, 2)->text().toInt();
    int productsWidth = ui->productsTableWidget->item(rowIndex, 0)->text().toInt();
    int productsHeight = ui->productsTableWidget->item(rowIndex, 1)->text().toInt();
    ui->productsTableWidget->removeRow(rowIndex);
    cutter.deleteProducts(productsWidth, productsHeight, amount);
    //productsTableWidgetUpdate();
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

void MainWindow::makeFirstExample()
{
    int rowOffset = 1;
    //3 изделия 200х100
    ui->productsTableWidget->insertRow(ui->productsTableWidget->rowCount());
    ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 0, new QTableWidgetItem(QString::number(200)));
    ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 1, new QTableWidgetItem(QString::number(100)));
    ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 2, new QTableWidgetItem(QString::number(3)));
    cutter.addProducts(200, 100, 3);
    //2 изделия 100х70
    ui->productsTableWidget->insertRow(ui->productsTableWidget->rowCount());
    ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 0, new QTableWidgetItem(QString("100")));
    ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 1, new QTableWidgetItem(QString("70")));
    ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 2, new QTableWidgetItem(QString("2")));
    cutter.addProducts(70, 100, 2);
    //3 изделия 70х70
    ui->productsTableWidget->insertRow(ui->productsTableWidget->rowCount());
    ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 0, new QTableWidgetItem(QString("70")));
    ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 1, new QTableWidgetItem(QString("70")));
    ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 2, new QTableWidgetItem(QString("3")));
    cutter.addProducts(70, 70, 3);

}

void MainWindow::makeSecondExample()
{
    int rowOffset = 1;
    //3 изделия 100х200
    ui->productsTableWidget->insertRow(ui->productsTableWidget->rowCount());
    ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 0, new QTableWidgetItem(QString::number(200)));
    ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 1, new QTableWidgetItem(QString::number(100)));
    ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 2, new QTableWidgetItem(QString::number(3)));
    cutter.addProducts(200, 100, 3);
    //3 изделия 70х100
    ui->productsTableWidget->insertRow(ui->productsTableWidget->rowCount());
    ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 0, new QTableWidgetItem(QString("70")));
    ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 1, new QTableWidgetItem(QString("100")));
    ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 2, new QTableWidgetItem(QString("3")));
    cutter.addProducts(70, 100, 2);
    //3 изделия 70х70
    ui->productsTableWidget->insertRow(ui->productsTableWidget->rowCount());
    ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 0, new QTableWidgetItem(QString("70")));
    ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 1, new QTableWidgetItem(QString("70")));
    ui->productsTableWidget->setItem(ui->productsTableWidget->rowCount() - rowOffset, 2, new QTableWidgetItem(QString("3")));
    cutter.addProducts(70, 70, 3);
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

