#include "mainwindow.h"
#include <QApplication>

#include "signal.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();

//    Signal s;

//    QVector<double> input;
//    for(int i=0; i<4; i++)
//    {
//        input.push_back(i);
//    }

//    QVector<std::complex<double> > result = s.fft(input);

//    for(std::complex<double> value : result)
//    {
//        std::cout << value << std::endl;
//    }

//    QVector<double> orig = s.ifft(result);

//    for(double value : orig)
//    {
//        std::cout << value << std::endl;
//    }

    return 0;

}
