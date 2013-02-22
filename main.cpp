#include "wdgTvController.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    wdgTvController w;
    w.show();
    return a.exec();
}
