#ifndef DISPLAYSIGNALWIDGET_H
#define DISPLAYSIGNALWIDGET_H

/**
 *@file displaysignalwidget.cpp
 *@author Ján Bella <xbella1@fi.muni.cz>
 *
 * The file contains defintion of display widget. Basically a plot with callbacks
 **/

#include "glob_includes.h"

#include "qcustomplot/qcustomplot.h"
#include "signal.h"
#include "localization.h"

/**
 * @brief The DisplaySignalWidgetType enum defines the type of the widget. Behavior of the widget is specific for each type
 */
enum DisplaySignalWidgetType
{
    FREQUENCY_NO_INTERACTION,  // no viewport changing, no plot editing
    BASIC,                     // allows viewport changing
    BASIC_INTERACTION,         // allows viewport changing and moving points in vertical direction
    EDIT_MODE                  // a special mode intended for adding and removing points
};

/**
 * @brief The Domain enum defines, whether the displayed points are in frequency or time domain. Affects axis' labels
 */
enum Domain
{
    FREQUENCY,
    TIME
};

/**
 * @brief The DisplaySignalWidget class
 */
class DisplaySignalWidget : public QWidget
{
    Q_OBJECT

public:

    /**
     * @brief DisplaySignalWidget constructor
     * @param type
     * @param space
     * @param allowEditMode
     * @param parent
     */
    explicit DisplaySignalWidget(enum DisplaySignalWidgetType type, enum Domain space, bool allowEditMode, QWidget *parent = 0);

    /**
     * @brief ~DisplaySignalWidget destructor
     */
    virtual ~DisplaySignalWidget();

    /**
     * @brief displaySignal
     * @param signal
     */
    void displaySignal(Signal* signal, bool shadowPrevious = false);

    /**
     * @brief displayFrequency
     * @param points
     * @param lines
     */
    void displayFrequency(Signal* points, Signal* lines);

    /**
     * @brief plotReplot replots the plot.
     */
    inline void plotReplot() { plot->replot(); }

    /**
     * @brief setAutoScaling
     * @param val
     */
    inline void setAutoScaling(bool val) { actionAutoScaling->setChecked(val); }

    /**
     * @brief setDefaultTexts sets defaults values to each text or title or label in the window.
     */
    void setDefaultTexts();

    /**
     * @brief setLocalizedTexts sets text, title or label values according to given Translation object`
     * @param language Translation object used to set texts
     */
    void setLocalizedTexts(const Translation* language);

    /**
     * @brief setInteractionsEnabled disable or
     * @param val
     */
    void setInteractionsEnabled(bool val);

    void setSibling(DisplaySignalWidget*& other);

signals:
    void mouseMoved(double x, double y);
    void needUpdateFiltered();
    void editModeNeedUpdate();
    void callForSaveState();
    void callForSaveEditModeState();
    void openEditMode();
    void displayValue(int index);
    void mouseLeave();


public slots:
    void plotDefaultScale();
    void displayWithLines(bool value);
    void enableCentering(bool enabled);


private slots:

    void plotXAxisChanged(const QCPRange& range);
    void plotYAxisChanged(const QCPRange& range);

    void plotMouseWheel(QWheelEvent* event);
    void plotMousePress(QMouseEvent* event);
    void plotMouseRelease(QMouseEvent* event);
    void plotMouseMove(QMouseEvent* event);
    void contextMenuRequest(QPoint pos);

    void editModePlotMousePress(QMouseEvent* event);
    void editModePlotMouseRelease(QMouseEvent* event);
    void editModePlotMouseMove(QMouseEvent* event);

private:
    void placePlotBackground(QCPItemRect*& section);
    double roundToClosestMultiple(double toRound, double base);

    bool event(QEvent* e) override;


private:    // attributes

    enum DisplaySignalWidgetType type;

    QCustomPlot *plot;

    Signal* p_signal;
    Signal* shadow_signal;

    bool centering;

    QCPItemRect* plotBackground;
    QCPItemLine* verticalLine;

    bool haveSelectedPoint;
    double selectedPointX;

    QAction* actionDisplayLines;
    QAction* actionDefaultScale;
    QAction* actionAutoScaling;
    QAction* actionEditMode;

    QLabel* plotxAxisLabel;
    QLabel* plotyAxisLabel;

    DisplaySignalWidget* sibling;
};

#endif // DISPLAYSIGNALWIDGET_H

