#include <QApplication>
#include <QFile>
#include <QTextStream>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Загрузка файла стилей
    QFile styleFile(":/style/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QTextStream stream(&styleFile);
        QString style = stream.readAll();
        app.setStyleSheet(style);
        styleFile.close();
    }

    MainWindow w;
    w.show();
    return app.exec();
}
