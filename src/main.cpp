/**
 * @file main.cpp
 * @author Ján Bella
 **/

#include "mainwindow.h"
#include <QApplication>

/**
 * Application entry point. Creates the MainWindow and runs it.
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
