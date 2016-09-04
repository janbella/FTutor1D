#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

/**
 * @file filterdialog.h
 * @author Ján Bella <xbella1@fi.muni.cz>
 */


#include "glob_includes.h"
#include "localization.h"
#include "qcustomplot/qcustomplot.h"
#include "signal.h"

enum FilterType
{
    ILPF,
    IHPF,
    LPGAUSS,
    HPGAUSS,
    LPBUTTERWORTH,
    HPBUTTERWORTH,
    BANDPASS
};

class FilterDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief FilterDialog costructor
     * @param parent the parent object, should be MainWindow
     * @param localization the instance of Localization class, which provides translated labels
     */
    explicit FilterDialog(FilterType type, Signal& magnitude, Signal& phase, const Translation* language, QWidget *parent = nullptr);

    /**
     * Desctructor
     */
    virtual ~FilterDialog();

private:

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
    Signal& phase;

    int max;

    void initLowPass();
    void initHighPass();
    void initBandPass();
    void initGaussianLowPass();
    void initGaussianHighPass();
    void initButterworthLowPass();
    void initButterworthHighPass();

    void ilpfGraph(int omega0);
    void ihpfGraph(int omega0);
    void bpfGraph(int omega1, int omega2);
    void glpfGraph(int omega0);
    void ghpfGraph(int omega0);
    void blpfGraph(double omega0, int n);
    void bhpfGraph(double omega0, int n);

    void idealLowPassFilter(int omega0);
    void idealHighPassFilter(int omega0);

    void bandPassFilter(double from, double to);

    void gaussianLowPassFilter(double omega0);
    void gaussianHighPassFilter(double omega0);

    void butterworthLowPassFilter(double omega0, int n);
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


signals:
    void filterApplied();


};

#endif // FILTERDIALOG_H
