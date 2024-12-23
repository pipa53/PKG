#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Загрузка стиля из файла
    QFile styleFile(":/style.qss"); // Используем путь из файла ресурсов
    if (styleFile.open(QFile::ReadOnly)) {
        QString style = styleFile.readAll();
        a.setStyleSheet(style);
        styleFile.close();
    }

    // Установка иконки приложения
    a.setWindowIcon(QIcon(":/icon.ico")); // Используем путь из файла ресурсов

    MainWindow w;
    w.show();
    return a.exec();
}
