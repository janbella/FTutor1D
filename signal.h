#ifndef SIGNAL_H
#define SIGNAL_H

#include <QVector>
#include <iostream>
#include <exception>
#include <complex>

class MainWindow;

class Signal
{
    friend class MainWindow;
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

    inline double current_min_x()
    {
        return xmin - copies_left * (xmax - xmin);
    }


    inline double current_max_x()
    {
        return xmax + copies_right * (xmax - xmin);
    }


    inline double current_min_y()
    {
        return ymin;
    }


    inline double current_max_y()
    {
        return ymax;
    }


    inline double current_range_x()
    {
        return (xmax - xmin) * (1 + copies_left + copies_right);
    }


    inline double current_range_y()
    {
        return (xmax - xmin) * (1);
    }


    inline double original_range_x()
    {
        return xmax - xmin;
    }


    inline double original_range_y()
    {
        return ymax - ymin;
    }

    inline double allowed_max_x()
    {
        return xmax + 3*original_range_x();
    }

    inline double allowed_min_x()
    {
        return xmin - 3*original_range_x();
    }

    inline double allowed_max_y()
    {
        return ymax + original_range_y();
    }

    inline double allowed_min_y()
    {
        return ymin - original_range_y();
    }

private:
    QVector<std::complex<double> >  fft(QVector<double> input);

    QVector<double> ifft(QVector<std::complex<double> > input);

    QVector<std::complex<double> > fft_recursion(const QVector<double>& input);
    QVector<std::complex<double> >  ifft_recursion(const QVector<std::complex<double> >& input);

    static void complexToMagAndPhase(const QVector<std::complex<double> >& complex, QVector<double>& magnitude, QVector<double>& phase);

    static void magAndPhaseToComplex(const QVector<double>& magnitude, const QVector<double>& phase, QVector<std::complex<double> >& complex);
};

#endif // SIGNAL_H
