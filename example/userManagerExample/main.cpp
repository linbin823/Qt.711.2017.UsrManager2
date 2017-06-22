#include "usrmanager.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    UsrManager::Instance()->showUI(true);
    return a.exec();
}
