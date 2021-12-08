#include "cutter.h"

Cutter::Cutter(int sheetWidth, int sheetHeight) : sheetWidth(sheetWidth),
    sheetHeight(sheetHeight)
{}

int Cutter::getSheetWidth() const
{
    return sheetWidth;
}

void Cutter::setSheetWidth(int newSheetWidth)
{
    sheetWidth = newSheetWidth;
}

int Cutter::getSheetHeight() const
{
    return sheetHeight;
}

void Cutter::setSheetHeight(int newSheetHeight)
{
    sheetHeight = newSheetHeight;
}

void Cutter::addProducts(int width, int height, int amount)
{
    QRect newSheet;
    newSheet.setRect(0,0,width,height);
    for(int i = 0; i < amount; i++) {
        products.append(newSheet);
        qDebug() << "Added product: " << newSheet;
    }
}

const QList<QRect> &Cutter::getProducts() const
{
    return products;
}

void Cutter::deleteProducts(int width, int height, int amount)
{
    int count = 0;
    foreach(QRect rect, products) {
        if(count == amount) {
            break;
        } else {
            if(rect.width() == width && rect.height() == height) {
                products.removeAt(products.indexOf(rect));
                qDebug() << "Remove product: " << rect;
                count++;
            }
        }
    }
}

bool Cutter::decreasingWidthComparsion(const QRect &r1, const QRect &r2)
{
    qDebug() << "Decr width: " << r1.width() << " with " << r2.width();
    return r1.width() > r2.width();
}

bool Cutter::decreasingHeightComparsion(const QRect &r1, const QRect &r2)
{
    qDebug() << "Decr height: " << r1.height() << " with " << r2.height();
    return r1.height() > r2.height();
}

QList<QRect> Cutter::cutBurkeHorizontal()
{
    int sumWidth = 0;
    QList<int> gap;
    QList<QRect> unplacedProducts = products;
    std::sort(unplacedProducts.begin(), unplacedProducts.end(), decreasingWidthComparsion);
    std::sort(unplacedProducts.begin(), unplacedProducts.end(), decreasingHeightComparsion);

    for (int i = 0; i < sheetHeight; i++) {
        gap.push_back(0);
    }
    QList<QRect> placedProducts;
    while (!unplacedProducts.isEmpty()) {
        // find lowest gap
        int min = gap[0];
        int coordY = 0;
        for (int i = 1; i < gap.size(); i++) {
            if (gap[i] < min) {
                min = gap[i];
                coordY = i;
            }
        }
        int i = coordY + 1;
        int gapHeight = 1;
        while (i < gap.size() && gap[i] == gap[i - 1]) {
            gapHeight++;
            i++;
        }
        // find best fitting rectangle
        int   ind = -1;
        qreal fit = 0.0;
        for (int j = 0; j < unplacedProducts.size(); j++) {
            qreal curFit =   static_cast<qreal>(unplacedProducts[j].height())
                    / static_cast<qreal>(gapHeight);
            if (curFit < static_cast<qreal>(1) && curFit > fit) {
                fit = curFit;
                ind = j;
            }
        }
        if (ind > -1) {
            // place best fitting rectangle
            QRect newRect;
            sumWidth = gap[coordY] + unplacedProducts[ind].width();
            if (sumWidth <= sheetWidth) {
                newRect.setRect(gap[coordY], coordY, unplacedProducts[ind].width(), unplacedProducts[ind].height());
                qDebug() << "Burke vertical: " << gap[coordY] << " | " << coordY <<  " | " << newRect;
                placedProducts.push_back(newRect);
            } else {
                break;
            }
            // raise elements of array to appropriate height
            for (int j = coordY; j < coordY + unplacedProducts[ind].height(); j++) {
                gap[j] += unplacedProducts[ind].width();
            }
            unplacedProducts.removeAt(ind);
        } else {
            // raise gap to width of the lowest neighbour
            int lowest;
            if (coordY == 0) {
                lowest = gap[gapHeight];
            } else if (coordY + gapHeight == gap.size()) {
                lowest = gap[gap.size() - gapHeight - 1];
            } else if (gap[coordY - 1] < gap[coordY + gapHeight]) {
                lowest = gap[coordY - 1];
            } else {
                lowest = gap[coordY + gapHeight];
            }
            for (int j = coordY; j < coordY + gapHeight; j++) {
                gap[j] = lowest;
            }
        }
    }
    return placedProducts;
}

QList<QRect> Cutter::cutBurkeVertical()
{
    int sumHeight = 0;
    QList<int> gap;
    QList<QRect> unplacedProducts = products;
    std::sort(unplacedProducts.begin(), unplacedProducts.end(), decreasingHeightComparsion);
    std::sort(unplacedProducts.begin(), unplacedProducts.end(), decreasingWidthComparsion);

    for (int i = 0; i < sheetWidth; i++) {
        gap.push_back(0);
    }
    QList<QRect> placedProducts;
    while (!unplacedProducts.isEmpty()) {
        // find lowest gap
        int min = gap[0];
        int coordX = 0;
        for (int i = 1; i < gap.size(); i++) {
            if (gap[i] < min) {
                min = gap[i];
                coordX = i;
            }
        }
        int i = coordX + 1;
        int gapWidth = 1;
        while (i < gap.size() && gap[i] == gap[i - 1]) {
            gapWidth++;
            i++;
        }
        // find best fitting rectangl
        int   ind = -1;
        qreal fit = 0.0;
        for (int j = 0; j < unplacedProducts.size(); j++) {
            qreal curFit =   static_cast<qreal>(unplacedProducts[j].width())
                    / static_cast<qreal>(gapWidth);
            if (curFit < static_cast<qreal>(1) && curFit > fit) {
                fit = curFit;
                ind = j;
            }
        }
        if (ind > -1) {
            // place best fitting rectangle
            QRect newRect;
            sumHeight = sheetHeight - (gap[coordX] + unplacedProducts[ind].height());
            if (sumHeight >= 0) {
                newRect.setRect(coordX, sheetHeight - (gap[coordX] + unplacedProducts[ind].height()),
                                unplacedProducts[ind].width(), unplacedProducts[ind].height());
                qDebug() << "Burke vertical: " << coordX << " | " << sheetHeight - (gap[coordX] + unplacedProducts[ind].height()) <<  " | " << newRect;
                placedProducts.push_back(newRect);
            } else {
                break;
            }
            // raise elements of array to appropriate width
            for (int j = coordX; j < coordX + unplacedProducts[ind].width(); j++) {
                gap[j] += unplacedProducts[ind].height();
            }
            unplacedProducts.removeAt(ind);
        } else {
            // raise gap to height of the lowest neighbour
            int lowest;
            if (coordX == 0) {
                lowest = gap[gapWidth];
            } else if (coordX + gapWidth == gap.size()) {
                lowest = gap[gap.size() - gapWidth - 1];
            } else if (gap[coordX - 1] < gap[coordX + gapWidth]) {
                lowest = gap[coordX - 1];
            } else {
                lowest = gap[coordX + gapWidth];
            }
            for (int j = coordX; j < coordX + gapWidth; j++) {
                gap[j] = lowest;
            }
        }
    }
    return placedProducts;
}
