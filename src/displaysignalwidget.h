#ifndef DISPLAYSIGNALWIDGET_H
#define DISPLAYSIGNALWIDGET_H

/**
 *@file displaysignalwidget.cpp
 *@author Ján Bella <xbella1@fi.muni.cz>
 *
 * The file contains defintion of display widget. A plot basically with defined behaviour
 **/

#include "glob_includes.h"

#include "qcustomplot/qcustomplot.h"
#include "signal.h"
#include "localization.h"


class DisplaySignalWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DisplaySignalWidget(bool displayLabel, bool withBackground = false, QWidget *parent = 0);
    virtual ~DisplaySignalWidget();

    void displaySignal(Signal* signal);

    void displayFrequency(Signal* points, Signal* lines);

    void setLabel(QString text);

    void clear();

    inline void plotReplot()
    {
        plot->replot();
    }



public slots:
    void contextMenuRequest(QPoint pos);
    virtual void plotDefaultScale();
    void displayWithLines(bool value);


protected slots:

    void plotXAxisChanged(const QCPRange& range);
    void plotYAxisChanged(const QCPRange& range);
    void plotMouseWheel();
    virtual void plotMousePress(QMouseEvent* event);

private:
    bool hasLabel;
    bool hasBackground;
    QCPItemRect* plotBackground;

    virtual void placePlotBackground(QCPItemRect*& section);

    QAction* actionDisplayLines;
    QAction* actionDefaultScale;
    QAction* actionAutoScaling;

protected:

     QLabel *label;
     Signal* p_signal;


public:
     QCustomPlot *plot;

     /**
      * @brief setDefaultTexts sets defaults values to each text or title or label in the window.
      */
     void setDefaultTexts();

     /**
      * @brief setLocalizedTexts sets text, title or label values according to given Translation object`
      * @param language Translation object used to set texts
      */
     void setLocalizedTexts(const Translation* language);


};

#endif // DISPLAYSIGNALWIDGET_H

