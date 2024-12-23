#include "mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    originalImageLabel(new QLabel(this)),
    processedImageLabel(new QLabel(this)),
    selectImageButton(new QPushButton("Выбрать изображение", this)),
    //processButton(new QPushButton("Применить сглаживание", this)),
    saveButton(new QPushButton("Сохранить обработанное изображение", this)),
    equalizeHistButton(new QPushButton("Эквализация гистограммы", this)),
    linearContrastButton(new QPushButton("Линейное контрастирование", this)),
    rgbButton(new QPushButton("RGB", this)),
    hsvButton(new QPushButton("HSV", this)),
    uniformFilterButton(new QPushButton("Однородный фильтр", this)),
    gaussFilterButton(new QPushButton("Фильтр Гаусса", this)),
    imageProcessor(new ImageProcessor()),
    colorModel("RGB"),
    smoothingFilter("Uniform"),
    originalImagePath("")
{
    // Применение стилей (если есть .qss файл)
    QFile file(":/style.qss");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
    }

    setWindowTitle("Image Processor");
    setWindowIcon(QIcon(":/ico.ico"));

    setMinimumSize(1200, 1000);

    // Создание и настройка текста для "Оригинал" и "Измененное изображение"
    QLabel *originalTextLabel = new QLabel("Оригинал", this);
    QLabel *processedTextLabel = new QLabel("Измененное изображение", this);

    // Увеличение шрифта для текста
    QFont textFont = originalTextLabel->font();
    textFont.setPointSize(14);  // Установим размер шрифта больше обычного

    originalTextLabel->setFont(textFont);
    processedTextLabel->setFont(textFont);

    // Центрирование текста
    originalTextLabel->setAlignment(Qt::AlignCenter);
    processedTextLabel->setAlignment(Qt::AlignCenter);

    // Установка начальных текстов для меток с изображениями
    originalImageLabel->setText("Здесь будет оригинальное изображение");
    processedImageLabel->setText("Здесь будет обработанное изображение");

    originalImageLabel->setStyleSheet("background-color: lightblue;");
    processedImageLabel->setStyleSheet("background-color: lightblue;");
    originalImageLabel->setAlignment(Qt::AlignCenter);
    processedImageLabel->setAlignment(Qt::AlignCenter);

    // Политика размера для изображений
    originalImageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    processedImageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Макет
    QVBoxLayout *originalLayout = new QVBoxLayout;
    originalLayout->addWidget(originalImageLabel);
    originalLayout->addWidget(new QLabel("Оригинал", this));

    QVBoxLayout *processedLayout = new QVBoxLayout;
    processedLayout->addWidget(processedImageLabel);
    processedLayout->addWidget(new QLabel("Измененное изображение", this));

    QHBoxLayout *imageLayout = new QHBoxLayout;
    imageLayout->addLayout(originalLayout);
    imageLayout->addLayout(processedLayout);

    QVBoxLayout *colorModelLayout = new QVBoxLayout;
    colorModelLayout->addWidget(rgbButton);
    colorModelLayout->addWidget(hsvButton);

    colorModelLayout->addWidget(equalizeHistButton);

    QString buttonStyle =
        "QPushButton {"
        "   background-color: #299470;"
        "   color: white;"
        "}"
        "QPushButton:hover {"
        "   background-color: #175943;"
        "}";

    rgbButton->setStyleSheet(buttonStyle);
    hsvButton->setStyleSheet(buttonStyle);
    equalizeHistButton->setStyleSheet(buttonStyle);


    QVBoxLayout *filterAndActionsLayout = new QVBoxLayout;
    filterAndActionsLayout->addWidget(linearContrastButton);
    filterAndActionsLayout->addWidget(uniformFilterButton);
    filterAndActionsLayout->addWidget(gaussFilterButton);
    //filterAndActionsLayout->addWidget(processButton);
    filterAndActionsLayout->addWidget(saveButton);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addLayout(colorModelLayout);
    buttonsLayout->addLayout(filterAndActionsLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(imageLayout);
    mainLayout->addWidget(selectImageButton);
    mainLayout->addLayout(buttonsLayout);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Соединения сигналов и слотов
    connect(selectImageButton, &QPushButton::clicked, this, &MainWindow::selectImage);
    //connect(processButton, &QPushButton::clicked, this, &MainWindow::processImage);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveProcessedImage);

    connect(rgbButton, &QPushButton::clicked, this, &MainWindow::setColorModelRGB);
    connect(hsvButton, &QPushButton::clicked, this, &MainWindow::setColorModelHSV);

    connect(equalizeHistButton, &QPushButton::clicked, this, &MainWindow::applyEqualizeHistogram);
    connect(linearContrastButton, &QPushButton::clicked, this, &MainWindow::applyLinearContrast);

    connect(uniformFilterButton, &QPushButton::clicked, this, &MainWindow::applyGaussianFilter);
    connect(gaussFilterButton, &QPushButton::clicked, this, &MainWindow::applyUniformFilter);
}

