#include "displaysignalwidget.h"

#include<limits>

using namespace FT1D;

FT1D::DisplaySignalWidget::DisplaySignalWidget(FT1D::DisplaySignalWidgetType type, bool allowEditMode, QWidget *parent) :  QWidget(parent)
{
    // does not work in initialisation section.
    p_signal = nullptr;
    shadow_signal = nullptr;

    this->type = type;
    centering = false;

    haveSelectedPoint = false;
    selectedPointX = 0;

    sibling = nullptr;

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
    if( type == EDIT_MODE)
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

    if(type == EDIT_MODE)
    {
        plot->setInteractions(QCP::iRangeZoom | QCP::iSelectAxes);
    }
    else
    {
        plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes);
    }

    // setup basic callbacks - all graphs but edit mode.
    if(type != EDIT_MODE)
    {
        connect(plot->xAxis, static_cast<void (QCPAxis::*)(const QCPRange& r)>(&QCPAxis::rangeChanged), this, &DisplaySignalWidget::plotXAxisChanged);
//        connect(plot->yAxis, static_cast<void (QCPAxis::*)(const QCPRange& r)>(&QCPAxis::rangeChanged), this, &DisplaySignalWidget::plotYAxisChanged);

        plot->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(plot, &QCustomPlot::customContextMenuRequested, this,&DisplaySignalWidget::contextMenuRequest);

        connect(plot, &QCustomPlot::mouseMove,  this, &DisplaySignalWidget::plotMouseMove);
        connect(plot, &QCustomPlot::mouseWheel, this, &DisplaySignalWidget::plotMouseWheel);

        connect(plot, &QCustomPlot::mousePress,   this, [=](QMouseEvent* e)
        {
            if(plot->graphCount() == 2)
            {
                plot->removeGraph(0);
                update();
            }
            if(sibling) { sibling->plot->axisRect()->mousePressEvent(e); }
        });

        connect(plot, &QCustomPlot::mouseMove,   this, [=](QMouseEvent* e) {
            if(sibling && !haveSelectedPoint) { sibling->plot->axisRect()->mouseMoveEvent(e); }});

        connect(plot, &QCustomPlot::mouseRelease,   this, [=](QMouseEvent* e) {
            if(sibling) { sibling->plot->axisRect()->mouseReleaseEvent(e); }});
    }


    if(type == MAGNITUDE || type == PHASE)
    {
        connect(plot, &QCustomPlot::mousePress,   this, &DisplaySignalWidget::plotMousePress);
        connect(plot, &QCustomPlot::mouseMove,    this, &DisplaySignalWidget::plotMouseMove);
        connect(plot, &QCustomPlot::mouseRelease, this, &DisplaySignalWidget::plotMouseRelease);
    }
    else if(type == EDIT_MODE)
    {
        connect(plot, &QCustomPlot::mousePress,   this, &DisplaySignalWidget::editModePlotMousePress);
        connect(plot, &QCustomPlot::mouseMove,    this, &DisplaySignalWidget::editModePlotMouseMove);
        connect(plot, &QCustomPlot::mouseRelease, this, &DisplaySignalWidget::editModePlotMouseRelease);
    }


    plot->xAxis->setRange(0, 1, Qt::AlignCenter);
    plot->xAxis->setNumberFormat("g");
    plot->xAxis->setNumberPrecision(3);


    plot->yAxis->setRange(0, 2.2, Qt::AlignCenter);
    plot->yAxis->setNumberFormat("f");
    plot->yAxis->setNumberPrecision(3);


    // axis labels

    QLabel* plotxAxisLabel = new QLabel(plot);
    QLabel* plotyAxisLabel = new QLabel(plot);

    plotxAxisLabel->setFont(QFont("sans-serif",10));
    plotyAxisLabel->setFont(QFont("sans-serif",10));

    if(type == MAGNITUDE || type == PHASE)
    {
        plotxAxisLabel->setText(QStringLiteral("ω"));
        plotyAxisLabel->setText(QStringLiteral("F(ω)"));
        plotxAxisLabel->setGeometry(plot->width()-15,plot->height() - 27,20,20);
        plotyAxisLabel->setGeometry(23,-3,30,20);
    }
    else
    {
        plotxAxisLabel->setText(QStringLiteral("x"));
        plotyAxisLabel->setText(QStringLiteral("f(x)"));
        plotxAxisLabel->setGeometry(plot->width()-15,plot->height() - 27,20,20);
        plotyAxisLabel->setGeometry(27,-3,30,20);
    }


    actionDefaultScale = new QAction(this);

    actionDisplayLines = new QAction(this);
    actionDisplayLines->setCheckable(true);
    actionDisplayLines->setChecked(false);

    actionAutoScaling = new QAction(this);
    actionAutoScaling->setCheckable(true);
    actionAutoScaling->setChecked(true);

    connect(actionDefaultScale, &QAction::triggered, this, [=]()
    {
        this->plotDefaultScale();
        if(sibling)
        {
            sibling->plotDefaultScale();
        }
    });
    connect(actionDisplayLines, &QAction::triggered, this, &DisplaySignalWidget::displayWithLines);

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

