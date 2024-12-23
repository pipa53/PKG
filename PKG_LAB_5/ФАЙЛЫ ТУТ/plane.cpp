#include "plane.h"
#include <QFileDialog>
#include <QPainter>
#include <cmath>
#include <QMessageBox>
#include <QVector2D>
plane::plane(type t, QWidget *parent) : QWidget(parent), t(t) {}
void plane::paintEvent(QPaintEvent*)
{
    QPainter painter1(this);
    double h = this->height();
    double w = this->width();
    h /= 2;
    w /= 2;
    double hScale = 20;
    double wScale = 20;
    painter1.translate(w, h);

    QPainterPath text;
    QFont f("Times", 8);
    f.setStyleStrategy(QFont::ForceOutline);

    QColor green1Color("#31bd83");
    QColor pinkColor("#bd3169");

    QColor purpleColor("#3f31bd");
    QColor orangeColor("#bd3831");

    painter1.setPen(QPen(pinkColor, 3));
    painter1.drawLine(-this->width() / 2, 0, this->width() / 2, 0);
    painter1.drawLine(0, -this->height() / 2, 0, this->height() / 2);

    painter1.setPen(QPen(Qt::black, 1));

    painter1.setPen(QPen(Qt::black, 3));
    text.addText(0, 0, f, QString::number(0));

    for (int i = 1; i < this->width() / (wScale * 2); i++) {
        painter1.setPen(QPen(Qt::black, 3));
        painter1.drawPoint(i * wScale, 0);
        text.addText(i * wScale, 12, f, QString::number(i));
        painter1.drawPoint(-i * wScale, 0);
        text.addText(-i * wScale, 12, f, QString::number(-i));
        painter1.setPen(QPen(Qt::black, 1));
        painter1.drawLine(-i * wScale, -this->height() / 2, -i * wScale, this->height() / 2);
        painter1.drawLine(i * wScale, -this->height() / 2, i * wScale, this->height() / 2);
    }

    for (int i = 1; i < this->height() / (hScale * 2); i++) {
        painter1.setPen(QPen(Qt::black, 3));
        painter1.drawPoint(0, i * hScale);
        text.addText(0, -i * hScale, f, QString::number(i));
        painter1.drawPoint(0, -i * hScale);
        text.addText(0, i * hScale, f, QString::number(-i));
        painter1.setPen(QPen(Qt::black, 1));
        painter1.drawLine(-this->width() / 2, -i * hScale, this->width() / 2, -i * hScale);
        painter1.drawLine(-this->width() / 2, i * hScale, this->width() / 2, i * hScale);
    }

    painter1.setPen(QPen(orangeColor, 3));
    painter1.setBrush(Qt::NoBrush);
    for (const auto& segment : segments) {
        painter1.drawLine(segment.first.x() * 20, -segment.first.y() * 20, segment.second.x() * 20, -segment.second.y() * 20);
    }

    painter1.setPen(QPen(purpleColor, 4));
    if (t == type::Central) {
        painter1.drawLine(Xmax * 20, -Ymax * 20, Xmax * 20, -Ymin * 20);
        painter1.drawLine(Xmax * 20, -Ymax * 20, Xmin * 20, -Ymax * 20);
        painter1.drawLine(Xmin * 20, -Ymin * 20, Xmax * 20, -Ymin * 20);
        painter1.drawLine(Xmin * 20, -Ymin * 20, Xmin * 20, -Ymax * 20);
    } else {
        for (const auto& edge : polygon) {
            painter1.drawLine(edge.first.x() * 20, -edge.first.y() * 20, edge.second.x() * 20, -edge.second.y() * 20);
        }
    }

    painter1.setPen(QPen(green1Color, 4));
    for (const auto& clipped : clippedSegments) {
        painter1.drawLine(clipped.first.x() * 20, -clipped.first.y() * 20, clipped.second.x() * 20, -clipped.second.y() * 20);
    }

    painter1.setPen(QPen(Qt::black, 1));
    painter1.setBrush(QBrush(Qt::black));
    painter1.drawPath(text);
}
void plane::loadFromFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите файл ввода", "", "Text Files (*.txt)");

    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Файл не выбран. Операция отменена.");
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл.");
        return;
    }

    QTextStream fin(&file);

    QString line = fin.readLine();
    int n = line.toInt();
    if (n <= 0) {
        QMessageBox::warning(this, "Ошибка", "Некорректные данные в файле.");
        file.close();
        return;
    }

    int x1, y1, x2, y2;
    QPointF p1, p2;

    for (int i = 0; i < n; i++) {
        line = fin.readLine();
        QStringList values = line.split(' ', Qt::SkipEmptyParts);
        if (values.size() < 4) {
            QMessageBox::warning(this, "Ошибка", "Некорректные данные в файле.");
            file.close();
            return;
        }
        x1 = values[0].toInt();
        y1 = values[1].toInt();
        x2 = values[2].toInt();
        y2 = values[3].toInt();

        p1.setX(x1);
        p1.setY(y1);
        p2.setX(x2);
        p2.setY(y2);
        segments.push_back(qMakePair(p1, p2));
    }

    line = fin.readLine();
    QStringList bounds = line.split(' ', Qt::SkipEmptyParts);
    if (bounds.size() < 4) {
        QMessageBox::warning(this, "Ошибка", "Некорректные данные в файле.");
        file.close();
        return;
    }
    Xmin = bounds[0].toInt();
    Ymin = bounds[1].toInt();
    Xmax = bounds[2].toInt();
    Ymax = bounds[3].toInt();

    line = fin.readLine();
    n = line.toInt();
    if (n <= 0) {
        QMessageBox::warning(this, "Ошибка", "Некорректные данные в файле.");
        file.close();
        return;
    }

    for (int i = 0; i < n; i++) {
        line = fin.readLine();
        QStringList values = line.split(' ', Qt::SkipEmptyParts);
        if (values.size() < 4) {
            QMessageBox::warning(this, "Ошибка", "Некорректные данные в файле.");
            file.close();
            return;
        }
        x1 = values[0].toInt();
        y1 = values[1].toInt();
        x2 = values[2].toInt();
        y2 = values[3].toInt();

        p1.setX(x1);
        p1.setY(y1);
        p2.setX(x2);
        p2.setY(y2);
        polygon.push_back(qMakePair(p1, p2));
    }

    file.close();

    if (t == type::Central) {
        clipSegments();
    } else {
        Cirus();
    }
}

