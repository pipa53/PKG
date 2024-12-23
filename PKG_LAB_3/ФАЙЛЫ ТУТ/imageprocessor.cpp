#include "imageprocessor.h"
#include <QtMath>
#include <QColor>
#include <algorithm>

#include "imageprocessor.h"
#include <QtAlgorithms>
#include <QColor>

QImage ImageProcessor::applyMeanFilter(const QImage &image, int kernelSize, int iterations)
{
    QImage result = image;
    int width = image.width();
    int height = image.height();

    for (int it = 0; it < iterations; ++it) {
        QImage temp = result;
        for (int y = 1; y < height - 1; ++y) {
            for (int x = 1; x < width - 1; ++x) {
                int r = 0, g = 0, b = 0;

                // Применение фильтра 3x3
                for (int j = -1; j <= 1; ++j) {
                    for (int i = -1; i <= 1; ++i) {
                        QColor pixelColor = temp.pixelColor(x + i, y + j);
                        r += pixelColor.red();
                        g += pixelColor.green();
                        b += pixelColor.blue();
                    }
                }

                // Усреднение
                r /= 9;
                g /= 9;
                b /= 9;

                // Установка новых значений цвета пикселя
                result.setPixelColor(x, y, QColor(r, g, b));
            }
        }
    }

    return result;
}

QImage ImageProcessor::applyGaussianFilter(const QImage &image, int kernelSize, double sigma, int iterations)
{
    QImage result = image;
    int width = image.width();
    int height = image.height();

    double b = sigma;
    double W = (2 + b) * (2 + b);
    int kernel[3][3] = {
        { 1, b, 1 },
        { b, b * b, b },
        { 1, b, 1 }
    };

    for (int it = 0; it < iterations; ++it) {
        QImage temp = result;
        for (int y = 1; y < height - 1; ++y) {
            for (int x = 1; x < width - 1; ++x) {
                double r = 0, g = 0, b_val = 0;

                for (int j = -1; j <= 1; ++j) {
                    for (int i = -1; i <= 1; ++i) {
                        QColor pixelColor = temp.pixelColor(x + i, y + j);
                        r += kernel[j + 1][i + 1] * pixelColor.red();
                        g += kernel[j + 1][i + 1] * pixelColor.green();
                        b_val += kernel[j + 1][i + 1] * pixelColor.blue();
                    }
                }

                r /= W;
                g /= W;
                b_val /= W;

                result.setPixelColor(x, y, QColor(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b_val)));
            }
        }
    }

    return result;
}


QImage ImageProcessor::linearContrast(const QImage &image)
{
    QImage result = image;
    for (int y = 0; y < result.height(); ++y) {
        for (int x = 0; x < result.width(); ++x) {
            QColor color = result.pixelColor(x, y);
            int red = qBound(0, static_cast<int>(1.2 * (color.red() - 128) + 128), 255); // Пример коррекции контраста
            int green = qBound(0, static_cast<int>(1.2 * (color.green() - 128) + 128), 255);
            int blue = qBound(0, static_cast<int>(1.2 * (color.blue() - 128) + 128), 255);
            result.setPixelColor(x, y, QColor(red, green, blue));
        }
    }
    return result;
}


QImage ImageProcessor::equalizeHistogramRGB(const QImage &image) {
    QImage result = image;
    int histogram[256] = {0};
    int pixelCount = image.width() * image.height();

    // Вычисляем гистограмму
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            int intensity = qGray(image.pixel(x, y));
            histogram[intensity]++;
        }
    }

    // Вычисляем кумулятивную гистограмму
    int cumHistogram[256] = {0};
    cumHistogram[0] = histogram[0];
    for (int i = 1; i < 256; ++i) {
        cumHistogram[i] = cumHistogram[i - 1] + histogram[i];
    }

    // Эквализация гистограммы
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor pixelColor = image.pixelColor(x, y);
            int r = cumHistogram[pixelColor.red()] * 255 / pixelCount;
            int g = cumHistogram[pixelColor.green()] * 255 / pixelCount;
            int b = cumHistogram[pixelColor.blue()] * 255 / pixelCount;
            result.setPixelColor(x, y, QColor(r, g, b));
        }
    }

    return result;
}

QImage ImageProcessor::equalizeHistogramHSV(const QImage &image)
{
    QImage result = image;
    QVector<int> histogram(256, 0);
    QVector<int> lut(256, 0); // Look-Up Table

    // Пройтись по изображению и посчитать гистограмму яркости (V)
    for (int y = 0; y < result.height(); ++y) {
        for (int x = 0; x < result.width(); ++x) {
            QColor color = result.pixelColor(x, y);
            int value = color.value();
            histogram[value]++;
        }
    }

    // Накопить распределение и создать LUT для эквализации
    int totalPixels = result.width() * result.height();
    int cumulativeSum = 0;
    for (int i = 0; i < 256; ++i) {
        cumulativeSum += histogram[i];
        lut[i] = qBound(0, (cumulativeSum * 255) / totalPixels, 255);
    }

    // Применить LUT к компоненте V
    for (int y = 0; y < result.height(); ++y) {
        for (int x = 0; x < result.width(); ++x) {
            QColor color = result.pixelColor(x, y);
            int hue = color.hue();
            int saturation = color.saturation();
            int value = lut[color.value()];
            color.setHsv(hue, saturation, value); // Применяем эквализацию только к яркости
            result.setPixelColor(x, y, color);
        }
    }

    return result;
}