bool FT1D::DisplaySignalWidget::event(QEvent* e)
{
    if (e->type()==QEvent::Leave)
    {
        verticalLine->setVisible(false);
        plot->replot();
        emit mouseLeave();
    }

    return QWidget::event(e); // Or whatever parent class you have.
}


FT1D::DisplaySignalWidget::~DisplaySignalWidget()
{
    delete verticalLine;

    delete plotBackground;
    delete plot;

    delete actionDefaultScale;
    delete actionDisplayLines;
    delete actionAutoScaling;
    delete actionEditMode;
}


void FT1D::DisplaySignalWidget::plotXAxisChanged(const QCPRange& range)
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
                //if(sibling && sibling->p_signal) sibling->p_signal->extend_left();
            }
            while(range.upper > p_signal->max_x())
            {
                p_signal->extend_right();
                //if(sibling && sibling->p_signal) sibling->p_signal->extend_right();
            }

            if(plot->graph() != nullptr)
            {
                plot->graph()->setData(p_signal->x(), p_signal->y());
                plot->replot();
            }
        }
    }
//    if(sibling)
//    {
//        sibling->plot->xAxis->setRange(range);
//    }
}


//void DisplaySignalWidget::plotYAxisChanged(const QCPRange& range)
//{
//    // nothing
////    if(sibling)
////    {
////        sibling->plot->yAxis->setRange(range);
////    }
//}


void FT1D::DisplaySignalWidget::plotDefaultScale()
{
    if(p_signal != nullptr)
    {

            double offset = p_signal->original_range_x() * 0.1;
            //double offset = 0;
            if(p_signal->range_x() < 0.000001)
            {
                offset = 0.5;
            }

            if(centering)
            {
                float start = p_signal->original_min_x() - floor(p_signal->original_range_x() / 2.0);
                plot->xAxis->setRange(start - offset, start + p_signal->original_range_x() + offset);
            }
            else
            {
                plot->xAxis->setRange(p_signal->original_min_x() - offset,p_signal->original_max_x() + offset);

            }


            offset = p_signal->original_range_y() * 0.1;
            //offset = 0;
            if(p_signal->range_y() < 0.000001)
            {
                offset = 0.5;
            }
            plot->yAxis->setRange(p_signal->original_min_y() - offset,p_signal->original_max_y() + offset);

        plot->replot();
    }
    if(type == EDIT_MODE)
    {
        if(p_signal == nullptr || p_signal->empty())
        {
            plot->xAxis->setRange(-4,4);
        }
        else
        {
            plot->xAxis->setRange(p_signal->min_x() - 4, p_signal->max_x() + 4);
        }

        if(plot->yAxis->range().upper < 2 && plot->yAxis->range().lower > -2)
        {
            plot->yAxis->setRange(-2,2);
        }
    }
}


void FT1D::DisplaySignalWidget::contextMenuRequest(QPoint pos)
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


