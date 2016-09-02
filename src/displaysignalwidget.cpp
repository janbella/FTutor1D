#include "displaysignalwidget.h"

#include<limits>

DisplaySignalWidget::DisplaySignalWidget(DisplaySignalWidgetType type, bool allowEditMode, QWidget *parent) :  QWidget(parent)
{
    // does not work in initialisation section.
    p_signal = nullptr;
    this->type = type;
    centering = false;

    haveSelectedPoint = false;

    QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);

    resize(470, 300);

    plot = new QCustomPlot(this);
    plot->setGeometry(QRect(0, 0, 470, 270));
    plot->setCursor(QCursor(Qt::CrossCursor));

    verticalLine = new QCPItemLine(plot);
    QPen pen;
    pen.setColor( Qt::red );
    verticalLine->setPen(pen );
    verticalLine->setVisible(false);

    // create graph background:

    if(type == FREQUENCY_NO_INTERACTION || type == EDIT_MODE)
    {
        plotBackground = nullptr;
    }
    else
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

    // setup interactions

    if(type == FREQUENCY_NO_INTERACTION)
    {
        plot->setInteractions(QCP::Interactions());
    }
    else if(type == EDIT_MODE)
    {
        plot->setInteractions(QCP::iRangeZoom | QCP::iSelectAxes);
    }
    else
    {
        plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes);
    }

    // setup callbacks
    if(type == BASIC || type == BASIC_INTERACTION)
    {
        connect(plot->xAxis, static_cast<void (QCPAxis::*)(const QCPRange& r)>(&QCPAxis::rangeChanged), this, &DisplaySignalWidget::plotXAxisChanged);
        connect(plot->yAxis, static_cast<void (QCPAxis::*)(const QCPRange& r)>(&QCPAxis::rangeChanged), this, &DisplaySignalWidget::plotYAxisChanged);

        plot->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(plot, &QCustomPlot::customContextMenuRequested, this,&DisplaySignalWidget::contextMenuRequest);

        connect(plot, &QCustomPlot::mouseWheel, this, &DisplaySignalWidget::plotMouseWheel);
    }

    if(type == BASIC_INTERACTION)
    {
        connect(plot, &QCustomPlot::mousePress, this, &DisplaySignalWidget::plotMousePress);
        connect(plot,&QCustomPlot::mouseMove,this,&DisplaySignalWidget::plotMouseMove);
        connect(plot,&QCustomPlot::mouseRelease,this,&DisplaySignalWidget::plotMouseRelease);
    }
    else if(type == EDIT_MODE)
    {
        connect(plot, &QCustomPlot::mousePress, this, &DisplaySignalWidget::editModePlotMousePress);
        connect(plot,&QCustomPlot::mouseMove,this,&DisplaySignalWidget::editModePlotMouseMove);
        connect(plot,&QCustomPlot::mouseRelease,this,&DisplaySignalWidget::editModePlotMouseRelease);
    }


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

    connect(actionDefaultScale, &QAction::triggered, this, &DisplaySignalWidget::plotDefaultScale);
    connect(actionDisplayLines, &QAction::triggered, this, &DisplaySignalWidget::displayWithLines);
    connect(actionAutoScaling, &QAction::triggered, this, [=]
    {
        // nothing so far.
    });


    if(allowEditMode)
    {
        actionEditMode = new QAction(this);
        connect(actionEditMode, &QAction::triggered, this, [=]
        {
            emit openEditMode();
        });
    }
    else
    {
        actionEditMode = nullptr;
    }
}


void DisplaySignalWidget::plotXAxisChanged(const QCPRange& range)
{
    if(p_signal!= nullptr && type != EDIT_MODE && !p_signal->empty())
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
}


void DisplaySignalWidget::plotYAxisChanged(const QCPRange& )
{
    // nothing
}

