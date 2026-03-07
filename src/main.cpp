#include <QApplication>
#include <QFontDatabase>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QFontDatabase::addApplicationFont(":/fonts/OpenSans-VariableFont_wdth,wght.ttf");
    MainWindow wnd;
    wnd.resize(1400, 900);
    wnd.show();
    return app.exec();
}