void FT1D::DisplaySignalWidget::displaySignal(Signal* signal, bool shadowPrevious)
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
        if(shadowPrevious && shadow_signal)
        {
            QCPGraph* shadowGraph = plot->addGraph();
            shadowGraph->setSelectable(false);
            shadowGraph->setName(QStringLiteral("shadowGraph"));

            shadowGraph->setData(shadow_signal->x(), shadow_signal->y());
            shadowGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::gray, Qt::gray,5));

            if(actionDisplayLines->isChecked())
            {
                shadowGraph->setPen(QPen(Qt::gray));
                shadowGraph->setLineStyle(QCPGraph::lsLine);
                shadowGraph->setBrush(Qt::NoBrush);
            }
            else
            {
                shadowGraph->setLineStyle(QCPGraph::lsNone);
            }
        }

        QCPGraph* graph = plot->addGraph();

        if(type == EDIT_MODE)
        {
            graph->setData(p_signal->original.keys().toVector(), p_signal->original.values().toVector());
            graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::blue, Qt::blue,5));
        }
        else
        {
            graph->setData(p_signal->x(), p_signal->y());
            graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::blue, Qt::blue,5));

        }

        if(actionDisplayLines->isChecked())
        {
            graph->setPen(QPen(QColor::fromRgb(255,165,0)));
            graph->setLineStyle(QCPGraph::lsLine);
            graph->setBrush(Qt::NoBrush);
        }
        else
        {
            graph->setLineStyle(QCPGraph::lsNone);
        }


        if(plotBackground)
        {
            plotBackground->setVisible(true);
            placePlotBackground(plotBackground);
        }
        if(actionAutoScaling->isChecked())
        {
            plotDefaultScale();

// I had these lines commented for some reason. Does it cause any problems?
            if(sibling)
            {
                sibling->plotDefaultScale();
                //sibling->plotXAxisChanged(plot->xAxis->range());
            }
        }

        plotXAxisChanged(plot->xAxis->range());
        plot->replot();

// This was also commented.
        if(sibling)
        {
            sibling->plotXAxisChanged(plot->xAxis->range());
            sibling->plot->replot();
        }

    }

    if(shadow_signal)
    {
        delete shadow_signal;
    }
    shadow_signal = new Signal(*p_signal);
}


void FT1D::DisplaySignalWidget::placePlotBackground(QCPItemRect*& section)
{
    if(centering)
    {
        float start = p_signal->original_min_x() - floor(p_signal->original_range_x() / 2.0);
        section->topLeft->setCoords(start,999999999999);
        section->bottomRight->setCoords(start + p_signal->original_range_x(),-999999999999);
    }
    else
    {
        section->topLeft->setCoords(p_signal->original_min_x(),999999999999);
        section->bottomRight->setCoords(p_signal->original_max_x(),-999999999999);
    }
}


void FT1D::DisplaySignalWidget::enableCentering(bool enabled)
{
    centering = enabled;

    if(p_signal != nullptr)
    {
        displaySignal(p_signal);
    }
}


void FT1D::DisplaySignalWidget::displayWithLines(bool value)
{
    actionDisplayLines->setChecked(value);
    displaySignal(p_signal);
}


void FT1D::DisplaySignalWidget::setDefaultTexts()
{
    actionDefaultScale->setText(QStringLiteral("Default scale in this graph"));
    actionDisplayLines->setText(QStringLiteral("Display with lines in this graph"));
    actionAutoScaling->setText(QStringLiteral("Automatic scaling"));
    if(actionEditMode) actionEditMode->setText(QStringLiteral("Open edit mode"));
}


void FT1D::DisplaySignalWidget::setLocalizedTexts(const Translation* language)
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


void FT1D::DisplaySignalWidget::plotMousePress(QMouseEvent* event)
{
    // there was some code for range dragging when axis selected.

    if(!plot->graph() || p_signal == nullptr)
    {
        return;
    }

    if(plot->graphCount() == 2)
    {
        plot->removeGraph(0);
    }

    double x = plot->xAxis->pixelToCoord(event->pos().x());
    double y = plot->yAxis->pixelToCoord(event->pos().y());

    double delta = p_signal->spacing * 0.25;
    if(delta == 0) delta = 0.25;

    QCPDataMap::iterator u = plot->graph()->data()->lowerBound(x);
    QCPDataMap::iterator l = (u == plot->graph()->data()->begin()) ? u : (u-1);
    u = ( u == plot->graph()->data()->end()) ? l : u;

    if ((fabs(u->key - x) < delta) && (fabs(u->value - y) < delta) )
    {
        selectedPointX = u.key();
        haveSelectedPoint = true;
        // selected u
    }
    else if ( (fabs(l->key - x) < delta) && (fabs(l->value - y) < delta) )
    {
        selectedPointX = l.key();
        haveSelectedPoint = true;
        // selected l
    }
    if(haveSelectedPoint)
    {
        plot->setInteraction(QCP::iRangeDrag, false);
        emit callForSaveState();
    }
}