void DisplaySignalWidget::plotMousePress(QMouseEvent* event)
{
    // there was some code for range dragging when axis selected.

    if(!plot->graph() || p_signal == nullptr)
    {
        return;
    }

    double x = plot->xAxis->pixelToCoord(event->pos().x());
    double y = plot->yAxis->pixelToCoord(event->pos().y());

    double delta = p_signal->avg_dx() * 0.25;

    QCPDataMap::iterator u = plot->graph()->data()->lowerBound(x);
    QCPDataMap::iterator l = (u == plot->graph()->data()->begin()) ? u : (u-1);
    u = ( u == plot->graph()->data()->end()) ? l : u;

    if ((fabs(u->key - x) < delta) && (fabs(u->value - y) < delta) )
    {
        selected_point_x = u.key();
        haveSelectedPoint = true;
        // selected u
    }
    else if ( (fabs(l->key - x) < delta) && (fabs(l->value - y) < delta) )
    {
        selected_point_x = l.key();
        haveSelectedPoint = true;
        // selected l
    }
    if(haveSelectedPoint)
    {
        signalSelectedPointIndex = p_signal->getOriginalIndex(selected_point_x);
        plot->setInteraction(QCP::iRangeDrag, false);
        emit callForSaveState();
    }
}

void DisplaySignalWidget::plotMouseWheel()
{
    // if an axis is selected, only allow the direction of that axis to be zoomed
    // if no axis is selected, both directions may be zoomed

    if (plot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    {
        plot->axisRect()->setRangeZoom(plot->xAxis->orientation());
    }
    else if (plot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    {
        plot->axisRect()->setRangeZoom(plot->yAxis->orientation());
    }
    else
    {
        plot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
    }
}



void DisplaySignalWidget::plotDefaultScale()
{
    if(p_signal != nullptr)
    {
        if(p_signal->original_length() == 1 && type == FREQUENCY_NO_INTERACTION)
        {
            plot->xAxis->setRange(-0.1,1);
        }
        else
        {
        double offset = p_signal->original_range_x() * 0.1;
        //double offset = 0;
        if(p_signal->range_x() < 0.000001)
        {
            offset = 0.5;
        }

        if(centering)
        {
            plot->xAxis->setRange(p_signal->original_min_x() - p_signal->original_range_x() / 2.0 - offset, p_signal->original_max_x() + p_signal->original_range_x() / 2.0 + offset);
        }
        else
        {
            plot->xAxis->setRange(p_signal->original_min_x() - offset,p_signal->original_max_x() + offset);

        }
        }

        if(type == FREQUENCY_NO_INTERACTION)
        {
            plot->yAxis->setRange(-1.2,1.2);
        }
        else
        {
            double offset = p_signal->original_range_y() * 0.1;
            //offset = 0;
            if(p_signal->range_y() < 0.000001)
            {
                offset = 0.5;
            }
            plot->yAxis->setRange(p_signal->original_min_y() - offset,p_signal->original_max_y() + offset);
        }
        plot->replot();
    }
}

void DisplaySignalWidget::contextMenuRequest(QPoint pos)
{
    QMenu *menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    menu->addAction(actionDefaultScale);
    menu->addAction(actionDisplayLines);
    menu->addAction(actionAutoScaling);
    if(actionEditMode)
    {
        menu->addAction(actionEditMode);
    }
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

        if(type == EDIT_MODE)
        {
            plot->graph()->setData(signal->original.keys().toVector(), signal->original.values().toVector());
            plot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::blue, Qt::blue,5));
        }
        else
        {
            plot->graph()->setData(signal->x(), signal->y());
            plot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::blue, Qt::blue,5));
        }

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
        if(actionAutoScaling->isChecked())
        {
            plotDefaultScale();
            plotXAxisChanged(plot->xAxis->range());
        }
        else
        {
            plotXAxisChanged(plot->xAxis->range());
            plot->replot();
        }
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
    if(centering)
    {
        section->topLeft->setCoords(p_signal->original_min_x() - p_signal->original_range_x() / 2.0,QCPRange::maxRange);
        section->bottomRight->setCoords(p_signal->original_min_x() + p_signal->original_range_x() / 2.0,QCPRange::minRange);
    }
    else
    {
        section->topLeft->setCoords(p_signal->original_min_x(),999999999);
        section->bottomRight->setCoords(p_signal->original_max_x(),-999999999);
    }
}


