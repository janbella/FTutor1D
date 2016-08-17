#include "displaysignalwidget.h"

DisplaySignalWidget::DisplaySignalWidget(bool displayLabel, bool withBackground, QWidget *parent) :  QWidget(parent)
{
    // does not work in initialisation section.
    p_signal = nullptr;
    hasLabel = displayLabel;
    hasBackground = withBackground;

    QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);

    int shiftWhenNoLabel = -30;

    label = new QLabel(this);
    label->setVisible(false);

    if(hasLabel)
    {
        label->setVisible(true);
        shiftWhenNoLabel = 0;
        label->setGeometry(QRect(0, 5, 470, 20));
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setAlignment(Qt::AlignCenter);
    }

    resize(470, 300);

    plot = new QCustomPlot(this);
    plot->setGeometry(QRect(0, 30 + shiftWhenNoLabel, 470, 270));
    plot->setCursor(QCursor(Qt::CrossCursor));

    if(withBackground)
    {
        plotBackground = new QCPItemRect(plot);
        plotBackground->topLeft->setType(QCPItemPosition::ptPlotCoords);
        plotBackground->topLeft->setAxes(plot->xAxis, plot->yAxis);
        plotBackground->bottomRight->setType(QCPItemPosition::ptPlotCoords);
        plotBackground->bottomRight->setAxes(plot->xAxis, plot->yAxis);
        plotBackground->setBrush(QBrush(QColor(0,200,0,100)));
        plotBackground->setPen(Qt::NoPen);
        plotBackground->setVisible(false);
    }
    else
    {
        plotBackground = nullptr;
    }
    
    //    verticalScrollBar = new QScrollBar(this);
    //    verticalScrollBar->setObjectName(QStringLiteral("verticalScrollBar"));
    //    verticalScrollBar->setGeometry(QRect(450, 30 + shiftWhenNoLabel, 16, 250));
    //    verticalScrollBar->setOrientation(Qt::Vertical);

    //    horizontalScrollBar = new QScrollBar(this);
    //    horizontalScrollBar->setObjectName(QStringLiteral("horizontalScrollBar"));
    //    horizontalScrollBar->setGeometry(QRect(0, 280 + shiftWhenNoLabel, 450, 16));
    //    horizontalScrollBar->setOrientation(Qt::Horizontal);

    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes);

    //    connect(horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(horizontalScrollBarChanged(int)));
    //    connect(verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(verticalScrollBarChanged(int)));

    connect(plot->xAxis, static_cast<void (QCPAxis::*)(const QCPRange& r)>(&QCPAxis::rangeChanged), this, &DisplaySignalWidget::plotXAxisChanged);
    connect(plot->yAxis, static_cast<void (QCPAxis::*)(const QCPRange& r)>(&QCPAxis::rangeChanged), this, &DisplaySignalWidget::plotYAxisChanged);

    connect(plot, &QCustomPlot::mousePress, this, &DisplaySignalWidget::plotMousePress);
    connect(plot, &QCustomPlot::mouseWheel, this, &DisplaySignalWidget::plotMouseWheel);

    plot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(plot, &QCustomPlot::customContextMenuRequested, this,&DisplaySignalWidget::contextMenuRequest);

    plot->xAxis->setRange(0, 10, Qt::AlignCenter);
    plot->yAxis->setRange(0, 10, Qt::AlignCenter);

    plot->yAxis->setNumberFormat("f");
    plot->yAxis->setNumberPrecision(3);

    plot->xAxis->setNumberFormat("g");
    plot->xAxis->setNumberPrecision(3);

    actionDefaultScale = new QAction(this);

    actionDisplayLines = new QAction(this);
    actionDisplayLines->setCheckable(true);
    actionDisplayLines->setChecked(false);

    actionAutoScaling = new QAction(this);
    actionAutoScaling->setCheckable(true);
    actionAutoScaling->setChecked(true);

    // TO DO.
    actionAutoScaling->setEnabled(false);

    connect(actionDefaultScale, &QAction::triggered, this, &DisplaySignalWidget::plotDefaultScale);
    connect(actionDisplayLines, &QAction::triggered, this, &DisplaySignalWidget::displayWithLines);
    connect(actionAutoScaling, &QAction::triggered, this, [=]
    {
        // nothing so far.
    });
}