void FT1D::DisplaySignalWidget::plotMouseWheel(QWheelEvent* e)
{
    if(plot->graphCount() == 2)
    {
        plot->removeGraph(0);
    }

    // if an axis is selected, only allow the direction of that axis to be zoomed
    // if no axis is selected, both directions may be zoomed

    if (plot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    {
        plot->axisRect()->setRangeZoom(plot->xAxis->orientation());
        if(sibling)
        {
            sibling->plot->axisRect()->setRangeZoom(sibling->plot->xAxis->orientation());
        }
    }
    else if (plot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    {
        plot->axisRect()->setRangeZoom(plot->yAxis->orientation());
        if(sibling)
        {
            sibling->plot->axisRect()->setRangeZoom(sibling->plot->yAxis->orientation());
        }
    }
    else
    {
        plot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
        if(sibling)
        {
            sibling->plot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
        }
    }

    if(sibling)
    {
        sibling->plot->axisRect()->wheelEvent(e);
        sibling->plot->replot();
    }

}


void FT1D::DisplaySignalWidget::plotMouseMove(QMouseEvent * event)
{
    if(!plot->graph() || p_signal == nullptr || p_signal->original_length() == 0)
    {
        return;
    }

    double x = plot->xAxis->pixelToCoord(event->pos().x());
    double y = plot->yAxis->pixelToCoord(event->pos().y());

    if(type == MAGNITUDE && y < 0)
    {
        y = 0;
    }
    else if(type == PHASE)
    {
        if(y < -M_PI)
        {
            y = -M_PI;
        }
        else if(y > M_PI)
        {
            y = M_PI;
        }
    }

    if(haveSelectedPoint)
    {
        int selectedPointIndex = (int)selectedPointX % p_signal->original_length();
        if(selectedPointIndex < 0) selectedPointIndex += p_signal->original_length();

        emit displayValueStatusBar(selectedPointX, selectedPointIndex);
        p_signal->updateAll(selectedPointIndex,y);
        if(selectedPointX != 0)
        {
            if(type == MAGNITUDE)
            {
                p_signal->updateAll(- selectedPointIndex  + p_signal->original_length(),y);
            }
            else if( (- selectedPointIndex  + p_signal->original_length()) != selectedPointIndex)
            {
                p_signal->updateAll(- selectedPointIndex  + p_signal->original_length(), -y);
            }
        }

        plot->graph()->data()->clear();
        plot->graph()->setData(p_signal->x(), p_signal->y());
        plot->replot();
        emit needUpdateFiltered();
    }
    else
    {
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

        verticalLine->setVisible(true);

        verticalLine->start->setCoords(pos, -999999999);
        verticalLine->end->setCoords(pos, 999999999);

        int idx = (int)pos % p_signal->original_length();
        if(idx < 0) idx += p_signal->original_length();
        // pos je index to original signal

        emit needFrequencyUpdate(idx, val);
        emit displayValueStatusBar(pos, idx);

        plot->replot();
    }
}

void FT1D::DisplaySignalWidget::plotMouseRelease(QMouseEvent * event)
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


void FT1D::DisplaySignalWidget::editModePlotMousePress(QMouseEvent* event)
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
            emit callForSaveEditModeState();

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
        selectedPointX = x;
        break;
    }
    case Qt::RightButton:   // delete point
    {
        if(!plot->graph()->data()->isEmpty())
        {
            emit callForSaveEditModeState();

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


void FT1D::DisplaySignalWidget::editModePlotMouseMove(QMouseEvent* event)
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
        p_signal->original[selectedPointX] = y;

        plot->graph()->data()->clear();
        plot->graph()->setData(p_signal->original.keys().toVector(), p_signal->original.values().toVector());
        plot->replot();
        emit editModeNeedUpdate();
    }

    plot->replot();
}


void FT1D::DisplaySignalWidget::editModePlotMouseRelease(QMouseEvent* event)
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
        p_signal->findYMinMax();
        emit editModeNeedUpdate();
    }
}


double FT1D::DisplaySignalWidget::roundToClosestMultiple(double toRound, double base)
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


void FT1D::DisplaySignalWidget::setInteractionsEnabled(bool val)
{
    actionDefaultScale->setEnabled(val);
    actionDisplayLines->setEnabled(val);
    actionAutoScaling->setEnabled(val);

    if(val)
    {
        if(type == EDIT_MODE)
        {
            plot->setInteractions(QCP::iRangeZoom | QCP::iSelectAxes);
        }
        else
        {
            plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes);
        }
    }
    else
    {
        plot->setInteractions(QCP::Interactions());
    }
}

void FT1D::DisplaySignalWidget::setSibling(FT1D::DisplaySignalWidget*& other)
{
    this->sibling = other;
    other->sibling = this;
}
