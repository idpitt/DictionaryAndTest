#include "mainwindow.h"
#include <QApplication>
#include <dictionary.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   // qRegisterMetaType<Dictionary>("Dictionary");
    MainWindow w;
    w.show();

    return a.exec();
}
