#include "modbuscrc.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ModBusCRC w;
    w.show();
    return a.exec();
}
