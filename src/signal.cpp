#include "signal.h"

#include <iostream>
#include <fstream>
#include <exception>
#include <string>
#include <sstream>
#include <cmath>
#include <limits>
#include <algorithm>
#include <cassert>

#include <iostream>
#include <type_traits>
#include <utility>
#include <iterator>

#include <list>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


// TO_DELETE
// http://stackoverflow.com/a/31937324
void sort_together( QVector<double> & a,  QVector<double> & b)
{
    using PairOfIts = std::pair<decltype(a.begin()), decltype(b.begin())>;

    std::vector<PairOfIts> v;
    auto i = a.begin();
    auto j = b.begin();
    for (; i != a.end(); ++i, ++j)
        v.push_back(std::make_pair(i, j));

    std::sort(v.begin(), v.end(), [](PairOfIts const& i, PairOfIts const& j) { return *i.first < *j.first; } );

    QVector<double> sortedA;
    QVector<double> sortedB;
    for (auto& x : v) {
        sortedA.push_back(*x.first);
        sortedB.push_back(*x.second);
    }

    std::swap(sortedA, a);
    std::swap(sortedB, b);
}



Signal::Signal()
{
    copies_left = 0;
    copies_right = 0;

    ymin = 0;
    ymax = 0;

    spacing = 1;

    original.clear();

    extended_x.clear();
    extended_y.clear();
}


Signal::Signal(const std::string& filename)
{
    load_file(filename);

    copies_left = 0;
    copies_right = 0;
}

Signal::Signal(const QVector<double>& x, const QVector<double>& y)
{
    QVector<double> original_x = x;
    QVector<double> original_y = y;


    if(original_x.length() != original_y.length())
    {
        while(original_x.length() > original_y.length())
        {
            original_x.pop_back();
        }

        while(original_y.length() > original_x.length())
        {
            original_y.pop_back();
        }
    }

    original.clear();

    if(original_x.isEmpty())
    {
        spacing = 1;
        ymin = -1;
        ymax = 1;
    }
    else if(original_x.length() >= 1)
    {
        ymin = y[0];
        ymax = y[0];
        original.insert(x[0],y[0]);

        if(original_x.length() > 1)
        {
            spacing = original_x[1] - original_x[0];
        }
        else
        {
            spacing = 1;
        }
    }

    for(int i = 1; i< original_x.length();i++)
    {
        original.insert(x[i],y[i]);
        if(y[i] > ymax)
        {
            ymax = y[i];
        }
        if(y[i] < ymin)
        {
            ymin = y[i];
        }

        if(original_x[i] - original_x[i-1] != spacing)
        {
            // TO_DO
            //emit problem
        }
    }

    reset();
}

Signal::Signal(const Signal& other)
{
    this->original = other.original;
    this->spacing = other.spacing;

    this->extended_x = other.extended_x;
    this->extended_y = other.extended_y;

    this->copies_left = other.copies_left;
    this->copies_right = other.copies_right;

    this->ymax = other.ymax;
    this->ymin = other.ymin;

    if(this->extended_x.empty() || this->extended_y.empty())
    {
        this->reset();
    }
}

Signal Signal::operator=(const Signal& other)
{
    this->original = other.original;
    this->spacing = other.spacing;

    this->extended_x = other.extended_x;
    this->extended_y = other.extended_y;
    this->copies_left = other.copies_left;
    this->copies_right = other.copies_right;

    this->ymax = other.ymax;
    this->ymin = other.ymin;

    if(this->extended_x.empty() || this->extended_y.empty())
    {
        this->reset();
    }

    return *this;
}

void Signal::reset()
{
    copies_left = 0;
    copies_right = 0;
    extended_x.clear();
    extended_y.clear();

    extended_x = original.keys().toVector();
    extended_y = original.values().toVector();
}

bool Signal::load_file(const std::string& filename)
{
    ymax = std::numeric_limits<double>::min();
    ymin = std::numeric_limits<double>::max();

    original.clear();

    std::ifstream file(filename);
    if(!file.is_open())
    {
        return false;
    }

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
            return false;
        }
        if (!(linestream >> y))
        {
            return false;
        }

        original.insert(x,y);

        if(y > ymax)
        {
            ymax = y;
        }
        if(y < ymin)
        {
            ymin = y;
        }

        if(linestream >> x)
        {
            return false;
        }
    }

    if(original.isEmpty())
    {
        spacing = 1;
        ymin = -1;
        ymax = 1;
    }
    else if(original.size() > 1)
    {
        QMap<double, double>::iterator iter = original.begin();
        spacing = (iter + 1).key() - iter.key();
        iter += 2;
        while(iter != original.end())
        {
            if(iter.key() - (iter - 1).key() != spacing)
            {
                // emit problem
            }
            iter++;
        }
    }
    else
    {
        spacing = 1;
    }

    reset();

    return true;
}

