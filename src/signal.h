#ifndef SIGNAL_H
#define SIGNAL_H

/**
 * @file signal.h
 * @author Ján Bella <xbella1@fi.muni.cz>
 */

#include <QVector>
#include <QMap>

#include <complex>

#define NUM_COPIES_ALLOWED 3

/**
 * @brief The Signal class represents a signal
 */
class Signal
{
private:

    // extended coordinates contain the original signal, plus its copies.
    QVector<double> extended_x;
    QVector<double> extended_y;

    // these are for setting the y axis default scale
    double ymax;
    double ymin;

    // number of signal copies
    int copies_left;
    int copies_right;


public:
    // original values.
    // UGLY: make it private and provide enough callbacks for manipulation
    QMap<double, double> original;

    // x-axis distance between two consecutive points
    double spacing;

    /**
     * Constructor, creates an empty signal
     **/
    Signal();

    /**
     * @brief Signal constructor, which loads the signal from the given file
     * @param filename path to the file to load
     */
    Signal(const std::string& filename);

    /**
     * @brief Signal construcotr, which takes a pair of x and y points coordinates
     * @param x x-axis coordinates
     * @param y y-axis coordinates
     */
    Signal(const QVector<double>& x, const QVector<double>& y);

    /**
     * @brief Signal copy constructor
     * @param other signal to copy
     */
    Signal(const Signal& other);

    /**
     * @brief operator = copies the signal @a other to this
     * @param other signal to copy
     * @return *this
     */
    Signal operator=(const Signal& other);

    /**
     * @brief load_file loads signal from the given file.
     * @param filename path to the file to load
     * @return true in case of success, false otherwise (e.g. when the file format is invalid)
     */
    bool load_file(const std::string& filename);

    /**
     * @brief save_file saves signal to the specified file
     * @param filename where to save the signal
     * @return true in case of success
     */
    bool save_file(const  std::string& filename) const;

    /**
     * @brief x obtain all x-coordinates
     * @return
     */
    inline QVector<double> x() const
    {
        return extended_x;
    }

    /**
     * @brief y obtain all y-coordinates
     * @return
     */
    inline QVector<double> y() const
    {
        return extended_y;
    }

    /**
     * @brief empty check, whether the signal is empty or not
     * @return
     */
    inline bool empty() const
    {
        return original.isEmpty();
    }

    /**
     * @brief extend_left extend the signal by copying the original part to the left
     */
    void extend_left();

    /**
     * @brief extend_right extend the signal by copyting the original part to the right
     */
    void extend_right();

    /**
     * @brief shrink_left shrink the signal by removing one copy of the original from the left
     */
    void shrink_left();

    /**
     * @brief shrink_right shrink the signal by removing one copy of the original from the right
     */
    void shrink_right();

    /**
     * @brief reset remove all copies
     */
    void reset();

    /**
     * @brief min_x
     * @return minimum x coordinate
     */
    inline double min_x() const
    {
        return extended_x.first();
    }

    /**
     * @brief max_x
     * @return maximum x coordinate
     */
    inline double max_x() const
    {
        return extended_x.last();
    }


    /**
     * @brief min_y
     * @return minimum y coordinate
     */
    inline double min_y() const
    {
        return ymin;
    }

    /**
     * @brief max_y
     * @return maximum y coordinate
     */
    inline double max_y() const
    {
        return ymax;
    }

    /**
     * @brief original_min_x
     * @return minimum x coordinate from the original part of the signal
     */
    inline double original_min_x() const
    {
        if(original.isEmpty()) return 0;
        else return original.firstKey();
    }

    /**
     * @brief original_max_x
     * @return maximum x coordinate from the original part of the signal
     */
    inline double original_max_x() const
    {
        if(original.isEmpty()) return 0;
        else return original.lastKey();
    }

    /**
     * @brief original_min_y
     * @return minimum y coordinate from the original part of the signal
     */
    inline double original_min_y() const
    {
        return ymin;
    }

    /**
     * @brief original_max_y
     * @return maximum y coordinate from the original part of the signal
     */
    inline double original_max_y() const
    {
        return ymax;
    }

    /**
     * @brief range_x
     * @return range of x coordinates
     */
    inline double range_x() const
    {
        if(extended_x.isEmpty())
        {
            return 0;
        }
        return extended_x.last() - extended_x.first();
    }

    /**
     * @brief range_y
     * @return range of y coordinates
     */
    inline double range_y() const
    {
        return (ymax - ymin);
    }

    /**
     * @brief original_range_x
     * @return range of x coordinates of the original part of the signal
     */
    inline double original_range_x() const
    {
        return original_max_x() - original_min_x();
    }