// Функция plane::clipSegments
// Основная функция для отсечения отрезков в прямоугольной области.
// Использует алгоритм Коэна-Сазерленда для проверки положения каждого отрезка
// относительно границ области и выполнения отсечения или полного исключения.
void plane::clipSegments() {
    QVector<QPair<QPointF, QPointF>> segments = this->segments;
    for (int i = 0; i < segments.size(); i++) {
        QPointF P1 = segments[i].first;
        QPointF P2 = segments[i].second;
        int P1_code = getCode(P1), P2_code = getCode(P2);

        // Проверяем, если отрезок слишком мал для отсечения
        if (sqrt(pow(P1.x() - P2.x(), 2) + pow(P1.y() - P2.y(), 2)) <= 1) {
            return;
        }

        // Если оба конца внутри области (по коду), отрезок добавляется целиком
        if ((P1_code | P2_code) == 0) {
            clippedSegments.push_back(segments[i]);
        }
        // Если отрезок полностью снаружи, пропускаем его
        else if ((P1_code & P2_code) != 0) {
            continue;
        } else {
            // Если одна точка внутри, а другая вне, пересчитываем отрезок
            if (P1_code == 0) {
                QPointF tmp = P1;
                P1 = P2;
                P2 = tmp;
            }
            // Пересекаем отрезок с границей области и повторяем проверку
            segments[i] = qMakePair(P2, intersectionPoint(P1, P2));
            i--; // Перезапускаем проверку для обновленного отрезка
        }
    }
}
// Функция plane::getCode
// Вычисляет код области для заданной точки относительно границ области отсечения.
// Код области используется для определения положения точки внутри или вне границ.
int plane::getCode(QPointF point) {
    double x = point.x();
    double y = point.y();
    int code = 0;

    // Проверяем положение точки относительно области отсечения и устанавливаем биты кода
    if (x < Xmin) code += 1;
    if (x > Xmax) code += 2;
    if (y < Ymin) code += 4;
    if (y > Ymax) code += 8;

    return code;
}
// Функция plane::intersectionPoint
// Вычисляет точку пересечения заданного отрезка с границей области отсечения,
// исходя из кода положения точки вне границ.
// Помогает при частичном отсечении отрезков.
QPointF plane::intersectionPoint(QPointF P1, QPointF P2) {
    int P1_code = getCode(P1);
    QPointF intersectionPoint;

    // Рассчитываем точку пересечения для каждой стороны области, исходя из кода
    if ((P1_code & 8)) // Верхняя граница
        intersectionPoint = QPointF(P1.x() + (P2.x() - P1.x()) * (Ymax - P1.y()) / (P2.y() - P1.y()), Ymax);
    else if ((P1_code & 4)) // Нижняя граница
        intersectionPoint = QPointF(P1.x() + (P2.x() - P1.x()) * (Ymin - P1.y()) / (P2.y() - P1.y()), Ymin);
    else if ((P1_code & 2)) // Правая граница
        intersectionPoint = QPointF(Xmax, P1.y() + (P2.y() - P1.y()) * (Xmax - P1.x()) / (P2.x() - P1.x()));
    else if ((P1_code & 1)) // Левая граница
        intersectionPoint = QPointF(Xmin, P1.y() + (P2.y() - P1.y()) * (Xmin - P1.x()) / (P2.x() - P1.x()));

    return intersectionPoint;
}
// Функция plane::Cirus
// Выполняет отсечение отрезков в произвольной многоугольной области, используя алгоритм Кируса-Бека.
// Для каждого отрезка вычисляет параметры точки входа и выхода из области и добавляет
// отсеченные части в список.
void plane::Cirus() {
    clippedSegments.clear();
    for (const auto& segment : segments) {
        QPointF p1 = segment.first;
        QPointF p2 = segment.second;

        double tEnter = 0.0; // Параметр для входа
        double tExit = 1.0;  // Параметр для выхода

        bool visible = true;

        for (const auto& edge : polygon) {
            QPointF edgeStart = edge.first;
            QPointF edgeEnd = edge.second;

            // Направление и нормаль к ребру
            QPointF edgeDir = edgeEnd - edgeStart;
            QPointF normal(-edgeDir.y(), edgeDir.x());

            QPointF segDir = p2 - p1;

            // Вычисляем вектор от начала отрезка к началу грани
            QPointF w = p1 - edgeStart;
            double D = QVector2D::dotProduct(QVector2D(normal), QVector2D(segDir));
            double N = -QVector2D::dotProduct(QVector2D(normal), QVector2D(w));

            // Проверка на параллельность
            if (D == 0) {
                if (N < 0) {
                    visible = false;
                    break;
                } else {
                    continue;
                }
            }

            double t = N / D;
            if (D < 0) {
                // Входной пересекающий параметр
                if (t > tEnter) {
                    tEnter = t;
                }
            } else {
                // Выходной пересекающий параметр
                if (t < tExit) {
                    tExit = t;
                }
            }

            // Проверка на видимость
            if (tEnter > tExit) {
                visible = false;
                break;
            }
        }

        if (visible) {
            QPointF clippedStart = p1 + (p2 - p1) * tEnter;
            QPointF clippedEnd = p1 + (p2 - p1) * tExit;

            // Добавление видимой части отрезка в список
            clippedSegments.push_back(qMakePair(clippedStart, clippedEnd));
        }
    }
    update();
}