void DisplaySignalWidget::enableCentering(bool enabled)
{
    centering = enabled;

    if(p_signal != nullptr)
    {
        displaySignal(p_signal);
    }
}


DisplaySignalWidget::~DisplaySignalWidget()
{
    delete verticalLine;

    delete plotBackground;
    delete plot;

    delete actionDefaultScale;
    delete actionDisplayLines;
    delete actionAutoScaling;
    delete actionEditMode;

}

void DisplaySignalWidget::displayWithLines(bool value)
{
    actionDisplayLines->setChecked(value);
    displaySignal(p_signal);
}

void DisplaySignalWidget::setDefaultTexts()
{
    actionDefaultScale->setText(QStringLiteral("Default scale in this graph"));
    actionDisplayLines->setText(QStringLiteral("Display with lines in this graph"));
    actionAutoScaling->setText(QStringLiteral("Automatic scaling"));
    if(actionEditMode) actionEditMode->setText(QStringLiteral("Open edit mode"));
}


void DisplaySignalWidget::setLocalizedTexts(const Translation* language)
{
    if(!language)
    {
        setDefaultTexts();
    }
    else
    {
        actionDefaultScale->setText(language->getChildElementText("actionDefaultScale"));
        if(actionDefaultScale->text().isEmpty()) actionDefaultScale->setText(QStringLiteral("Default scale in this graph"));

        actionDisplayLines->setText(language->getChildElementText("actionDisplayLines"));
        if(actionDisplayLines->text().isEmpty()) actionDisplayLines->setText(QStringLiteral("Display with lines in this graph"));

        actionAutoScaling->setText(language->getChildElementText("actionAutoScaling"));
        if(actionAutoScaling->text().isEmpty()) actionAutoScaling->setText(QStringLiteral("Automatic scaling"));

        if(actionEditMode)
        {
            actionEditMode->setText(language->getChildElementText("actionEditMode"));
            if(actionEditMode->text().isEmpty()) actionEditMode->setText(QStringLiteral("Open edit mode"));
        }
    }
}


void DisplaySignalWidget::plotMouseMove(QMouseEvent * event)
{
    if(!plot->graph() || p_signal == nullptr)
    {
        return;
    }

    double x = plot->xAxis->pixelToCoord(event->pos().x());
    double y = plot->yAxis->pixelToCoord(event->pos().y());

    if(haveSelectedPoint)
    {
        //selected_point->value = y;
        p_signal->updateAll(selected_point_x,signalSelectedPointIndex,y);

        plot->graph()->data()->clear();
        plot->graph()->setData(p_signal->x(), p_signal->y());
        plot->replot();
        emit needUpdateFiltered();
    }
    else
    {
        verticalLine->setVisible(true);

        QCPDataMap::iterator u = plot->graph()->data()->lowerBound(x);
        QCPDataMap::iterator l = (u == plot->graph()->data()->begin()) ? u : (u-1);
        u = ( u == plot->graph()->data()->end()) ? l : u;


        double pos = 0;
        double val = 0;

        if(fabs(l.value().key - x) < fabs(u.value().key - x))
        {
            pos = l.key();
            val = l.value().value;
        }
        else
        {
            pos = u.key();
            val = u.value().value;
        }


        verticalLine->start->setCoords(pos, -999999999);
        verticalLine->end->setCoords(pos, 999999999);

        pos = (int)pos % p_signal->original_length();
        if(pos < 0) pos += p_signal->original_length();
        // pos je index to original signal

        emit mouseMoved(pos, val);

        plot->replot();
    }
}

void DisplaySignalWidget::plotMouseRelease(QMouseEvent * event)
{
    if(haveSelectedPoint)
    {
        double y = plot->yAxis->pixelToCoord(event->pos().y());

        if(y > plot->yAxis->range().upper || y < plot->yAxis->range().lower )
        {
            double offset = p_signal->original_range_y() * 0.1;

            plot->yAxis->setRange(p_signal->original_min_y() - offset,p_signal->original_max_y() + offset);
            plot->replot();
        }
    }
    haveSelectedPoint = false;
    plot->setInteraction(QCP::iRangeDrag, true);
}