bool Signal::save_file(const  std::string& filename)
{
    std::ofstream file(filename);
    if(!file.is_open())
    {
        return false;
    }

    if(file.good())
    {
        file << "# Function file generated by FTutor1D" << std::endl;
    }
    else return false;


    for(QMap<double,double>::iterator iter = original.begin(); iter != original.end(); iter++)
    {
        if(file.good())
        {
            file << iter.key() << " " << iter.value() << std::endl;
        }
        else return false;
    }
    file.close();
    return true;
}




void Signal::extend_left()
{
    if(original_length() == 0) return;

    QVector<double> sig_x = original.keys().toVector();

    for(int i = original_length(); i > 0; i--)
    {
        extended_x.push_front(sig_x[i - 1] - (copies_left + 1) * (original_range_x() + spacing));
        extended_y.push_front(original[sig_x[i - 1]]);
    }

    copies_left++;
}


void Signal::extend_right()
{
    if(original_length() == 0) return;

    QVector<double> sig_x = original.keys().toVector();

    for(int i = 0; i < original_length(); i++)
    {
        extended_x.push_back(sig_x[i] + (copies_right + 1) * (original_range_x() + spacing));
        extended_y.push_back(original[sig_x[i]]);
    }
    copies_right++;
}


void Signal::shrink_left()
{
    if(original_length() == 0) return;
    for(int i = 0; i < original_length(); i++)
    {
        extended_x.pop_front();
        extended_y.pop_front();
    }
    copies_left--;
}


void Signal::shrink_right()
{
    if(original_length() == 0) return;
    for(int i = 0; i < original_length(); i++)
    {
        extended_x.pop_back();
        extended_y.pop_back();
    }
    copies_right--;
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

    const double normalization = 1.0 / sqrt(input.size());

    for(auto& i : result)
    {
        i *= normalization;
    }
    return result;
}


QVector<double >  Signal::ifft(QVector<std::complex<double> > input, bool resultReal)
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
        if(resultReal)
        {
            real.push_back(result[i].real());
        }
        else
        {
            real.push_back(result[i].imag());
        }
    }

    return real;
}


void Signal::fourierTransform(Signal& input, Signal& magnitudeSignal, Signal& phaseSignal)
{
    if(input.empty())
    {
        magnitudeSignal = input;
        phaseSignal = input;
        return;
    }
    QVector<std::complex<double> > complex = input.fft(input.original.values().toVector());
    QVector<double> magnitude;
    QVector<double> phase;

    complexToMagAndPhase(complex,magnitude,phase);

    double maxmag = -std::numeric_limits<double>::max();
    double minmag = std::numeric_limits<double>::max();
    double maxpha = -std::numeric_limits<double>::max();
    double minpha = std::numeric_limits<double>::max();

    for(int i = 0; i < magnitude.size(); i++)
    {
        if(magnitude[i] < minmag)
        {
            minmag = magnitude[i];
        }
        if(magnitude[i] > maxmag)
        {
            maxmag = magnitude[i];
        }

        if(phase[i] < minpha)
        {
            minpha = phase[i];
        }
        if(phase[i] > maxpha)
        {
            maxpha = phase[i];
        }
    }

    magnitudeSignal.original.clear();
    phaseSignal.original.clear();

    int i = 0;
    for(QMap<double,double>::iterator iter = input.original.begin(); iter != input.original.end(); iter++,i++)
    {
        magnitudeSignal.original.insert(iter.key(), magnitude[i]);
        phaseSignal.original.insert(iter.key(), phase[i]);
    }

    magnitudeSignal.ymin = minmag;
    magnitudeSignal.ymax = maxmag;
    magnitudeSignal.spacing = 1;
    magnitudeSignal.reset();

    phaseSignal.ymin = minpha;
    phaseSignal.ymax = maxpha;
    phaseSignal.spacing = 1;
    phaseSignal.reset();
}

void Signal::inverseFourierTransform(Signal& magnitude, Signal& phase, Signal& output, bool outputReal)
{
    if(magnitude.empty())
    {
        output = magnitude;
        return;
    }
    QVector<std::complex<double> > complex;
    magAndPhaseToComplex(magnitude.original.values().toVector(),phase.original.values().toVector(),complex);

    QVector<double> real = output.ifft(complex,outputReal);


    double max = -std::numeric_limits<double>::max();
    double min = std::numeric_limits<double>::max();

    for(int i = 0; i < real.size(); i++)
    {
        if(real[i] < min)
        {
            min = real[i];
        }
        if(real[i] > max)
        {
            max = real[i];
        }
    }

    int i = 0;
    for(QMap<double,double>::iterator iter = magnitude.original.begin(); iter != magnitude.original.end(); iter++,i++)
    {
        output.original.insert(iter.key(), real[i]);
    }

    output.ymin = min;
    output.ymax = max;
    output.spacing = magnitude.spacing; // no work

    output.reset();

}

