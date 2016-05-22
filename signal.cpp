#include "signal.h"

#include <iostream>
#include <fstream>
#include <exception>
#include <string>
#include <sstream>
#include <cmath>
#include <limits>
#include <algorithm>

Signal::Signal()
{
    copies_left = 0;
    copies_right = 0;
    original_length = 0;

    xmin = -std::numeric_limits<double>::max();
    xmax = std::numeric_limits<double>::max();
    ymin = -std::numeric_limits<double>::max();
    ymax = std::numeric_limits<double>::max();
}

Signal::Signal(const std::string& filename)
{
    original_length = 0;

    load_file(filename);

    copies_left = 0;
    copies_right = 0;
}


void Signal::load_file(const std::string& filename)
{
    x.clear();
    y.clear();
    xmax = std::numeric_limits<double>::min();
    ymax = std::numeric_limits<double>::min();
    xmin = std::numeric_limits<double>::max();
    ymin = std::numeric_limits<double>::max();

    std::ifstream file(filename);
    while(file.good())
    {
        std::string line;
        std::getline (file,line);
        if(line.empty() || line[0] == '#')
        {
            continue;
        }

        std::stringstream linestream(line);
        double x;
        double y;

        if (!(linestream >> x))
        {
            //error
        }
        if (!(linestream >> y))
        {
            // error
        }

        this->x.push_back(x);
        this->y.push_back(y);

        if(x > xmax)
            xmax = x;
        if(x < xmin)
            xmin = x;

        if(y > ymax)
            ymax = y;
        if(y < ymin)
            ymin = y;

        if(linestream >> x)
        {
            // error.
        }

        original_length++;
    }
}


void Signal::extend_left()
{
    if(original_length == 0) return;
    for(int i = original_length; i >= 0; i--)
    {
        x.push_front(x[(copies_left + 1) * original_length - 1] - (copies_left + 1) * (xmax - xmin));
        y.push_front(y[(copies_left + 1) * original_length - 1]);
    }

    copies_left++;
}


void Signal::extend_right()
{
    if(original_length == 0) return;

    for(int i = 0; i < original_length; i++)
    {
        x.push_back(x[copies_left * original_length + i] + (copies_right + 1) * (xmax - xmin));
        y.push_back(y[copies_left * original_length + i]);
    }
    copies_right++;
}


void Signal::shrink_left()
{
    if(original_length == 0) return;
    for(int i = 0; i < original_length; i++)
    {
        x.pop_front();
        y.pop_front();
    }
    copies_left--;
}


void Signal::shrink_right()
{
    if(original_length == 0) return;
    for(int i = 0; i < original_length; i++)
    {
        x.pop_back();
        y.pop_back();
    }
    copies_right--;

}


double  Signal::min_x()
{
    return xmin - copies_left * (xmax - xmin);
}


double  Signal::max_x()
{
    return xmax + copies_right * (xmax - xmin);
}


double  Signal::min_y()
{
    return ymin;
}


double  Signal::max_y()
{
    return ymax;
}


double Signal::range_x()
{
    return (xmax - xmin) * (1 + copies_left + copies_right);
}


double Signal::range_y()
{
    return (xmax - xmin) * (1);
}


double Signal::original_range_x()
{
    return xmax - xmin;
}


double Signal::original_range_y()
{
    return ymax - ymin;
}