void DisplaySignalWidget::plotXAxisChanged(const QCPRange& range)
{
    if(p_signal!= nullptr)
    {
        if(range.lower < p_signal->allowed_min_x())
        {
            plot->xAxis->setRangeLower(p_signal->allowed_min_x());
            return;
        }
        if(range.upper > p_signal->allowed_max_x())
        {
            plot->xAxis->setRangeUpper(p_signal->allowed_max_x());
            return;
        }

        if(range.lower < p_signal->min_x() || range.upper > p_signal->max_x())
        {
            while(range.lower < p_signal->min_x())
            {
                p_signal->extend_left();
            }
            while(range.upper > p_signal->max_x())
            {
                p_signal->extend_right();
            }

            if(plot->graph() != nullptr)
            {
                plot->graph()->setData(p_signal->x(), p_signal->y());
                plot->replot();
            }
        }
    }

    ////    if(signal.original_min_x() + signal.original_range_x() <= range.lower || signal.original_max_x() - signal.original_range_x() >= range.upper)
    ////    {
    ////        while(signal.original_min_x() + signal.original_range_x() <= range.lower)
    ////        {
    ////            signal.shrink_left();
    ////        }

    ////        while(signal.original_max_x() - signal.original_range_x() >= range.upper)
    ////        {
    ////            signal.shrink_right();
    ////        }

    ////        if(originalPlot->graph() != nullptr)
    ////        {
    ////            originalPlot->graph()->setData(this->signal.x, this->signal.y);
    ////            originalPlot->replot();
    ////        }
    ////    }
}

void DisplaySignalWidget::plotYAxisChanged(const QCPRange& )
{
    //    if(p_signal != nullptr)
    //    {
    //        if(range.lower < p_signal->ymin - p_signal->original_range_y())
    //        {
    //            plot->yAxis->setRangeLower(p_signal->ymin - p_signal->original_range_y());
    //            return;
    //        }
    //        if(range.upper > p_signal->ymax + p_signal->original_range_y())
    //        {
    //            plot->yAxis->setRangeUpper(p_signal->ymax + p_signal->original_range_y());
    //            return;
    //        }
    //    }
}

