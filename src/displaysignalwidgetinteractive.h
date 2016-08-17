#ifndef DISPLAYSIGNALWIDGETINTERACTIVE_H
#define DISPLAYSIGNALWIDGETINTERACTIVE_H

/**
 *@file displaysignalwidgetinteractive.cpp
 *@author Ján Bella <xbella1@fi.muni.cz>
 *
 * Extended DisplaySignalWidget. Allows for dragging points, centering, has position indicator
 **/


#include "glob_includes.h"

#include "displaysignalwidget.h"


class DisplaySignalWidgetInteractive : public DisplaySignalWidget
{
    Q_OBJECT

public:
    DisplaySignalWidgetInteractive(bool displayLabel, bool hasBackground, QWidget *parent = 0);
    virtual ~DisplaySignalWidgetInteractive();

public slots:
    void plotMouseMove(QMouseEvent* event);
    void enableCentering(bool enabled);
    virtual void plotDefaultScale();


signals:
    void mouseMoved(double x, double y);
    void needUpdateFiltered();
    void callForSaveState();



private slots:
   virtual void plotMousePress(QMouseEvent* event);
   virtual void plotMouseRelease(QMouseEvent* event);

private:
    QCPItemLine* vert;
    bool centering;

    QCPDataMap::iterator selected_point;
    bool haveSelectedPoint;

    int signalSelectedPointIndex;

    virtual void placePlotBackground(QCPItemRect*& section);

};


#endif // DISPLAYSIGNALWIDGETINTERACTIVE_H
