#ifndef SIGNAL_H
#define SIGNAL_H

#include <QVector>
#include <iostream>
#include <exception>
#include <complex>


class Signal
{
    QVector<double> x;
    QVector<double> y;

private:
    double xmax;
    double xmin;

    double ymax;
    double ymin;

    int original_length;

    int copies_left;
    int copies_right;

public:
    Signal();

    Signal(const std::string& filename);

    void load_file(const std::string& filename);

    static void fourierTransform(Signal& input, Signal& magnitude, Signal& phase);

    static void inverseFourierTransform(Signal& magnitude, Signal& phase, Signal& output);

    static void filtered(Signal& input, Signal& filter);

    void extend_left();
    void extend_right();

    void shrink_left();
    void shrink_right();

    inline double min_x();
    inline double max_x();
    inline double min_y();
    inline double max_y();

    inline double range_x();
    inline double range_y();

    inline double original_range_x();
    inline double original_range_y();

private:
    QVector<std::complex<double> >  fft(QVector<double> input);

    QVector<double> ifft(QVector<std::complex<double> > input);

    QVector<std::complex<double> > fft_recursion(const QVector<double>& input);
    QVector<std::complex<double> >  ifft_recursion(const QVector<std::complex<double> >& input);

    void complexToMagAndPhase(const QVector<double>& complex, QVector<double>& magnitude, QVector<double>& phase);

    void magAndPhaseToComplex(const QVector<double>& magnitude, const QVector<double>& phase, QVector<double>& complex);
};

#endif // SIGNAL_H
