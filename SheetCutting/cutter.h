#ifndef CUTTER_H
#define CUTTER_H
#include <QList>
#include <QRect>
#include <QDebug>

const int STRIPW = 120;
const int STRIPH = 60;

class Cutter
{
private:
    int sheetWidth;
    int sheetHeight;
    QList<QRect> products;

public:
    Cutter(int sheetWidth = 900, int sheetHeight = 300);

    int getSheetWidth() const;
    void setSheetWidth(int newSheetWidth);
    int getSheetHeight() const;
    void setSheetHeight(int newSheetHeight);

    void addProducts(int width, int height, int amount);
    const QList<QRect> &getProducts() const;
    void deleteProducts(int width, int height, int amount);

    static bool decreasingWidthComparsion(const QRect &r1, const QRect &r2);
    static bool decreasingHeightComparsion(const QRect &r1, const QRect &r2);

    QList<QRect> cutBurkeHorizontal();
    QList<QRect> cutBurkeVertical();

};

#endif // CUTTER_H