// Функция plane::getT
// Вычисляет параметр t для точки пересечения отрезка с гранью многоугольника.
// Если отрезок и грань параллельны, возвращает специальное значение и устанавливает флаг onLine.
float plane::getT(QPair<QPointF, QPointF> edge, QPair<QPointF, QPointF> segment, bool* onLine) {
    bool a;
    float ks = (segment.second.y() - segment.first.y()) / (segment.second.x() - segment.first.x());
    float ke = (edge.second.y() - edge.first.y()) / (edge.second.x() - edge.first.x());
    float bs = segment.first.y() - ks * segment.first.x();
    float be = edge.first.y() - ke * edge.first.x();
    float x = (be - bs) / (ks - ke);

    // Проверка, попадает ли x в границы отрезка
    if ((x - edge.first.x()) / (edge.second.x() - edge.first.x()) <= 0 || (x - edge.first.x()) / (edge.second.x() - edge.first.x()) >= 1) {
        return -1;
    }

    // Проверка на параллельность отрезка и грани
    if ((segment.second.x() - segment.first.x()) == 0 && ke == ks && be == bs) {
        a = true;
        onLine = &a;
        return -1;
    } else {
        float te = (x - segment.first.x()) / (segment.second.x() - segment.first.x());
        return te;
    }
}
// Вспомогательная функция ScalarMultiply
// Вычисляет скалярное произведение двух векторов, определенных парами точек.
// Используется для определения направления отсечения и позиции точки относительно многоугольника.
float ScalarMultiply(QPair<QPointF, QPointF> v1, QPair<QPointF, QPointF> v2) {
    return (-(v1.second.y() - v1.first.y()) * (v2.second.x() - v2.first.x()) + (v1.second.x() - v1.first.x()) * (v2.second.y() - v2.first.y()));
}
// Вспомогательная функция getParameterOfPoint
// Находит параметр точки p относительно отрезка segment, используемый для отсечения.
// Параметр отражает относительное положение точки на отрезке.
float getParameterOfPoint(QPointF p, QPair<QPointF, QPointF> segment) {
    return (p.x() - segment.first.x()) / (segment.second.x() - segment.first.x());
}
// Функция plane::ClipByCirus
// Основная функция отсечения методом Кируса-Бека для одного отрезка относительно многоугольной области.
// Вычисляет параметры входа и выхода, проверяя каждую грань многоугольника, и определяет
// видимую часть отрезка внутри области.
void plane::ClipByCirus(QPair<QPointF, QPointF> segment) {
    QVector<float> T_enter;
    QVector<float> T_outer;
    float t, S;
    bool onLine = false;

    for (const auto& edge : polygon) {
        // Находим параметр пересечения с каждой стороной многоугольника
        t = getT(edge, segment, &onLine);
        if (onLine) {
            // Параллельный отрезок полностью внутри/вне области отсечения
            T_enter.push_back(getParameterOfPoint(edge.first, segment));
            T_outer.push_back(getParameterOfPoint(edge.second, segment));
            T_outer.push_back(getParameterOfPoint(edge.first, segment));
            T_enter.push_back(getParameterOfPoint(edge.second, segment));
            onLine = false;
            continue;
        }
        S = -ScalarMultiply(edge, segment);
        if (t >= 0 && t <= 1) {
            if (S > 0) {
                T_enter.push_back(t);
            } else if (S < 0) {
                T_outer.push_back(t);
            } else {
                T_enter.push_back(t);
                T_outer.push_back(t);
            }
        }
    }

    // Если отрезок не пересекается ни с одной гранью
    if (T_outer.empty() && T_enter.empty()) {
        t_1 = -1;
        t_2 = -1;
        return;
    }

    // Определяем параметры входа и выхода
    float t_enter = 0;
    float t_outer = 1;

    for (float te : T_enter) {
        if (t_enter < te) {
            t_enter = te;
        }
    }
    for (float to : T_outer) {
        if (t_outer > to) {
            t_outer = to;
        }
    }

    t_1 = t_enter;
    t_2 = t_outer;
}

