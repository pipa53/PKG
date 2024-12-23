#include "brforsegment.h"
#include <cmath>

BrForSegment::BrForSegment(int x1, int y1, int x2, int y2, QWidget *parent)
    : QWidget(parent) {
    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;
    int error = deltaX - deltaY;

    QPointF p;

    // Первой точкой будет (x1, y1)
    p.setX(x1);
    p.setY(y1);
    points.push_back(p);

    // Алгоритм добавления всех точек между начальной и конечной
    while (x1 != x2 || y1 != y2) {
        int error2 = error * 2;
        if (error2 > -deltaY) {
            error -= deltaY;
            x1 += signX;
        }
        if (error2 < deltaX) {
            error += deltaX;
            y1 += signY;
        }
        p.setX(x1);
        p.setY(y1);
        points.push_back(p); // Добавляем точку после движения
    }
}

void BrForSegment::paintEvent(QPaintEvent*) {
    QPainter painter1(this);
    double h = this->height();
    double w = this->width();
    h /= 2;
    w /= 2;
    double hScale = 20;
    double wScale = 20;

    painter1.translate(w, h);

    QColor green1Color("#31bd83");
    QColor pinkColor("#bd3169");
    // Отрисовка осей абсцисс и ординат
    painter1.setPen(QPen(pinkColor, 3));  // Более толстая линия для осей
    painter1.drawLine(-this->width() / 2, 0, this->width() / 2, 0);  // Ось X
    painter1.drawLine(0, -this->height() / 2, 0, this->height() / 2);  // Ось Y



    // Настройка красных линий и маркеров
    painter1.setPen(QPen(green1Color, 2));
    painter1.setBrush(pinkColor);

    // Рисуем линии между точками
    for (size_t i = 0; i < points.size() - 1; ++i) {
        double x1 = points[i].x() * wScale;
        double y1 = -points[i].y() * hScale;
        double x2 = points[i + 1].x() * wScale;
        double y2 = -points[i + 1].y() * hScale;

        // Рисуем линию между точками
        painter1.drawLine(x1, y1, x2, y2);
    }

    // Рисуем квадраты-маркеры на каждой точке
    for (const auto &point : points) {
        double x = point.x() * wScale;
        double y = -point.y() * hScale;

        // Рисуем квадрат размером 6x6 на каждой точке
        double markerSize = 8;  // Размер квадрата
        painter1.drawRect(x - markerSize / 2, y - markerSize / 2, markerSize, markerSize);
    }

    // Отрисовка сетки и меток
    QPainterPath text;
    QFont f("Times", 8);
    f.setStyleStrategy(QFont::ForceOutline);

    // Отрисовка вертикальной и горизонтальной сетки и меток по осям
    painter1.setPen(QPen(Qt::black, 1));  // Линии сетки будут тоньше
    for (int i = 1; i < this->width() / (wScale * 2); i++) {
        painter1.drawLine(-i * wScale, -this->height() / 2, -i * wScale, this->height() / 2);  // Вертикальные линии
        painter1.drawLine(i * wScale, -this->height() / 2, i * wScale, this->height() / 2);
    }

    for (int i = 1; i < this->height() / (hScale * 2); i++) {
        painter1.drawLine(-this->width() / 2, -i * hScale, this->width() / 2, -i * hScale);  // Горизонтальные линии
        painter1.drawLine(-this->width() / 2, i * hScale, this->width() / 2, i * hScale);
    }

    // Отрисовка цифр
    painter1.setPen(QPen(Qt::black, 3));
    text.addText(0, 0, f, QString::number(0));
    for (int i = 1; i < this->width() / (wScale * 2); i++) {
        painter1.drawPoint(i * wScale, 0);  // Метки на оси X
        text.addText(i * wScale, 12, f, QString::number(i));
        painter1.drawPoint(-i * wScale, 0);
        text.addText(-i * wScale, 12, f, QString::number(-i));
    }
    for (int i = 1; i < this->height() / (hScale * 2); i++) {
        painter1.drawPoint(0, i * hScale);  // Метки на оси Y
        text.addText(0, -i * hScale, f, QString::number(i));
        painter1.drawPoint(0, -i * hScale);
        text.addText(0, i * hScale, f, QString::number(-i));
    }

    // Отрисовка текста
    painter1.setPen(QPen(Qt::black, 1));
    painter1.setBrush(QBrush(Qt::black));
    painter1.drawPath(text);
}

