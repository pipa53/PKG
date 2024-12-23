#include "dialog.h"
#include<QPushButton>
#include<QIntValidator>

Dialog::Dialog(dialogType type, QWidget* pwgt) : QDialog(pwgt, Qt::WindowTitleHint|Qt::WindowSystemMenuHint)
{
    this->type = type;
    p0.setX(0);
    p0.setY(0);
    p1.setX(0);
    p1.setY(0);
    radius = 0;
    leX0->setText("0");
    leX1->setText("0");
    leY0->setText("0");
    leY1->setText("0");
    leRadius->setText("0");
    leX0->setValidator(new QIntValidator(-100, 100, this));
    leX1->setValidator(new QIntValidator(-100, 100, this));
    leY0->setValidator(new QIntValidator(-100, 100, this));
    leY1->setValidator(new QIntValidator(-100, 100, this));
    leRadius->setValidator(new QIntValidator(0, 100, this));
    QLabel *lX0 = new QLabel(" x0 = ");
    QLabel *lX1 = new QLabel(" x1 = ");
    QLabel *lY0 = new QLabel(" y0 = ");
    QLabel *lY1 = new QLabel(" y1 = ");
    QLabel *lRadius = new QLabel(" R (радиус) = ");
    QPushButton* pcmdOk = new QPushButton("&ОК");
    QPushButton* pcmdCancel = new QPushButton("&Отмена");
    connect(pcmdOk, SIGNAL(clicked()), SLOT(accept()));
    connect(pcmdCancel, SIGNAL(clicked()), SLOT(reject()));
    l = new QGridLayout();
    if(type == circle){
        setWindowTitle("Ввод круга для алгоритма");
        l->addWidget(lX0, 0,0,1,1);
        l->addWidget(lY0, 1,0,1,1);
        l->addWidget(lRadius, 0,2,2,1);
        l->addWidget(leX0, 0,1,1,1);
        l->addWidget(leY0, 1,1,1,1);
        l->addWidget(leRadius, 0,3,2,1);
    }else{
        setWindowTitle("Ввод отрезка для алгоритма");
        l->addWidget(lX0, 0,0,1,1);
        l->addWidget(lY0, 1,0,1,1);
        l->addWidget(lX1, 0,2,1,1);
        l->addWidget(lY1, 1,2,1,1);
        l->addWidget(leX0, 0,1,1,1);
        l->addWidget(leY0, 1,1,1,1);
        l->addWidget(leX1, 0,3,1,1);
        l->addWidget(leY1, 1,3,1,1);
    }
    l->addWidget(pcmdOk, 2, 0);
    l->addWidget(pcmdCancel, 2, 3);
    setLayout(l);
    connect(leX0, &QLineEdit::textChanged, [this] {setX0();});
    connect(leX1, &QLineEdit::textChanged, [this] {setX1();});
    connect(leY0, &QLineEdit::textChanged, [this] {setY0();});
    connect(leY1, &QLineEdit::textChanged, [this] {setY1();});
    connect(leRadius, &QLineEdit::textChanged, [this] {setRadius();});
}

