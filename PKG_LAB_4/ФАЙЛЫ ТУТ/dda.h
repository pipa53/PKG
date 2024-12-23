#ifndef DDA_H
#define DDA_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <vector>

class DDA : public QWidget
{
public:
    DDA(int x1, int y1, int x2, int y2, QWidget *parent = nullptr);

    void paintEvent(QPaintEvent*);

private:
    std::vector<QPointF> points;
};

#endif // DDA_H