void MainWindow::selectImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Выбрать изображение"), "", tr("Images (*.png *.xpm *.jpg)"));
    if (!fileName.isEmpty()) {
        originalImage.load(fileName);
        originalImageLabel->setPixmap(QPixmap::fromImage(originalImage).scaled(originalImageLabel->size(), Qt::KeepAspectRatio));
        originalImagePath = fileName;
    }
}

void MainWindow::applyUniformFilter()
{
    if (originalImage.isNull()) {
        QMessageBox::warning(this, "Ошибка", "Выберите изображение.");
        return;
    }

    processedImage = imageProcessor->applyMeanFilter(originalImage, 3, 5); // 5 итераций для сильного сглаживания
    processedImageLabel->setPixmap(QPixmap::fromImage(processedImage).scaled(processedImageLabel->size(), Qt::KeepAspectRatio));
}

void MainWindow::applyGaussianFilter()
{
    if (originalImage.isNull()) {
        QMessageBox::warning(this, "Ошибка", "Выберите изображение.");
        return;
    }

    processedImage = imageProcessor->applyGaussianFilter(originalImage, 3, 1.0, 5); // b=1.0, 5 итераций для сильного сглаживания
    processedImageLabel->setPixmap(QPixmap::fromImage(processedImage).scaled(processedImageLabel->size(), Qt::KeepAspectRatio));
}

void MainWindow::saveProcessedImage()
{
    if (processedImage.isNull()) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Нет обработанного изображения для сохранения."));
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить изображение"), "", tr("Images (*.png *.xpm *.jpg)"));
    if (!fileName.isEmpty()) {
        processedImage.save(fileName);
    }
}

void MainWindow::setColorModelRGB()
{
    colorModel = "RGB";
}

void MainWindow::setColorModelHSV()
{
    colorModel = "HSV";
}

void MainWindow::applyEqualizeHistogram()
{
    if (originalImage.isNull()) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Сначала выберите изображение."));
        return;
    }

    if (colorModel == "RGB") {
        processedImage = imageProcessor->equalizeHistogramRGB(originalImage);
    } else if (colorModel == "HSV") {
        processedImage = imageProcessor->equalizeHistogramHSV(originalImage);
    }

    processedImageLabel->setPixmap(QPixmap::fromImage(processedImage).scaled(processedImageLabel->size(), Qt::KeepAspectRatio));
}

void MainWindow::applyLinearContrast()
{
    if (originalImage.isNull()) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Сначала выберите изображение."));
        return;
    }

    processedImage = imageProcessor->linearContrast(originalImage);
    processedImageLabel->setPixmap(QPixmap::fromImage(processedImage).scaled(processedImageLabel->size(), Qt::KeepAspectRatio));
}

void MainWindow::setUniformFilter()
{
    smoothingFilter = "Uniform";
}

void MainWindow::setGaussFilter()
{
    smoothingFilter = "Gauss";
}
