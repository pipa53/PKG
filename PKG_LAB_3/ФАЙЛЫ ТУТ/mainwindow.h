#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QImage>
#include "imageprocessor.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void selectImage();
    //void processImage();
    void saveProcessedImage();
    void setColorModelRGB();
    void setColorModelHSV();
    void applyEqualizeHistogram();  // Эквализация гистограммы
    void applyLinearContrast();     // Линейное контрастирование
    void setUniformFilter();
    void setGaussFilter();

    void applyGaussianFilter();
    void applyUniformFilter();

private:
    QLabel *originalImageLabel;
    QLabel *processedImageLabel;
    QPushButton *selectImageButton;
    QPushButton *processButton;
    QPushButton *saveButton;
    QPushButton *rgbButton;
    QPushButton *hsvButton;
    QPushButton *uniformFilterButton;
    QPushButton *gaussFilterButton;
    QPushButton *equalizeHistButton;     // Кнопка для эквализации гистограммы
    QPushButton *linearContrastButton;   // Кнопка для линейного контрастирования

    ImageProcessor *imageProcessor;
    QString colorModel;
    QString smoothingFilter;
    QString originalImagePath;

    QImage originalImage;
    QImage processedImage;
};

#endif // MAINWINDOW_H
