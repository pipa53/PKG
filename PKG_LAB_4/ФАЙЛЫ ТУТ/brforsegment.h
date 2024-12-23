#ifndef BRFORSEGMENT_H
#define BRFORSEGMENT_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <vector>

class BrForSegment : public QWidget
{
public:
    BrForSegment(int x1, int y1, int x2, int y2, QWidget *parent = nullptr);

    void paintEvent(QPaintEvent*);

private:
    std::vector<QPointF> points;
};

#endif // BRFORSEGMENT_H
