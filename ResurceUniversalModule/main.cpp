#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile stylesheet_file(":/styles/qss/basic.css");
    stylesheet_file.open(QFile::ReadOnly);
    QString stylesheet = QLatin1String(stylesheet_file.readAll());
    a.setStyleSheet(stylesheet);
    MainWindow w;
    w.show();

    return a.exec();
}
