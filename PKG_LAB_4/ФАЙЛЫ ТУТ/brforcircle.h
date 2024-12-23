#ifndef BRFORCIRCLE_H
#define BRFORCIRCLE_H

#include <QWidget>
#include <QPainter>
#include <vector>
#include <QPainterPath>
#include <cmath>

class BrForCircle : public QWidget {
    Q_OBJECT

public:
    explicit BrForCircle(int x0, int y0, int radius, QWidget* parent = nullptr);

    void paintEvent(QPaintEvent*) override;

private:
    std::vector<QPointF> points;
    int radius;
};

#endif // BRFORCIRCLE_H
