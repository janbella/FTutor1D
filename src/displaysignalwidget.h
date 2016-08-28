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
    NO_INTERACTION,         // no viewport changing, no plot editing
    BASIC,                  // allows viewport changing
    BASIC_INTERACTION,      // allows viewport changing and moving points in vertical direction
    EDIT_MODE               // a special mode intended for adding and removing points
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
     * @param allowEditMode
     * @param parent
     */
    explicit DisplaySignalWidget(enum DisplaySignalWidgetType type, bool allowEditMode, QWidget *parent = 0);

    /**
     * @brief ~DisplaySignalWidget destructor
     */
    virtual ~DisplaySignalWidget();

    /**
     * @brief displaySignal
     * @param signal
     */
    void displaySignal(Signal* signal);

    /**
     * @brief displayFrequency
     * @param points
     * @param lines
     */
    void displayFrequency(Signal* points, Signal* lines);

    /**
     * @brief plotReplot replots the plot.
     */
    inline void plotReplot()
    {
        plot->replot();
    }


    inline void setAutoScaling(bool val)
    {
        actionAutoScaling->setChecked(val);
    }

    /**
     * @brief setDefaultTexts sets defaults values to each text or title or label in the window.
     */
    void setDefaultTexts();

    /**
     * @brief setLocalizedTexts sets text, title or label values according to given Translation object`
     * @param language Translation object used to set texts
     */
    void setLocalizedTexts(const Translation* language);


signals:
    void mouseMoved(double x, double y);
    void needUpdateFiltered();
    void callForSaveState();
    void openEditMode();


public slots:
    void plotDefaultScale();
    void displayWithLines(bool value);
    void enableCentering(bool enabled);


private slots:

    void plotXAxisChanged(const QCPRange& range);
    void plotYAxisChanged(const QCPRange& range);
    void plotMouseWheel();
    void plotMousePress(QMouseEvent* event);
    void plotMouseRelease(QMouseEvent* event);
    void plotMouseMove(QMouseEvent* event);
    void contextMenuRequest(QPoint pos);

    void editModePlotMousePress(QMouseEvent* event);
    void editModePlotMouseRelease(QMouseEvent* event);
    void editModePlotMouseMove(QMouseEvent* event);

private:
    void placePlotBackground(QCPItemRect*& section);


private:    // attributes

    enum DisplaySignalWidgetType type;

    bool centering;

    QCustomPlot *plot;

    Signal* p_signal;

    QCPItemRect* plotBackground;

    QCPItemLine* verticalLine;

    double selected_point_x;
    bool haveSelectedPoint;

    int signalSelectedPointIndex;

    QAction* actionDisplayLines;
    QAction* actionDefaultScale;
    QAction* actionAutoScaling;
    QAction* actionEditMode;
};

#endif // DISPLAYSIGNALWIDGET_H

