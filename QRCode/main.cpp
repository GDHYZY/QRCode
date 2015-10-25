#include "screenwidget.h"
#include <QApplication>
#include <QLabel>
#include "MyGlobalShortCut.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ScreenWidget w;
    return a.exec();
}
