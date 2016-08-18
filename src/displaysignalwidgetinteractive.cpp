#include "displaysignalwidgetinteractive.h"

DisplaySignalWidgetInteractive::DisplaySignalWidgetInteractive(bool displayLabel, bool hasBackground, QWidget *parent) : DisplaySignalWidget(displayLabel, hasBackground, parent)
{
    connect(plot,&QCustomPlot::mouseMove,this,&DisplaySignalWidgetInteractive::plotMouseMove);
    connect(plot,&QCustomPlot::mouseRelease,this,&DisplaySignalWidgetInteractive::plotMouseRelease);
    vert = nullptr;
    //selected_point = plot->graph()->data()->end();
    haveSelectedPoint = false;
}


DisplaySignalWidgetInteractive::~DisplaySignalWidgetInteractive()
{
    delete vert;
}

void DisplaySignalWidgetInteractive::plotMouseMove(QMouseEvent * event)
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
        p_signal->updateAll(signalSelectedPointIndex,y);

        plot->graph()->data()->clear();
        plot->graph()->setData(p_signal->x(), p_signal->y());
        plot->replot();
        emit needUpdateFiltered();
    }
    else
    {
        if(!vert)
        {
            vert = new QCPItemLine(plot);
            QPen pen;
            pen.setColor( Qt::green );
            vert->setPen( pen );
            QPen penSelect;
            penSelect.setColor( Qt::red );
            vert->setSelectedPen( penSelect );
            vert->setSelectable( true );
            vert->setSelected( true );
        }


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


        vert->start->setCoords(pos, QCPRange::minRange);
        vert->end->setCoords(pos, QCPRange::maxRange);

        pos = (int)pos % p_signal->original_length();
        if(pos < 0) pos += p_signal->original_length();
        // pos je index to original signal

        emit mouseMoved(pos, val);

        plot->replot();
    }
}



void DisplaySignalWidgetInteractive::enableCentering(bool enabled)
{
    centering = enabled;

    if(p_signal != nullptr)
    {
        displaySignal(p_signal);
    }
}

void DisplaySignalWidgetInteractive::plotDefaultScale()
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
            plot->xAxis->setRange(p_signal->original_min_x() - p_signal->original_range_x() / 2.0 - offset, p_signal->original_min_x() + p_signal->original_range_x() / 2.0 + offset);
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
}

void DisplaySignalWidgetInteractive::placePlotBackground(QCPItemRect*& section)
{
    if(centering)
    {
        section->topLeft->setCoords(p_signal->original_min_x() - p_signal->original_range_x() / 2.0,99999);
        section->bottomRight->setCoords(p_signal->original_min_x() + p_signal->original_range_x() / 2.0,-99999);
    }
    else
    {
        section->topLeft->setCoords(p_signal->original_min_x(),99999);
        section->bottomRight->setCoords(p_signal->original_max_x(),-99999);
    }
}

void DisplaySignalWidgetInteractive::plotMousePress(QMouseEvent* event)
{
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
        selected_point = u;
        haveSelectedPoint = true;
        // selected u
    }
    else if ( (fabs(l->key - x) < delta) && (fabs(l->value - y) < delta) )
    {
        selected_point = l;
        haveSelectedPoint = true;
        // selected l
    }
    if(haveSelectedPoint)
    {
        signalSelectedPointIndex = p_signal->getOriginalIndex(selected_point->key);
        plot->setInteraction(QCP::iRangeDrag, false);
        emit callForSaveState();
    }
}

void DisplaySignalWidgetInteractive::plotMouseRelease(QMouseEvent * event)
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
    //selected_point = plot->graph()->data()->end();
    haveSelectedPoint = false;
    plot->setInteraction(QCP::iRangeDrag, true);
}
