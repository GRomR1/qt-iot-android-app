#include "mainwindow.h"
#include <QApplication>
//#include <QDebug>
//#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QScreen *screen = QApplication::screens().at(0);
//    qDebug() << screen->size();

    MainWindow w;
    w.show();

    return a.exec();
}
