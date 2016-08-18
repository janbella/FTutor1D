#ifndef SIGNAL_H
#define SIGNAL_H

/**
 * @file signal.h
 * @author Ján Bella
 */


#include "glob_includes.h"

#include <iostream>
#include <exception>
#include <algorithm>
#include <complex>

class MainWindow;

class Signal
{
private:
    QVector<double> original_x;
    QVector<double> original_y;

    QVector<double> extended_x;
    QVector<double> extended_y;

    double ymax;
    double ymin;

    int copies_left;
    int copies_right;

public:
    Signal();

    Signal(const std::string& filename);

    Signal(const QVector<double>& x, const QVector<double>& y);

    Signal(const Signal& other);

    Signal operator=(const Signal& other);

    bool load_file(const std::string& filename);
    bool save_file(const  std::string& filename);


    void set_x(QVector<double> x);
    void set_y(QVector<double> y);

    inline QVector<double> x()
    {
        return extended_x;
    }

    inline QVector<double> y()
    {
        return extended_y;
    }

    void extend_left();
    void extend_right();

    void shrink_left();
    void shrink_right();

    void reset();

    Signal makeImpulse(double x, double y);

    void zeroSignal(int length);


    inline double min_x() const
    {
        return extended_x.first();
    }


    inline double max_x() const
    {
        return extended_x.last();
    }


    inline double min_y() const
    {
        return ymin;
    }


    inline double max_y() const
    {
        return ymax;
    }

    inline double original_min_x() const
    {
        return original_x.first();
    }


    inline double original_max_x() const
    {
        return original_x.last();
    }


    inline double original_min_y() const
    {
        return ymin;
    }


    inline double original_max_y() const
    {
        return ymax;
    }


    inline double range_x() const
    {
        return extended_x.last() - extended_x.first();
    }


    inline double range_y() const
    {
        return (ymax - ymin);
    }


    inline double original_range_x() const
    {
        return original_x.last() - original_x.first();
    }


    inline double original_range_y() const
    {
        return ymax - ymin;
    }

    inline double allowed_max_x() const
    {
        return original_x.last() + 3*original_range_x();
    }

    inline double allowed_min_x() const
    {
        return original_x.first() - 3*original_range_x();
    }

    inline double allowed_max_y() const
    {
        return ymax + original_range_y();
    }

    inline double allowed_min_y() const
    {
        return ymin - original_range_y();
    }

    inline double original_center_x() const
    {
        return 0.5 * ( original_x.first() +  original_x.last());
    }

    inline double original_center_y() const
    {
        return 0.5 * (ymin + ymax);
    }

    inline int original_length() const
    {
        return original_x.size();
    }

    inline double avg_dx() const
    {
        return original_range_x() / original_length();
    }

    static void fourierTransform(Signal& input, Signal& magnitude, Signal& phase);

    static void inverseFourierTransform(Signal& magnitude, Signal& phase, Signal& output, bool outputReal = true);

    static Signal filtered(Signal& input, Signal& filter);

    void sinusWave(int frequency, int xLength, int numberPoints);

    void cosinusWave(int frequency, int xLength, int numberPoints);

    void superSample();

    void dropSecondHalf();

    void dropSecondHalf2();

    void updateAll(int index, double value);

    int getOriginalIndex(double x);


private:
    QVector<std::complex<double> >  fft(QVector<double> input);

    QVector<double> ifft(QVector<std::complex<double> > input, bool outputReal = true);

    QVector<std::complex<double> > fft_recursion(const QVector<double>& input);
    QVector<std::complex<double> >  ifft_recursion(const QVector<std::complex<double> >& input);

    static void complexToMagAndPhase(const QVector<std::complex<double> >& complex, QVector<double>& magnitude, QVector<double>& phase);

    static void magAndPhaseToComplex(const QVector<double>& magnitude, const QVector<double>& phase, QVector<std::complex<double> >& complex);
};

#endif // SIGNAL_H
