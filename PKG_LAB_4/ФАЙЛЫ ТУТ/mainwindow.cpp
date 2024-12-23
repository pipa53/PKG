#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <QtMath>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    connect(pbBrCircle, SIGNAL(clicked()), this, SLOT(showBrCircle()));
    connect(pbBrSegment, SIGNAL(clicked()), this, SLOT(showBrSegment()));
    connect(pbNaiveAlgorithm, SIGNAL(clicked()), this, SLOT(showNaive()));
    connect(pbDDA, SIGNAL(clicked()), this, SLOT(showDDA()));
    this->setWindowTitle("Базовые растровые алгоритмы");
    this->setMinimumSize(1200,800);
    ui->setupUi(this);
    d = new Dialog(circle);
    lText->setText("Выберите алгоритм");
    pbBrCircle->setText("Алгоритм круга Брезенхэма");
    pbBrSegment->setText("Алгоритм сегмента Брезенхэма");
    pbNaiveAlgorithm->setText("Пошаговый алгоритм");
    pbDDA->setText("ЦДA");
    lText->setAlignment(Qt::AlignCenter);
    l->addWidget(lText, 10, 0, 1, 4);
    l->addWidget(pbBrCircle, 11,0,1,1);
    l->addWidget(pbBrSegment, 11,1,1,1);
    l->addWidget(pbNaiveAlgorithm, 11,2,1,1);
    l->addWidget(pbDDA, 11,3,1,1);
    l->addWidget(dda,1,0,10,4);
    /*l->addWidget(lText, 0, 1, 1, 1);
    l->addWidget(pbBrCircle, 1, 1);
    l->addWidget(pbBrSegment, 2, 1);
    l->addWidget(pbNaiveAlgorithm, 3, 1);
    l->addWidget(pbDDA, 4, 1);
    l->addWidget(dda, 0, 0, 10, 1);*/
    dda->hide();
    naive->hide();
    brfc->hide();
    brfs->hide();
    ui->centralwidget->setLayout(l);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showBrCircle(){
    naive->hide();
    brfc->hide();
    brfs->hide();
    dda->hide();
    QPointF p0;
    int radius;
    d = new Dialog(circle);
    while(true){
        if(d->exec()==QDialog::Accepted){
            radius = d->getRadius();
            p0 = d->getP0();
        }else{
            lText->setText("Выберите алгоритм");
            return;
        }
        if(qAbs(p0.x()) + qAbs(radius) >= this->height()/48 *2|| qAbs(p0.y()) + qAbs(radius) >= this->width()/40*2){
            outOfRangeError();
        }else{
            break;
        }
    }
    brfc = new BrForCircle(p0.x(),p0.y(),radius);
    l->addWidget(brfc,0,0,10,4);
    //l->addWidget(brfc, 0, 0, 10, 1);
    lText->setText("Алгоритм круга Брезенхэма");
}

void MainWindow::showBrSegment(){
    naive->hide();
    brfc->hide();
    brfs->hide();
    dda->hide();
    QPointF p0;
    QPointF p1;
    d = new Dialog(segment);
    while(true){
        if(d->exec()==QDialog::Accepted){
            p1 = d->getP1();
            p0 = d->getP0();
        }else{
            lText->setText("Выберите алгоритм");
            return;
        }
        if(qAbs(p0.x()) >= this->height()/48 || qAbs(p0.y()) >= this->width()/40 || qAbs(p1.x()) >= this->height()/48 || qAbs(p1.y()) >= this->width()/40){
            outOfRangeError();
        }else{
            break;
        }
    }
    brfs = new BrForSegment(p0.x(),p0.y(),p1.x(),p1.y());
    l->addWidget(brfs,0,0,10,4);
    lText->setText("Алгоритм сегмента Брезенхэма");
}

void MainWindow::showNaive(){
    naive->hide();
    brfc->hide();
    brfs->hide();
    dda->hide();
    QPointF p0;
    QPointF p1;
    d = new Dialog(segment);
    while(true){
        if(d->exec()==QDialog::Accepted){
            p1 = d->getP1();
            p0 = d->getP0();
        }else{
            lText->setText("Выберите алгоритм");
            return;
        }
        if(qAbs(p0.x()) >= this->height()/48 || qAbs(p0.y()) >= this->width()/40 || qAbs(p1.x()) >= this->height()/48 || qAbs(p1.y()) >= this->width()/40){
            outOfRangeError();
        }else{
            break;
        }
    }
    naive = new NaiveAlgorithm(p0.x(),p0.y(),p1.x(),p1.y());
    l->addWidget(naive,0,0,10,4);
    lText->setText("Пошаговый алгоритм");
}

void MainWindow::showDDA(){
    naive->hide();
    brfc->hide();
    brfs->hide();
    dda->hide();
    QPointF p0;
    QPointF p1;
    d = new Dialog(segment);
    while(true){
        if(d->exec()==QDialog::Accepted){
            p1 = d->getP1();
            p0 = d->getP0();
        }else{
            lText->setText("Выберите алгоритм");
            return;
        }
        if(qAbs(p0.x()) >= this->height()/48 || qAbs(p0.y()) >= this->width()/40 || qAbs(p1.x()) >= this->height()/48 || qAbs(p1.y()) >= this->width()/40){
            outOfRangeError();
        }else{
            break;
        }
    }
    dda = new DDA(p0.x(),p0.y(),p1.x(),p1.y());
    l->addWidget(dda,0,0,10,4);
    lText->setText("ЦДA");
}


void MainWindow::outOfRangeError(){
    QMessageBox::information(NULL,QObject::tr("Внимание!"),tr("Сегмент или окружность вне координатной плоскости! Выберите другие координаты."));
}

