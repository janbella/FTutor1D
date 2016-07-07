#ifndef DISPLAYSIGNALWIDGET_H
#define DISPLAYSIGNALWIDGET_H

#include <QFrame>
#include "QCustomPlot/qcustomplot.h"
#include <QScrollBar>
#include <QLabel>

namespace Ui {
class DisplaySignalWidget;
}

class DisplaySignalWidget : public QFrame
{
    Q_OBJECT

public:
    explicit DisplaySignalWidget(QWidget *parent = 0);
    ~DisplaySignalWidget();

private:
     QCustomPlot *plot;
     QScrollBar *verticalScrollBar;
     QScrollBar *horizontalScrollBar;
     QLabel *label;
};

#endif // DISPLAYSIGNALWIDGET_H