Signal Signal::filtered(Signal& input, Signal& filter)
{
    Signal filteredSignal;

    filteredSignal.spacing = input.spacing;

    if(input.original_length() != filter.original_length())
    {
        return filteredSignal;
    }

    filteredSignal.ymax = std::numeric_limits<double>::min();
    filteredSignal.ymin = std::numeric_limits<double>::max();


    QMap<double,double>::iterator inputIterator;
    QMap<double,double>::iterator filterIterator;
    for(inputIterator = input.original.begin(), filterIterator = filter.original.begin(); inputIterator != input.original.end(); inputIterator++, filterIterator++)
    {
        double value = inputIterator.value() * filterIterator.value();
        filteredSignal.original.insert(inputIterator.key(), value);
        if(value > filteredSignal.ymax)
        {
            filteredSignal.ymax = value;
        }
        if(value < filteredSignal.ymin)
        {
            filteredSignal.ymin = value;
        }
    }

    filteredSignal.extended_x = filteredSignal.original.keys().toVector();
    filteredSignal.extended_y = filteredSignal.original.values().toVector();

    for(int i = 0; i < input.copies_left; i++)
    {
        filteredSignal.extend_left();
    }
    for(int i = 0; i < input.copies_right; i++)
    {
        filteredSignal.extend_right();
    }
    return filteredSignal;
}

void Signal::complexToMagAndPhase(const QVector<std::complex<double> > &complex, QVector<double> &magnitude, QVector<double> &phase)
{
    magnitude.clear();
    phase.clear();
    magnitude.reserve(complex.size());
    phase.reserve(complex.size());

    for(std::complex<double> c : complex)
    {
        magnitude.push_back(sqrt(c.real() * c.real() + c.imag() * c.imag()));
        phase.push_back(atan2(c.imag(), c.real()));
    }
}

void Signal::magAndPhaseToComplex(const QVector<double> &magnitude, const QVector<double> &phase, QVector<std::complex<double> > &complex)
{
    complex.clear();
    complex.reserve(magnitude.size());

    for(int i = 0; i < magnitude.size(); i++)
    {
        complex.push_back(magnitude[i] * exp(phase[i] * std::complex<double>(0,1)));
    }
}

Signal Signal::makeImpulse(unsigned int x, double y)
{
    Signal s;

    for(int i = 0; i < original.size(); i++)
    {
        s.original.insert(i, 0);
    }

    s.original[x] = y;
    s.spacing = spacing;

    s.reset();
    return s;
}

void Signal::zeroSignal(int length, double spacing, double start)
{
    original.clear();

    for(int i = 0; i < length; i++)
    {
        original.insert(start + i*spacing, 0);
    }

    reset();
}


void Signal::sinusWave(int frequency, int xLength, int numberPoints)
{
    ymax = 1;
    ymin = -1;

    original.clear();

    double numberPointsTotal = frequency == 0 ? numberPoints : fabs(frequency) * numberPoints;
    spacing = (double)xLength / numberPointsTotal;

    if(frequency != 0)
    {
        for(int i = 0; i <= numberPointsTotal; i++)
        {
            original.insert(spacing * i, sin(frequency * 2.0*M_PI/ numberPointsTotal * i ));
        }
    }
    else
    {
        for(int i = 0; i<numberPoints; i++)
        {
            original.insert((double)xLength / (double)numberPoints * i, 0);
        }
    }
    reset();
}


void Signal::cosinusWave(int frequency, int xLength, int numberPoints)
{
    ymax = 1;
    ymin = -1;

    original.clear();

    double numberPointsTotal = frequency == 0 ? numberPoints : fabs(frequency) * numberPoints;
    spacing = (double)xLength / numberPointsTotal;

    if(frequency != 0)
    {
        for(int i = 0; i <= numberPointsTotal; i++)
        {
            original.insert(spacing * i,cos(frequency * 2.0*M_PI/ numberPointsTotal * i ));
        }
    }
    else
    {
        for(int i = 0; i<numberPoints; i++)
        {
            original.insert((double)xLength / (double)numberPoints * i, 1);

        }
    }
    reset();
}



void Signal::updateAll(double x, int index, double value)
{
    *(original.begin() + index) = value;

    int length = original_length();
    // we have totally copies_left + 1 (original) + copies_right copies.
    for(int i = 0; i <= copies_left + copies_right; i++)
    {
        extended_y[i * length + index] = value;
    }

    if(value < ymin)
    {
        ymin = value;
    }
    if(value > ymax)
    {
        ymax = value;
    }
}

int Signal::getOriginalIndex(double x)
{
    QVector<double> toSearch;

    if(x < original_min_x() || x > original_max_x())
    {
        toSearch = extended_x;
    }
    else
    {
        toSearch = original.keys().toVector();
    }

    //double delta = avg_dx() * 0.01;

    for(int i = 0; i < toSearch.length(); i++)
    {
        if(toSearch[i] == x)
        {
            return i % original_length();
        }
    }
    return -1;
}
