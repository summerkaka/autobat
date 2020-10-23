#include "mainwindow.h"
#include <QApplication>
#include "app_include.h"

int16_t comport_num = 0;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();

    return a.exec();
}

