#include "mainscene.h"
#include "dataconfig.h"
#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainScene w;
    w.show();
    return a.exec();
}