    /**
     * @brief original_range_y
     * @return range of the y coordinates of the original part of the signal
     */
    inline double original_range_y() const
    {
        return ymax - ymin;
    }

    /**
     * @brief allowed_max_x
     * @return maximum allowed x coordinate
     */
    inline double allowed_max_x() const
    {
        return original_max_x() +  NUM_COPIES_ALLOWED*std::max(original_range_x(), spacing);
    }

    /**
     * @brief allowed_min_x
     * @return  minimum allowed x coordinate
     */
    inline double allowed_min_x() const
    {
        return original_min_x() -  NUM_COPIES_ALLOWED*std::max(original_range_x(), spacing);
    }

    /**
     * @brief original_length
     * @return length of the original part of the signal
     */
    inline int original_length() const
    {
        return original.size();
    }

    /**
     * @brief updateAll change value on @a index and all its copies to @a value
     * @param index index of value in original part of signal to change
     * @param value value to change to
     */
    void updateAll(int index, double value);

    /**
     * @brief findYMinMax updates ymin and ymax attributes
     */
    void findYMinMax();

    /**
     * @brief clear clears the signal
     */
    void clear();

    /**
     * @brief applyFilter applies filter @a filter to this signal and returns the result
     * @param filter filter to apply. This is a signal, that must have the same original length as this
     * @return result of filtering
     */
    Signal applyFilter(Signal& filter);

    /**
     * @brief fourierTransform computes the fourier transform of signal @a input
     * @param input signal for which to compute the transform
     * @param magnitude signal of magnitudes of the fourier coefficients
     * @param phase signal of phases of the fourier coefficients
     */
    static void fourierTransform(Signal& input, Signal& magnitude, Signal& phase);

    /**
     * @brief inverseFourierTransform computes the inverse fourier transform of signal @a magnitude and @a phase
     * @param magnitude signal of magnitudes of the fourier coefficients (input)
     * @param phase signal of phases of the fourier coefficients (input)
     * @param output result of the inverse fourier transorm (output)
     * @param x if different x coordinates than 0,1,2,3,... should be used, pass them here
     * @param outputReal whether to take real or imaginary part of the IFT (can be useful for something)
     */
    static void inverseFourierTransform(Signal& magnitude, Signal& phase, Signal& output,  QVector<double> x = QVector<double>(), bool outputReal = true);


private:

    /**
     * @brief fft computes the fourier transform using the fast fourier transform algorithm
     * @param input real input signal
     * @return complex signal of fourier coefficients
     */
    QVector<std::complex<double> >  fft(QVector<double> input);

    /**
     * @brief fft_recursion recursive routine of fast fourier transform algorithm
     * @param input real input signal
     * @return complex signal of fourier coefficients
     */
    QVector<std::complex<double> > fft_recursion(const QVector<double>& input);

    /**
     * @brief ifft computes the fourier transform using the inverse fast fourier transform algorithm
     * @param input complex signal of fourier coefficients
     * @param outputReal whether to output real part of the result
     * @return result
     */
    QVector<double> ifft(QVector<std::complex<double> > input, bool outputReal = true);

    /**
     * @brief ifft_recursion recursive routine of inverse fast fourier transform algorithm
     * @param input complex signal of fourier coefficients
     * @return result
     */
    QVector<std::complex<double> >  ifft_recursion(const QVector<std::complex<double> >& input);

    /**
     * @brief complexToMagAndPhase convert complex signal to real signals of magnitudes and phases
     * @param complex complex signal (input)
     * @param magnitude magnitude signal (output)
     * @param phase phase signal (output)
     */
    static void complexToMagAndPhase(const QVector<std::complex<double> >& complex, QVector<double>& magnitude, QVector<double>& phase);

    /**
     * @brief magAndPhaseToComplex convert magnitude and phase signals the corresponding complex signal
     * @param magnitude signal of magnitudes of fourier coefficients (input)
     * @param phase signal of phases of fourier coefficients (input)
     * @param complex result (output)
     */
    static void magAndPhaseToComplex(const QVector<double>& magnitude, const QVector<double>& phase, QVector<std::complex<double> >& complex);

    /**
     * @brief ensureComplexConjugateness checks the result of the IFFT and "smoothens" it.
     * Small values are thresholded to 0, complex conjugatness is forced by setting complex conjugate's values
     * @param magnitude magnitude signal to check
     * @param phase phase signal to check
     */
    static void ensureComplexConjugateness(QVector<double> &magnitude, QVector<double> &phase);

};

#endif // SIGNAL_H
