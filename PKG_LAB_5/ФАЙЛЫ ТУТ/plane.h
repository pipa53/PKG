#ifndef PLANE_H
#define PLANE_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QVector>
#include <QPair>

enum type {
    Cirus,
    Central
};

class plane : public QWidget
{
public:
    plane(type, QWidget *parent = nullptr);
    void paintEvent(QPaintEvent*);

    void loadFromFile();

private:
    void clipSegments();
    void Cirus();
    void ClipByCirus(QPair<QPointF, QPointF>);
    float getT(QPair<QPointF, QPointF>, QPair<QPointF, QPointF>, bool*);
    int getCode(QPointF);
    QPointF intersectionPoint(QPointF, QPointF);

    QVector<QPair<QPointF, QPointF>> segments;
    QVector<QPair<QPointF, QPointF>> clippedSegments;
    QVector<QPointF> CirusSegment;
    QVector<QPair<QPointF, QPointF>> polygon;

    int Xmin, Xmax, Ymin, Ymax;
    type t;
    float t_1, t_2;
};

#endif // PLANE_H
