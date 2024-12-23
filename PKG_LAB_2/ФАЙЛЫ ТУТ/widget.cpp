#include "widget.h"
#include "ui_widget.h"
#include <QElapsedTimer>  // Для измерения времени
#include <QMessageBox>    // Для отображения сообщений
#include <QDir>           // Для работы с директориями
#include <QFileDialog>    // Для выбора файлов и папок
#include <QImageWriter>   // Для получения информации о сжатии

using namespace std;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    model = new QFileSystemModel(this);
    model->setFilter(QDir::QDir::AllEntries);
    model->setRootPath("");
    ui->listView->setModel(model);
    ui->lineEdit->setText(" ");
    for(int i = 0; i < 5; i++)
    {
        sortcolumns[i] = 0;
    }
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_listView_doubleClicked(const QModelIndex &index)
{
    QListView* listView = (QListView*)sender();
    QFileInfo fileInfo = model->fileInfo(index);
    ui->lineEdit->setText(model->filePath(index));
    if(fileInfo.fileName() == "..")
    {
        QDir dir = fileInfo.dir();
        dir.cdUp();
        model->index(dir.absolutePath());
        listView->setRootIndex(model->index(dir.absolutePath()));
    }
    else if (fileInfo.fileName() == ".")
    {
        listView->setRootIndex(model->index(""));
    }
    else if(fileInfo.isDir())
    {
        listView->setRootIndex(index);
    }
    else if(!fileInfo.isDir())
    {
        QDir dir = fileInfo.dir();
        QString fileExt = model->fileName(index);
        QImageWriter a(model->filePath(index));
        QString resolution = "";
        QImage img (model->filePath(index));
        for(int i = fileExt.lastIndexOf('.'); i < fileExt.size(); i++)
        {
            resolution.append(fileExt[i]);
        }
        if (resolution.toLower() == ".jpg" || resolution.toLower() == ".jpeg" || resolution == ".gif" || resolution == ".tiff" || resolution == ".bmp" ||
            resolution == ".png" || resolution == ".pcx" || resolution == ".BMP")
        {
            ui->FileName->setText(model->fileName(index));
            ui->Size->setText(QString::number(img.size().width() )+ "x" + QString::number(img.size().height()  ));
            ui->Contraction->setText(QString::number(a.compression()));
            ui->Color_depth->setText(QString::number(img.bitPlaneCount()));
            ui->Resolution->setText(QString::number(img.physicalDpiX()) + "x" + QString::number(img.physicalDpiY()));
        }
    }
}

void Widget::dialogClose()
{
    this->show();
}

void Widget::on_multiChoice_clicked()
{
    // Выбор папки для загрузки файлов
    QString folder = QFileDialog::getExistingDirectory(this, "Выбрать папку");

    if (folder.isEmpty()) {
        return;  // Если пользователь не выбрал папку, выходим
    }

    QDir dir(folder);
    QStringList filters;
    filters << "*.jpg" << "*.jpeg" << "*.gif" << "*.tiff" << "*.bmp" << "*.png" << "*.pcx";  // Форматы файлов
    QStringList files = dir.entryList(filters, QDir::Files);  // Получаем список файлов в папке

    // Начало измерения времени
    QElapsedTimer timer;
    timer.start();

    int successfullyLoadedFiles = 0;  // Счётчик успешных загрузок

    QDialog *table = new QDialog();
    table->setWindowTitle("Table");
    table->setWindowIcon(QPixmap(":/img/folder.png"));
    QGridLayout *tableLayout = new QGridLayout(table);
    table->setMinimumSize(1200, 700);
    twInfo = new QTableWidget(table);
    tableLayout->addWidget(twInfo);
    twInfo->setColumnCount(6);  // Изменено с 5 на 6, добавлена колонка для DPI
    twInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    twInfo->setRowCount(files.size());
    twInfo->setHorizontalHeaderItem(0, new QTableWidgetItem("Имя файла"));
    twInfo->setHorizontalHeaderItem(1, new QTableWidgetItem("Размер в пикселях"));
    twInfo->setHorizontalHeaderItem(2, new QTableWidgetItem("Расширение файла"));
    twInfo->setHorizontalHeaderItem(3, new QTableWidgetItem("Глубина цвета"));
    twInfo->setHorizontalHeaderItem(4, new QTableWidgetItem("Сжатие"));
    twInfo->setHorizontalHeaderItem(5, new QTableWidgetItem("Разрешение (DPI)"));  // Новая колонка для DPI
    twInfo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (int i = 0; i < files.size(); i++)
    {
        QString filePath = dir.filePath(files[i]);
        QImage im(filePath);
        QImageWriter a(filePath);
        QString resolution = files[i].section('.', -1);  // Получаем расширение файла

        if (resolution.toLower() != "jpg" && resolution.toLower() != "jpeg" && resolution != "gif" && resolution != "tiff" && resolution != "bmp" &&
            resolution != "png" && resolution != "pcx" && resolution != "BMP")
        {
            continue;  // Пропускаем файлы с неподдерживаемыми расширениями
        }

        // Увеличиваем счётчик успешных загрузок
        successfullyLoadedFiles++;

        QString fileName = files[i].section('.', 0, 0);  // Имя файла без расширения
        twInfo->setItem(i, 0, new QTableWidgetItem(fileName));
        twInfo->setItem(i, 2, new QTableWidgetItem(resolution));
        twInfo->setItem(i, 1, new QTableWidgetItem(QString::number(im.size().width()) + "x" + QString::number(im.size().height())));
        twInfo->setItem(i, 3, new QTableWidgetItem(QString::number(im.bitPlaneCount())));
        twInfo->setItem(i, 4, new QTableWidgetItem(QString::number(a.compression())));

        // Добавляем информацию о разрешении DPI
        QString dpiResolution = QString::number(im.physicalDpiX()) + "x" + QString::number(im.physicalDpiY());
        twInfo->setItem(i, 5, new QTableWidgetItem(dpiResolution));  // Новая колонка для DPI
    }

    if (twInfo->rowCount() == 0)
    {
        return;  // Если нет загруженных файлов, выходим
    }

    // Завершение измерения времени и получение затраченных секунд
    qint64 timeElapsed = timer.elapsed();  // Время в миллисекундах
    double timeInSeconds = timeElapsed / 1000.0;  // Переводим в секунды

    // Сообщение с информацией о времени загрузки и количестве загруженных файлов
    QMessageBox::information(this, "Загрузка завершена",
                             "Время загрузки: " + QString::number(timeInSeconds) + " секунд\n" +
                                 "Успешно загружено файлов: " + QString::number(successfullyLoadedFiles));

    table->show();
    connect(table, SIGNAL(rejected()), this, SLOT(dialogClose()));
    connect(twInfo->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(twInfoSelected(int)));
    this->hide();
}

