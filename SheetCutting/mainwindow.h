#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTextFormat>
#include <QMessageBox>
#include <QTableWidget>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QFileDialog>

#include "cutter.h"
#include "productsinputdialog.h"
#include "productssearchdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;
    Cutter cutter;
    bool editMode;
    bool sheetPaint;
    bool prodPaint;
    int previousAmount;
    int previousWidth;
    int previousHeight;
    int previousSearchAmount;
    int previousSearchWidth;
    int previousSearchHeight;
    QList<QRect> resCut;
    QGraphicsScene *mainScene;

protected:    
    void paintEvent(QPaintEvent *paintEvent);

public:
    MainWindow(QWidget *parent = nullptr);    
    ~MainWindow();

private slots:
    void keyPressEvent(QKeyEvent* keyEvent);

    void on_applySheetParametersPushButton_clicked();

    void on_productsTableAddPushButton_clicked();

    void on_productsTableFindPushButton_clicked();

    void on_productsTableDeletePushButton_clicked();

    void on_cutPushButton_clicked();

    void on_productsTableWidget_cellChanged(int row, int column);

    void on_productsTableWidget_cellDoubleClicked(int row, int column);    

    void on_saveCutAction_triggered();
};
#endif // MAINWINDOW_H
