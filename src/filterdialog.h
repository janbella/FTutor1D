#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

/**
 * @file filterdialog.h
 * @author Ján Bella <xbella1@fi.muni.cz>
 */

#include "localization.h"
#include "qcustomplot/qcustomplot.h"
#include "signal.h"


namespace FT1D
{

/**
 * @brief The FilterType enum denotes type of the filter
 */
enum FilterType
{
    ILPF,           // ideal low-pass
    IHPF,           // ideal high-pass
    LPGAUSS,        // gaussian low-pass
    HPGAUSS,        // gaussian high-pass
    LPBUTTERWORTH,  // butterworth low-pass
    HPBUTTERWORTH,  // butterworth high-pass
    BANDPASS        // ideal band-pass
};

/**
 * @brief The FilterDialog class is a window to setup a filter.
 */
class FilterDialog : public QDialog
{
    Q_OBJECT

public:

    /**
     * @brief FilterDialog constructor creates the filter dialog window
     * @param type type of filter, for which the window is created
     * @param magnitude magnitude of the signal, to which we will apply the filter
     * @param language a translation for this window
     * @param parent a parent object, typically MainWindow
     */
    explicit FilterDialog(FilterType type, Signal& magnitude, const Translation* language, QWidget *parent = nullptr);

    /**
     * Desctructor
     */
    virtual ~FilterDialog();

private:

// attributes
    QLabel *filterName;
    QLabel *filterFormula;

    QLabel *firstPreferenceLabel;
    QLabel* secondPreferenceLabel;

    QLineEdit* firstLineEdit;
    QLineEdit* secondLineEdit;

    QSlider *firstSlider;
    QSlider *secondSlider;

    QLabel* firstFrontLabel;
    QLabel* firstBackLabel;

    QLabel* secondFrontLabel;
    QLabel* secondBackLabel;

    QPushButton* okButton;
    QPushButton* cancelButton;

    QCustomPlot* plot;

    QLabel* plotxAxisLabel;
    QLabel* plotyAxisLabel;

    QSpinBox* spinBox;

    const Translation*& translation;
    Signal& magnitude;

    // the highest frequency contained in the signal.
    int maxFrequency;

// private methods

    /**
     * @brief initLowPass sets up the window for use with ideal low-pass filter
     */
    void initLowPass();

    /**
     * @brief initHighPass sets up the window for use with ideal high-pass filter
     */
    void initHighPass();

    /**
     * @brief initBandPass sets up the window for use with ideal band-pass filter
     */
    void initBandPass();

    /**
     * @brief initGaussianLowPass sets up the window for use with gaussian low-pass filter
     */
    void initGaussianLowPass();

    /**
     * @brief initGaussianHighPass sets up the window for use with gaussian high-pass filter
     */
    void initGaussianHighPass();

    /**
     * @brief initButterworthLowPass sets up the window for use with butterworth low-pass filter
     */
    void initButterworthLowPass();

    /**
     * @brief initButterworthHighPass sets up the window for use with butterworth high-pass filter
     */
    void initButterworthHighPass();

    /**
     * @brief ilpfGraph plots ideal low-pass filter funtion
     * @param omega0 threshold
     */
    void ilpfGraph(int omega0);

    /**
     * @brief ihpfGraph plots ideal high-pass filter funtion
     * @param omega0 threshold
     */
    void ihpfGraph(int omega0);

    /**
     * @brief bpfGraph plots ideal band-pass filter funtion
     * @param omega1 lower threshold
     * @param omega2 higher threshold
     */
    void bpfGraph(int omega1, int omega2);

    /**
     * @brief glpfGraph plots gaussian low-pass filter funtion
     * @param omega0 spread of the gaussian
     */
    void glpfGraph(int omega0);

    /**
     * @brief ghpfGraph plots gaussian high-pass filter funtion
     * @param omega0 spread of the gaussian
     */
    void ghpfGraph(int omega0);

    /**
     * @brief blpfGraph plots butterworth low-pass filter funtion
     * @param omega0
     * @param n
     */
    void blpfGraph(double omega0, int n);

    /**
     * @brief bhpfGraph plots butterworth high-pass filter funtion
     * @param omega0
     * @param n
     */
    void bhpfGraph(double omega0, int n);

    /**
     * @brief idealLowPassFilter applies ideal low-pass filter to the magnitude signal
     * @param omega0
     */
    void idealLowPassFilter(int omega0);

    /**
     * @brief idealHighPassFilter applies ideal low-pass filter to the magnitude signal
     * @param omega0
     */
    void idealHighPassFilter(int omega0);

    /**
     * @brief bandPassFilter applies ideal band-pass filter to the magnitude signal
     * @param from lower threshold
     * @param to upper threshold
     */
    void bandPassFilter(double from, double to);

    /**
     * @brief gaussianLowPassFilter applies gaussian low-pass filter to the magnitude signal
     * @param omega0
     */
    void gaussianLowPassFilter(double omega0);

    /**
     * @brief gaussianHighPassFilter applies gaussian high-pass filter to the magnitude signal
     * @param omega0
     */
    void gaussianHighPassFilter(double omega0);

    /**
     * @brief butterworthLowPassFilter applies butterworth low-pass filter to the magnitude signal
     * @param omega0
     * @param n
     */
    void butterworthLowPassFilter(double omega0, int n);

    /**
     * @brief butterworthHighPassFilter applies butterworth high-pass filter to the magnitude signal
     * @param omega0
     * @param n
     */
    void butterworthHighPassFilter(double omega0, int n);

    /**
     * @brief setDefaultTexts sets defaults values to each text or title or label in the window.
     */
    void setDefaultTexts(FilterType type);

    /**
     * @brief setLocalizedTexts sets text, title or label values according to given Translation object`
     * @param language Translation object used to set texts
     */
    void setLocalizedTexts(const Translation* language, FilterType type);

};
}
#endif // FILTERDIALOG_H