bool column1(QTableWidgetItem* item1, QTableWidgetItem* item2)
{
    return item1->text() > item2->text();
}

bool column2(QTableWidgetItem* item1, QTableWidgetItem* item2)
{
    QStringList string1 = item1->text().split('x');
    QStringList string2 = item2->text().split('x');
    return string1[0].toInt() > string2[0].toInt();
}

bool column3(QTableWidgetItem* item1, QTableWidgetItem* item2)
{
    return item1->text() > item2->text();
}

bool column4(QTableWidgetItem* item1, QTableWidgetItem* item2)
{
    return item1->text().toInt() > item2->text().toInt();
}

bool column5(QTableWidgetItem* item1, QTableWidgetItem* item2)
{
    return item1->text().toInt() > item2->text().toInt();
}

// Добавляем новую функцию для сортировки по DPI
bool column6(QTableWidgetItem* item1, QTableWidgetItem* item2)
{
    QStringList dpi1 = item1->text().split('x');
    QStringList dpi2 = item2->text().split('x');
    if (dpi1.size() != 2 || dpi2.size() != 2)
        return false;  // Если формат неправильный, не сортируем
    return dpi1[0].toInt() > dpi2[0].toInt();
}

vector<function<bool(QTableWidgetItem*, QTableWidgetItem*)>> functions = {column1, column2, column3, column4, column5, column6};  // Добавляем column6

vector<int> sortcolumns(6, 0);  // Изменено с 5 на 6, учитываем новую колонку

void Widget::twInfoSelected(int columnIndex)
{
    if (sortcolumns[columnIndex] == 0)
    {
        for(int i = 0; i < twInfo->rowCount(); i++)
        {
            for(int j = 0; j < twInfo->rowCount() - 1; j++)
            {
                if (functions[columnIndex](twInfo->item(j, columnIndex), twInfo->item(j + 1, columnIndex)))
                {
                    for(int k = 0; k < twInfo->columnCount(); k++)
                    {
                        QString temp1 = twInfo->item(j, k)->text();
                        QString temp2 = twInfo->item(j + 1, k)->text();
                        twInfo->setItem(j+1, k, new QTableWidgetItem(temp1));
                        twInfo->setItem(j, k, new QTableWidgetItem(temp2));
                    }
                }
            }
        }

        for(int k = 0; k < 6; k++)  // Изменено с 5 на 6
        {
            sortcolumns[k] = 0;
        }
        sortcolumns[columnIndex] = 1;
    }
    else
    {
        if (sortcolumns[columnIndex] == 1)
        {
            for(int i = 0; i < twInfo->rowCount(); i++)
            {
                for(int j = 0; j < twInfo->rowCount() - 1; j++)
                {
                    if (!functions[columnIndex](twInfo->item(j, columnIndex), twInfo->item(j + 1, columnIndex)))
                    {
                        for(int k = 0; k < twInfo->columnCount(); k++)
                        {
                            QString temp1 = twInfo->item(j, k)->text();
                            QString temp2 = twInfo->item(j + 1, k)->text();
                            twInfo->setItem(j+1, k, new QTableWidgetItem(temp1));
                            twInfo->setItem(j, k, new QTableWidgetItem(temp2));
                        }
                    }
                }
            }
            for(int k = 0; k < 6; k++)  // Изменено с 5 на 6
            {
                sortcolumns[k] = 0;
            }
            sortcolumns[columnIndex] = 2;
        }
        else
        {
            for(int i = 0; i < twInfo->rowCount(); i++)
            {
                for(int j = 0; j < twInfo->rowCount() - 1; j++)
                {
                    if (functions[columnIndex](twInfo->item(j, columnIndex), twInfo->item(j + 1, columnIndex)))
                    {
                        for(int k = 0; k < twInfo->columnCount(); k++)
                        {
                            QString temp1 = twInfo->item(j, k)->text();
                            QString temp2 = twInfo->item(j + 1, k)->text();
                            twInfo->setItem(j+1, k, new QTableWidgetItem(temp1));
                            twInfo->setItem(j, k, new QTableWidgetItem(temp2));
                        }
                    }
                }
            }
            for(int k = 0; k < 6; k++)  // Изменено с 5 на 6
            {
                sortcolumns[k] = 0;
            }
            sortcolumns[columnIndex] = 1;
        }
    }
}
