#include "dda.h"
#include <cmath>

#define roundf(x) floor(x + 0.5f)

DDA::DDA(int x1, int y1, int x2, int y2, QWidget *parent) : QWidget(parent)
{
    // Определяем разности координат
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;

    // Определяем количество шагов, основанное на большей разности координат
    int steps = std::max(abs(deltaX), abs(deltaY));

    // Вычисляем приращения по X и Y
    double dX = static_cast<double>(deltaX) / steps;
    double dY = static_cast<double>(deltaY) / steps;

    double x = x1;
    double y = y1;

    // Генерируем промежуточные точки
    for (int i = 0; i <= steps; i++) {
        QPointF p(roundf(x), roundf(y));
        points.push_back(p);
        x += dX;
        y += dY;
    }
}

void DDA::paintEvent(QPaintEvent*)
{
    QPainter painter1(this);
    double h = this->height();
    double w = this->width();
    h /= 2;
    w /= 2;

    double hScale = 20; // Шаг по оси Y
    double wScale = 20; // Шаг по оси X

    painter1.translate(w, h);
    QColor green1Color("#31bd83");
    QColor pinkColor("#bd3169");


    // Настройка красных линий и маркеров
    painter1.setPen(QPen(green1Color, 2));
    painter1.setBrush(pinkColor);

    // Рисуем линию, соединяющую точки
    for (size_t i = 0; i < points.size() - 1; ++i) {
        double x1 = points[i].x() * wScale;
        double y1 = -points[i].y() * hScale;
        double x2 = points[i + 1].x() * wScale;
        double y2 = -points[i + 1].y() * hScale;

        // Рисуем линию между соседними точками
        painter1.drawLine(x1, y1, x2, y2);
    }

    // Рисуем маркеры на каждой точке
    for (const auto &point : points) {
        double x = point.x() * wScale;
        double y = -point.y() * hScale;

        // Небольшой квадратный маркер на точке
        double markerSize = 8; // Размер маркера (можно настроить)
        painter1.drawRect(x - markerSize / 2, y - markerSize / 2, markerSize, markerSize);
    }

    // Отрисовка осей координат
    painter1.setPen(QPen(pinkColor, 3)); // Более толстая линия для осей
    painter1.drawLine(-this->width() / 2, 0, this->width() / 2, 0); // Ось X
    painter1.drawLine(0, -this->height() / 2, 0, this->height() / 2); // Ось Y

    // Отрисовка сетки и меток
    QPainterPath text;
    QFont f("Times", 8);
    f.setStyleStrategy(QFont::ForceOutline);

    // Отрисовка вертикальной и горизонтальной сетки и меток по осям
    painter1.setPen(QPen(Qt::black, 1)); // Линии сетки будут тоньше
    for (int i = 1; i < this->width() / (wScale * 2); i++) {
        painter1.drawLine(-i * wScale, -this->height() / 2, -i * wScale, this->height() / 2); // Вертикальные линии
        painter1.drawLine(i * wScale, -this->height() / 2, i * wScale, this->height() / 2);
    }

    for (int i = 1; i < this->height() / (hScale * 2); i++) {
        painter1.drawLine(-this->width() / 2, -i * hScale, this->width() / 2, -i * hScale); // Горизонтальные линии
        painter1.drawLine(-this->width() / 2, i * hScale, this->width() / 2, i * hScale);
    }

    // Отрисовка цифр по осям
    painter1.setPen(QPen(Qt::black, 3));
    text.addText(0, 0, f, QString::number(0));
    for (int i = 1; i < this->width() / (wScale * 2); i++) {
        painter1.drawPoint(i * wScale, 0); // Метки на оси X
        text.addText(i * wScale, 12, f, QString::number(i));
        painter1.drawPoint(-i * wScale, 0);
        text.addText(-i * wScale, 12, f, QString::number(-i));
    }
    for (int i = 1; i < this->height() / (hScale * 2); i++) {
        painter1.drawPoint(0, i * hScale); // Метки на оси Y
        text.addText(0, -i * hScale, f, QString::number(i));
        painter1.drawPoint(0, -i * hScale);
        text.addText(0, i * hScale, f, QString::number(-i));
    }

    // Отрисовка текста
    painter1.setPen(QPen(Qt::black, 1));
    painter1.setBrush(QBrush(Qt::black));
    painter1.drawPath(text);
}
