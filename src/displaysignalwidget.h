#ifndef DISPLAYSIGNALWIDGET_H
#define DISPLAYSIGNALWIDGET_H

/**
 *@file displaysignalwidget.cpp
 *@author Ján Bella <xbella1@fi.muni.cz>
 *
 * The file contains defintion of display widget. Basically a plot with callbacks
 **/

#include "qcustomplot/qcustomplot.h"
#include "signal.h"
#include "localization.h"

namespace FT1D
{

/**
 * @brief The DisplaySignalWidgetType enum defines the type of the widget. Behavior of the widget is specific for each type
 */
enum DisplaySignalWidgetType
{
    ORIGINAL,   // the original signal plot. Has option to open edit mode, but signal in it cannot be edited.
    FILTERED,   // signal after applying a filter, or modifying the fourier coefficients. No other interaction than rescale.
    MAGNITUDE,  // a plot of magnitudes of fourier coefficients. These can be modified.
    PHASE,      // a plot of arguments of fourier coefficients. These can be modified.
    EDIT_MODE   // a special mode intended for adding and removing points
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
     * @param type type of the widget
     * @param allowEditMode whether edit mode can be triggered from this instance
     * @param parent parent object (MainWindow)
     */
    explicit DisplaySignalWidget(enum DisplaySignalWidgetType type, bool allowEditMode, QWidget *parent = 0);

    /**
     * @brief ~DisplaySignalWidget destructor
     */
    virtual ~DisplaySignalWidget();

    /**
     * @brief displaySignal displays the signal @a signal in the plot in the widget
     * @param signal signal to display
     * @param shadowPrevious if set to true, then previous signal is kept in the graph in gray colour until the next repaint
     */
    void displaySignal(Signal* signal, bool shadowPrevious = false);


    /**
     * @brief plotReplot replots the plot.
     */
    inline void plotReplot() { plot->replot(); }

    /**
     * @brief setAutoScaling
     * @param val if true, auto rescale after applying a specific operation is enabled.
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
     * @brief setInteractionsEnabled disables or enables interaction in the widget
     * @param val true to enable, false to disable.
     */
    void setInteractionsEnabled(bool val);

    /**
     * @brief setSibling sets a pointer to a sibling widget. This widget is rescaled together with this
     * @param other a valid pointer to another widget. Causion: pointer validity is not checked!
     */
    void setSibling(DisplaySignalWidget*& other);

    /**
     * @brief forceXAxisUpdate Artificially triggers plotXAxisChanged callback
     */
    inline void forceXAxisUpdate()
    {
        plotXAxisChanged(plot->xAxis->range());
    }

    // public signals in this widget.
signals:
    /**
     * @brief needFrequencyUpdate mouse was moved so that it points to value at index @idx in the original signal.
     * @param idx index of a point
     * @param value value, which is currently at @a idx
     */
    void needFrequencyUpdate(int idx, double value);

    /**
     * @brief needUpdateFiltered request recomputing and redrawing the filtered graph, usually when the selected point was edited
     */
    void needUpdateFiltered();

    /**
     * @brief editModeNeedUpdate request recomputing and redrawing all graphs based on current edit mode state
     */
    void editModeNeedUpdate();

    /**
     * @brief callForSaveState request recording the current signals
     */
    void callForSaveState();

    /**
     * @brief callForSaveEditModeState request recording the current edit mode state
     */
    void callForSaveEditModeState();

    /**
     * @brief openEditMode notifies the application that an action to open edit mode was triggered.
     */
    void openEditMode();

    /**
     * @brief displayValueStatusBar notifies the main window that the value at index @a index should be displayed in the status bar.
     * @param x x coordinate of the value at index @a index
     * @param index
     */
    void displayValueStatusBar(int x, int index);

    /**
     * @brief mouseLeave notifies the application that the mouse left the widget.
     */
    void mouseLeave();


public slots:
    /**
     * @brief plotDefaultScale rescale the plot so that the original part of the signal just fits the plot
     */
    void plotDefaultScale();

    /**
     * @brief displayWithLines
     * @param value if true, draw graph as a polyline
     */
    void displayWithLines(bool value);

    /**
     * @brief enableCentering
     * @param enabled if true, draw the graph such that the signal displayed is centered
     */
    void enableCentering(bool enabled);


private slots:

    /**
     * @brief plotXAxisChanged handle rescaling the x axis
     * @param range
     */
    void plotXAxisChanged(const QCPRange& range);
//    void plotYAxisChanged(const QCPRange& range);

    /**
     * @brief plotMouseWheel handle when the mouse wheel is used over the plot
     * @param event
     */
    void plotMouseWheel(QWheelEvent* event);

    /**
     * @brief plotMousePress handle when the mouse button is pressed over the plot
     * @param event
     */
    void plotMousePress(QMouseEvent* event);

    /**
     * @brief plotMouseRelease handle releasing the mouse button over the plot
     * @param event
     */
    void plotMouseRelease(QMouseEvent* event);

    /**
     * @brief plotMouseMove handle mouse moving over the plot
     * @param event
     */
    void plotMouseMove(QMouseEvent* event);

    /**
     * @brief contextMenuRequest handle request to show the context menu
     * @param pos
     */
    void contextMenuRequest(QPoint pos);

    /**
     * @brief editModePlotMousePress handle mouse press in the edit mode
     * @param event
     */
    void editModePlotMousePress(QMouseEvent* event);

    /**
     * @brief editModePlotMouseRelease handle mouse release in the edit mode
     * @param event
     */
    void editModePlotMouseRelease(QMouseEvent* event);

    /**
     * @brief editModePlotMouseMove handle mouse move in the edit mode
     * @param event
     */
    void editModePlotMouseMove(QMouseEvent* event);

private:
    /**
     * @brief placePlotBackground position the background correctly, so that the signal is centered or not.
     * @param section background rectangle
     */
    void placePlotBackground(QCPItemRect*& section);


    /**
     * @brief roundToClosestMultiple rounds @a toRound to the closes multiple of @a base
     * @param toRound
     * @param base
     * @return closest multiple of @a toRound w.r.t. @a base
     */
    double roundToClosestMultiple(double toRound, double base);

    /**
     * @brief event overriding a general event to enable mouse leave callback
     * @param e
     * @return
     */
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

    // DisplaySignalWidget, which zooms and changes the visible area together with this
    DisplaySignalWidget* sibling;
};
}
#endif // DISPLAYSIGNALWIDGET_H