QVector<std::complex<double> > Signal::fft_recursion(const QVector<double>& input)
{
    size_t length = input.size();
    QVector<std::complex<double> > fourier_coeffs;


    if(length == 1)
    {
        fourier_coeffs.push_back(std::complex<double>(input[0],0));
    }
    else
    {
        unsigned int prime = 2;
        while(length % prime != 0)
        {
            prime++;
        }

        QVector<double>* positions = new QVector<double>[prime];

        for(size_t i = prime -1; i < length; i += prime)
        {
            for(size_t j = 0; j < prime; j++)
            {
                positions[j].push_back(input[i - prime + 1 + j]);
            }
        }

        std::complex<double> psi = -2.0 * M_PI * std::complex<double>(0,1) / static_cast<double>(length);
        psi = exp(psi);

        QVector<std::complex<double> >* results = new QVector<std::complex<double> >[prime];

        for(size_t j = 0; j < prime; j++)
        {
            results[j] = fft_recursion(positions[j]);
        }
        delete[] positions;
        positions = nullptr;

        fourier_coeffs.resize(length);

        for(size_t i = 0; i<length; i++)
        {
            for(int j = prime - 1; j >= 0; j--)
            {
                fourier_coeffs[i] += results[j][i % results[j].size()] * pow(psi, (double)(i*j));
            }
        }
        delete[] results;
        results = nullptr;
    }
    return fourier_coeffs;
}


QVector<std::complex<double> >  Signal::ifft_recursion(const QVector<std::complex<double> >& input)
{
    size_t length = input.size();
    QVector<std::complex<double> > output;


    if(length == 1)
    {
        output.push_back(input[0]);
    }
    else
    {
        unsigned int prime = 2;
        while(length % prime != 0)
        {
            prime++;
        }

        QVector<std::complex<double> >* positions = new QVector<std::complex<double> >[prime];

        for(size_t i = prime -1; i < length; i += prime)
        {
            for(size_t j = 0; j < prime; j++)
            {
                positions[j].push_back(input[i - prime + 1 + j]);
            }
        }

        std::complex<double> psi = 2.0 * M_PI * std::complex<double>(0,1) / static_cast<double>(length);
        psi = exp(psi);

        QVector<std::complex<double> >* results = new QVector<std::complex<double> >[prime];

        for(size_t j = 0; j < prime; j++)
        {
            results[j] = ifft_recursion(positions[j]);
        }
        delete[] positions;
        positions = nullptr;


        output.resize(length);

        for(size_t i = 0; i<length; i++)
        {
            for(int j = prime - 1; j >= 0; j--)
            {
                output[i] += (results[j][i % results[j].size()] * pow(psi, static_cast<double>(i*j)));
            }
        }
        delete[] results;
        results = nullptr;
    }
    return output;
}


QVector<std::complex<double> > Signal::fft(QVector<double> input)
{
    QVector<std::complex<double> > result = fft_recursion(input);

    const double normalization = 1.0 / sqrtf(input.size());

    for(auto& i : result)
    {
        i *= normalization;
    }
    return result;
}


QVector<double >  Signal::ifft(QVector<std::complex<double> > input)
{
    QVector<std::complex<double> > result = ifft_recursion(input);

    const double normalization = 1.0 / sqrt(input.size());

    for(auto& i : result)
    {
        i *= normalization;
    }

    QVector<double> real;
    real.reserve(input.size());

    for(int i = 0; i < result.size(); i++)
    {
        real.push_back(result[i].real());
    }

    return real;
}


void Signal::fourierTransform(Signal& input, Signal& magnitude, Signal& phase)
{
    QVector<std::complex<double> > complex = fft(input.y);
    QVector<double> magnitude;
    QVector<double> phase;

    complexToMagAndPhase(complex,magnitude,phase);

}

void Signal::inverseFourierTransform(Signal& magnitude, Signal& phase, Signal& output)
{
    QVector<std::complex<double> > complex;
    magAndPhaseToComplex(magnitude.y,phase.y,complex);

    QVector<double> real = ifft(complex);


    output.y = real;
    output.x = magnitude.x;


}

void Signal::filtered(Signal& input, Signal& filter)
{

}

void Signal::complexToMagAndPhase(const QVector<double> &complex, QVector<double> &magnitude, QVector<double> &phase)
{

}

void Signal::magAndPhaseToComplex(const QVector<double> &magnitude, const QVector<double> &phase, QVector<double> &complex)
{

}