void DisplaySignalWidget::editModePlotMousePress(QMouseEvent* event)
{
    if(!plot->graph() || p_signal == nullptr)
    {
        return;
    }

    double x = plot->xAxis->pixelToCoord(event->pos().x());
    double y = plot->yAxis->pixelToCoord(event->pos().y());

    x = roundToClosestMultiple(x,p_signal->spacing);

    switch(event->buttons())
    {
    case Qt::LeftButton:    // add point
    {
        if(!plot->graph()->data()->isEmpty())
        {
            while(p_signal->original.lastKey() < x)
            {
                p_signal->original[p_signal->original.lastKey() + p_signal->spacing] = 0;
            }
            while(p_signal->original.firstKey() > x)
            {
                p_signal->original[p_signal->original.firstKey() - p_signal->spacing] = 0;
            }
        }

        p_signal->original[x] = y;
        plot->graph()->setData(p_signal->original.keys().toVector(), p_signal->original.values().toVector());
        haveSelectedPoint = true;
        selected_point_x = x;
        break;
    }
    case Qt::RightButton:   // delete point
    {
        if(!plot->graph()->data()->isEmpty())
        {
            if(p_signal->original.lastKey() == x || p_signal->original.firstKey() == x)
            {
                p_signal->original.remove(x);
            }
            else
            {
                p_signal->original[x] = 0;
            }
            plot->graph()->setData(p_signal->original.keys().toVector(), p_signal->original.values().toVector());
        }
        break;
    }
    case Qt::MiddleButton: // dragging...
    {
        // this is a hack. AxisRect::mousePressEvent is normally protected and therefore not available...
        plot->setInteraction(QCP::iRangeDrag, true);
        QMouseEvent e(QEvent::MouseButtonPress,event->pos(),Qt::LeftButton,Qt::LeftButton,Qt::KeyboardModifiers());
        plot->axisRect()->mousePressEvent(&e);
        break;
    }
    default:
        break;
    }
}

void DisplaySignalWidget::editModePlotMouseRelease(QMouseEvent* event)
{
    if(haveSelectedPoint)
    {
        double y = plot->yAxis->pixelToCoord(event->pos().y());

        if(y > plot->yAxis->range().upper || y < plot->yAxis->range().lower )
        {
            double offset = p_signal->original_range_y() * 0.1;

            plot->yAxis->setRange(p_signal->original_min_y() - offset,p_signal->original_max_y() + offset);
            plot->replot();
        }
    }
    haveSelectedPoint = false;
    plot->setInteraction(QCP::iRangeDrag, false);

    if(event->button() == Qt::LeftButton || event->button()== Qt::RightButton)
    {
        emit editModeNeedUpdate();
        emit callForSaveEditModeState();
    }
}

double DisplaySignalWidget::roundToClosestMultiple(double toRound, double base)
{
    double quotient = toRound / base;
    double lower = floor(quotient) * base;
    double upper = ceil(quotient) * base;


    if(fabs(toRound - lower) < fabs(toRound - upper))
    {
        return lower;
    }
    else
    {
        return upper;
    }
}

void DisplaySignalWidget::editModePlotMouseMove(QMouseEvent* event)
{
    if(!plot->graph() || p_signal == nullptr)
    {
        return;
    }

    double x = plot->xAxis->pixelToCoord(event->pos().x());
    double y = plot->yAxis->pixelToCoord(event->pos().y());

    x = roundToClosestMultiple(x,p_signal->spacing);

    verticalLine->setVisible(true);

    verticalLine->start->setCoords(x, -999999999);
    verticalLine->end->setCoords(x, 999999999);

    if(haveSelectedPoint)
    {
        p_signal->original[selected_point_x] = y;

        plot->graph()->data()->clear();
        plot->graph()->setData(p_signal->original.keys().toVector(), p_signal->original.values().toVector());
        plot->replot();
        emit editModeNeedUpdate();
    }

    plot->replot();
}
