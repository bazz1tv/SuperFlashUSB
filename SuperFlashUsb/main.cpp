#include "mainwindow.h"
#include <QApplication>

MainWindow *w;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    w = new MainWindow;
    w->show();

    int r = a.exec();

    delete w;

    return r;
}
