#include "widget.h"
#include <QFile>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile styleFile(":/style.qss");
    styleFile.open(QFile::ReadOnly);
    QString style = QLatin1String(styleFile.readAll());
    qApp->setStyleSheet(style);

    Widget w;

    w.setWindowTitle("Чтение инфо из графических файлов");
    w.setWindowIcon(QIcon(":/icon.ico"));

    w.show();
    return a.exec();
}
