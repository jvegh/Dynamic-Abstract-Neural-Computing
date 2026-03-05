#include <QApplication>
//#include <systemc.h>

#include "MyWindow.h"


int sc_main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyWindow w;

    w.show();

    return a.exec();
}

