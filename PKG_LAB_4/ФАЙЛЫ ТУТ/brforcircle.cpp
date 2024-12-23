#include "brforcircle.h"
#include <QColor>

BrForCircle::BrForCircle(int x0, int y0, int radius, QWidget* parent) : QWidget(parent), radius(radius) {
    int x = radius;
    int y = 0;
    int radiusError = 1 - x;

    // Добавляем точки в порядке их обхода по кругу
    while (x >= y) {
        points.push_back(QPoint(x + x0, y + y0)); // 1
        points.push_back(QPoint(y + x0, x + y0)); // 2
        points.push_back(QPoint(-y + x0, x + y0)); // 3
        points.push_back(QPoint(-x + x0, y + y0)); // 4
        points.push_back(QPoint(-x + x0, -y + y0)); // 5
        points.push_back(QPoint(-y + x0, -x + y0)); // 6
        points.push_back(QPoint(y + x0, -x + y0)); // 7
        points.push_back(QPoint(x + x0, -y + y0)); // 8

        y++;

        if (radiusError < 0) {
            radiusError += 2 * y + 1;
        }
        else {
            x--;
            radiusError += 2 * (y - x + 1);
        }
    }

    // Сортируем точки по углу для правильного соединения
    std::sort(points.begin(), points.end(), [x0, y0](const QPointF& a, const QPointF& b) {
        double angleA = atan2(a.y() - y0, a.x() - x0);
        double angleB = atan2(b.y() - y0, b.x() - x0);
        return angleA < angleB;
        });
}

void BrForCircle::paintEvent(QPaintEvent*) {
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


    // Настройка красных линий и маркеров
    painter1.setPen(QPen(green1Color, 2));
    painter1.setBrush(pinkColor);

    // Рисуем маркеры на каждой точке
    for (const auto& point : points) {
        double x = point.x() * wScale;
        double y = -point.y() * hScale;

        double markerSize = 8;
        painter1.drawRect(x - markerSize / 2, y - markerSize / 2, markerSize, markerSize);
    }

    // Соединяем точки красной линией
    for (size_t i = 0; i < points.size(); ++i) {
        double x1 = points[i].x() * wScale;
        double y1 = -points[i].y() * hScale;

        double x2 = points[(i + 1) % points.size()].x() * wScale; // Соединяем последнюю точку с первой
        double y2 = -points[(i + 1) % points.size()].y() * hScale;

        painter1.drawLine(x1, y1, x2, y2);
    }

    // Отрисовка осей координат
    painter1.setPen(QPen(pinkColor, 3));  // Более толстая линия для осей
    painter1.drawLine(-this->width() / 2, 0, this->width() / 2, 0);  // Ось X
    painter1.drawLine(0, -this->height() / 2, 0, this->height() / 2);  // Ось Y

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

    // Отрисовка цифр по осям
    painter1.setPen(QPen(Qt::black, 3));
    text.addText(0, 0, f, QString::number(0));
    for (int i = 1; i < this->width() / (wScale * 2); i++) {
        text.addText(i * wScale, 12, f, QString::number(i));
        text.addText(-i * wScale, 12, f, QString::number(-i));
    }
    for (int i = 1; i < this->height() / (hScale * 2); i++) {
        text.addText(0, -i * hScale, f, QString::number(i));
        text.addText(0, i * hScale, f, QString::number(-i));
    }

    // Отрисовка текста
    painter1.setPen(QPen(Qt::black, 1));
    painter1.setBrush(QBrush(Qt::black));
    painter1.drawPath(text);
}
