#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QImage>

class ImageProcessor
{
public:
    ImageProcessor() = default;

    QImage applyMeanFilter(const QImage &image, int kernelSize, int iterations);
    QImage applyGaussianFilter(const QImage &image, int kernelSize, double sigma, int iterations);
    QImage equalizeHistogramRGB(const QImage &image);
    QImage equalizeHistogramHSV(const QImage &image);
    QImage linearContrast(const QImage &image);
};

#endif // IMAGEPROCESSOR_H
