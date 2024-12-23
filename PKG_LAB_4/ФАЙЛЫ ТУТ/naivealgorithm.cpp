#include "naivealgorithm.h"
#include <cmath>

#define roundf(x) floor(x + 0.5f)

NaiveAlgorithm::NaiveAlgorithm(int x1, int y1, int x2, int y2, QWidget *parent) : QWidget(parent)
{
    double dx = static_cast<double>(x2 - x1);
    double dy = static_cast<double>(y2 - y1);

    // Проверка на одинаковые точки
    if (dx == 0 && dy == 0) {
        points.emplace_back(0, 0);
        return;
    }

    // Определение алгоритма отрисовки на основе наклона
    if (std::abs(dx) >= std::abs(dy)) {
        double a = dy / dx;
        double x = (x1 <= x2) ? x1 : x2;
        double y = (x1 <= x2) ? y1 : y2;

        if (x1 > x2) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }

        while ((x1 <= x2 && x <= x2) || (x1 >= x2 && x >= x2)) {
            points.emplace_back(roundf(x), roundf(y));
            x++;
            y += a;
        }
    } else {
        double a = dx / dy;
        double x = (y1 <= y2) ? x1 : x2;
        double y = (y1 <= y2) ? y1 : y2;

        if (y1 > y2) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }

        while ((y1 <= y2 && y <= y2) || (y1 >= y2 && y >= y2)) {
            points.emplace_back(roundf(x), roundf(y));
            y++;
            x += a;
        }
    }
}

void NaiveAlgorithm::paintEvent(QPaintEvent*)
{
    int side = qMin(width(), height());
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
    // Отрисовка осей
    painter1.setPen(QPen(pinkColor, 3)); // Более толстая линия для осей
    painter1.drawLine(-this->width() / 2, 0, this->width() / 2, 0); // Ось X
    painter1.drawLine(0, -this->height() / 2, 0, this->height() / 2); // Ось Y




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

    for (const auto &point : points) {
        double x = point.x() * wScale;
        double y = -point.y() * hScale;

        // Рисуем небольшой квадрат на каждой точке
        double markerSize = 8; // Размер маркера (можно настроить)
        painter1.drawRect(x - markerSize / 2, y - markerSize / 2, markerSize, markerSize);
    }

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

    // Отрисовка цифр
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
