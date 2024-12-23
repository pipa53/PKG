#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Pruning Algorithms");
    layout = new QVBoxLayout;

    p = new plane(type::Cirus);
    layout->addWidget(p);

    pbCentral = new QPushButton("Алгоритм Сазерленда-Коэна");
    pbCirus = new QPushButton("Алгоритм Кируса-Бека - отсечение отрезков выпуклым многоугольником");

    layout->addWidget(pbCentral);
    layout->addWidget(pbCirus);

    ui->centralwidget->setLayout(layout);

    connect(pbCentral, &QPushButton::clicked, this, &MainWindow::showCentral);
    connect(pbCirus, &QPushButton::clicked, this, &MainWindow::showCirus);
}

void MainWindow::showCirus() {
    p->hide();
    delete p;
    p = new plane(type::Cirus);
    p->loadFromFile();
    layout->insertWidget(0, p);
    p->show();
}

void MainWindow::showCentral() {
    p->hide();
    delete p;
    p = new plane(type::Central);
    p->loadFromFile();
    layout->insertWidget(0, p);
    p->show();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete p;
}
