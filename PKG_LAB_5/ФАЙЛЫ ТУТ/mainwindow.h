#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <plane.h>
#include <QVBoxLayout>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void showCirus();
    void showCentral();

private:
    plane* p;
    QVBoxLayout *layout;
    Ui::MainWindow *ui;
    QPushButton *pbCentral;
    QPushButton *pbCirus;
};

#endif // MAINWINDOW_H