void DisplaySignalWidget::plotMousePress(QMouseEvent* )
{
    // if an axis is selected, only allow the direction of that axis to be dragged
    // if no axis is selected, both directions may be dragged

    if (plot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    {
        plot->axisRect()->setRangeDrag(plot->xAxis->orientation());
    }
    else if (plot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    {
        plot->axisRect()->setRangeDrag(plot->yAxis->orientation());
    }
    else
    {
        plot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
    }
}

void DisplaySignalWidget::plotMouseWheel()
{
    // if an axis is selected, only allow the direction of that axis to be zoomed
    // if no axis is selected, both directions may be zoomed

    if (plot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
        plot->axisRect()->setRangeZoom(plot->xAxis->orientation());
    else if (plot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
        plot->axisRect()->setRangeZoom(plot->yAxis->orientation());
    else
        plot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void DisplaySignalWidget::plotDefaultScale()
{
    if(p_signal != nullptr)
    {
        double offset = p_signal->original_range_x() * 0.1;
        plot->xAxis->setRange(p_signal->original_min_x() - offset,p_signal->original_max_x() + offset);

        offset = p_signal->original_range_y() * 0.1;
        plot->yAxis->setRange(p_signal->original_min_y() - offset,p_signal->max_y() + offset);
    }
    plot->replot();
}

void DisplaySignalWidget::contextMenuRequest(QPoint pos)
{
    QMenu *menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    menu->addAction(actionDefaultScale);
    menu->addAction(actionDisplayLines);
    menu->addAction(actionAutoScaling);
    menu->popup(plot->mapToGlobal(pos));
}

void DisplaySignalWidget::displaySignal(Signal* signal)
{
    p_signal = signal;

    plot->clearGraphs();
    plot->clearItems();
    plot->clearPlottables();
    plot->clearFocus();
    plot->clearMask();

    if(p_signal == nullptr)
    {
        return;
    }
    else
    {
        plot->addGraph();

        plot->graph()->setData(signal->x(), signal->y());
        plot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::blue, Qt::blue,5));

        if(actionDisplayLines->isChecked())
        {
            plot->graph()->setPen(QPen(QColor::fromRgb(255,165,0)));
            plot->graph()->setLineStyle(QCPGraph::lsLine);
            plot->graph()->setBrush(Qt::NoBrush);
        }
        else
        {
            plot->graph()->setPen(QPen(Qt::red));
            plot->graph()->setBrush(QBrush(QColor(255, 0, 0, 20)));
            plot->graph()->setLineStyle(QCPGraph::lsNone);
        }


        if(plotBackground)
        {
            plotBackground->setVisible(true);
            placePlotBackground(plotBackground);
        }
        plotDefaultScale();
    }
}


void DisplaySignalWidget::displayFrequency(Signal* points, Signal* lines)
{
    p_signal = points;

    plot->clearGraphs();
    plot->clearItems();
    plot->clearPlottables();
    plot->clearFocus();
    plot->clearMask();

    if(p_signal == nullptr)
    {
        return;
    }
    else
    {
        QCPGraph* pointsGraph = plot->addGraph();

        pointsGraph->setData(points->x(), points->y());
        pointsGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::blue, Qt::blue,5));
        pointsGraph->setLineStyle(QCPGraph::lsNone);

        QCPGraph* linesGraph = plot->addGraph();

        linesGraph->setData(lines->x(), lines->y());
        linesGraph->setScatterStyle(QCPScatterStyle::ssNone);
        linesGraph->setPen(QPen(QColor::fromRgb(255,165,0)));
        linesGraph->setLineStyle(QCPGraph::lsLine);
        linesGraph->setBrush(Qt::NoBrush);

        plotDefaultScale();
    }
}



void DisplaySignalWidget::placePlotBackground(QCPItemRect*& section)
{
    section->topLeft->setCoords(p_signal->original_min_x(),99999999);
    section->bottomRight->setCoords(p_signal->original_max_x(),-99999999);
}


void DisplaySignalWidget::setLabel(QString text)
{
    if(hasLabel)
    {
        label->setText(text);
    }
}

void DisplaySignalWidget::clear()
{
    plot->clearGraphs();
    plot->clearItems();
    p_signal = nullptr;
    plot->clearPlottables();
    plot->clearFocus();
    plot->clearMask();
}


DisplaySignalWidget::~DisplaySignalWidget()
{
    delete label;
    delete plotBackground;
    delete plot;

    delete actionDefaultScale;
    delete actionDisplayLines;
    delete actionAutoScaling;
}

void DisplaySignalWidget::displayWithLines(bool value)
{
    actionDisplayLines->setChecked(value);
    displaySignal(p_signal);
}

void DisplaySignalWidget::setDefaultTexts()
{
    label->setText(QStringLiteral("A graph."));
    actionDefaultScale->setText(QStringLiteral("Default scale in this graph"));
    actionDisplayLines->setText(QStringLiteral("Display with lines in this graph"));
    actionAutoScaling->setText(QStringLiteral("Automatic scaling"));
}


void DisplaySignalWidget::setLocalizedTexts(const Translation* language)
{
    label->setText(language->getChildElementText(QStringLiteral("label")));
    if(label->text().isEmpty()) label->setText(QStringLiteral("A graph."));

    actionDefaultScale->setText(language->getChildElementText("actionDefaultScale"));
    if(actionDefaultScale->text().isEmpty()) actionDefaultScale->setText(QStringLiteral("Default scale in this graph"));

    actionDisplayLines->setText(language->getChildElementText("actionDisplayLines"));
    if(actionDisplayLines->text().isEmpty()) actionDisplayLines->setText(QStringLiteral("Display with lines in this graph"));

    actionAutoScaling->setText(language->getChildElementText("actionAutoScaling"));
    if(actionAutoScaling->text().isEmpty()) actionAutoScaling->setText(QStringLiteral("Automatic scaling"));
}
