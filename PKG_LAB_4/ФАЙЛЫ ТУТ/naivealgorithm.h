#ifndef NAIVEALGORITHM_H
#define NAIVEALGORITHM_H

#include <QPainterPath>
#include <QWidget>
#include <vector>
#include <QPainter>

class NaiveAlgorithm : public QWidget
{
    Q_OBJECT
public:
    explicit NaiveAlgorithm(int x1, int y1, int x2, int y2, QWidget *parent = nullptr);

    void paintEvent(QPaintEvent*);

private:
    std::vector<QPointF> points;
};

#endif // NAIVEALGORITHM_H
