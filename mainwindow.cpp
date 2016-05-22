#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "aboutdialog.h"
#include "helpdialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exitApplication()));
    connect(ui->actionView_Help, SIGNAL(triggered()), this, SLOT(showHelpDialog()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFileDialog()));

    ui->magnitudeHorizontalScrollBar->setRange(-500, 500);
    ui->magnitudeVerticalScrollBar->setRange(-500, 500);

    ui->phaseHorizontalScrollBar->setRange(-500, 500);
    ui->phaseVerticalScrollBar->setRange(-500, 500);

    ui->originalHorizontalScrollBar->setRange(-500, 500);
    ui->originalVerticalScrollBar->setRange(-500, 500);

    ui->frequencyHorizontalScrollBar->setRange(-500, 500);
    ui->frequencyVerticalScrollBar->setRange(-500, 500);

    ui->filteredHorizontalScrollBar->setRange(-500, 500);
    ui->filteredVerticalScrollBar->setRange(-500, 500);

    // create connection between axes and scroll bars:
    connect(ui->magnitudeHorizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(magnitudeHorzScrollBarChanged(int)));
    connect(ui->magnitudeVerticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(magnitudeVertScrollBarChanged(int)));
    connect(ui->magnitudePlot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(magnitudePlotxAxisChanged(QCPRange)));
    connect(ui->magnitudePlot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(magnitudePlotyAxisChanged(QCPRange)));
    connect(ui->magnitudePlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(magnitudePlotMousePress()));
    connect(ui->magnitudePlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(magnitudePlotMouseWheel()));

    connect(ui->phaseHorizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(phaseHorzScrollBarChanged(int)));
    connect(ui->phaseVerticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(phaseVertScrollBarChanged(int)));
    connect(ui->phasePlot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(phasePlotxAxisChanged(QCPRange)));
    connect(ui->phasePlot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(phasePlotyAxisChanged(QCPRange)));
    connect(ui->phasePlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(phasePlotMousePress()));
    connect(ui->phasePlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(phasePlotMouseWheel()));

    connect(ui->originalHorizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(originalHorzScrollBarChanged(int)));
    connect(ui->originalVerticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(originalVertScrollBarChanged(int)));
    connect(ui->originalPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(originalPlotxAxisChanged(QCPRange)));
    connect(ui->originalPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(originalPlotyAxisChanged(QCPRange)));
    connect(ui->originalPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(originalPlotMousePress()));
    connect(ui->originalPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(originalPlotMouseWheel()));

    // setup policy and connect slot for context menu popup:
    ui->originalPlot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->originalPlot, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));

    connect(ui->frequencyHorizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(frequencyHorzScrollBarChanged(int)));
    connect(ui->frequencyVerticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(frequencyVertScrollBarChanged(int)));
    connect(ui->frequencyPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(frequencyPlotxAxisChanged(QCPRange)));
    connect(ui->frequencyPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(frequencyPlotyAxisChanged(QCPRange)));
    connect(ui->frequencyPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(frequencyPlotMousePress()));
    connect(ui->frequencyPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(frequencyPlotMouseWheel()));

    connect(ui->filteredHorizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(filteredHorzScrollBarChanged(int)));
    connect(ui->filteredVerticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(filteredVertScrollBarChanged(int)));
    connect(ui->filteredPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(filteredPlotxAxisChanged(QCPRange)));
    connect(ui->filteredPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(filteredPlotyAxisChanged(QCPRange)));
    connect(ui->filteredPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(filteredPlotMousePress()));
    connect(ui->filteredPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(filteredPlotMouseWheel()));


    // initialize axis range (and scroll bar positions via signals we just connected):
    ui->magnitudePlot->xAxis->setRange(0, 10, Qt::AlignCenter);
    ui->magnitudePlot->yAxis->setRange(0, 10, Qt::AlignCenter);

    ui->phasePlot->xAxis->setRange(0, 10, Qt::AlignCenter);
    ui->phasePlot->yAxis->setRange(0, 10, Qt::AlignCenter);

    ui->originalPlot->xAxis->setRange(0, 10, Qt::AlignCenter);
    ui->originalPlot->yAxis->setRange(0, 10, Qt::AlignCenter);

    ui->frequencyPlot->xAxis->setRange(0, 10, Qt::AlignCenter);
    ui->frequencyPlot->yAxis->setRange(0, 10, Qt::AlignCenter);

    ui->filteredPlot->xAxis->setRange(0, 10, Qt::AlignCenter);
    ui->filteredPlot->yAxis->setRange(0, 10, Qt::AlignCenter);

    // setup interactions
    ui->magnitudePlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes );
    ui->magnitudePlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes );

    ui->phasePlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes );
    ui->phasePlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes );

    ui->originalPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes );
    ui->originalPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes );

    ui->filteredPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes );
    ui->filteredPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes );

    ui->frequencyPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes );
    ui->frequencyPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes );
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::exitApplication()
{
    close();
}


void MainWindow::showAboutDialog()
{
    AboutDialog dialog(this);
    dialog.setModal(true);
    dialog.exec();
}


void MainWindow::showHelpDialog()
{
    HelpDialog dialog(this);
    dialog.setModal(true);
    dialog.exec();
}


void MainWindow::showOfficialWebsite()
{

}


void MainWindow::openFileDialog()
{
    QString chosenFile = QFileDialog::getOpenFileName(this, tr("Open signal..."));
    this->original.load_file(chosenFile.toStdString());

    displayOriginalSignal();
    displayMagnitudeSignal();
    displayPhaseSignal();
}


void MainWindow::horizontalScrollBarChanged(QCustomPlot* plot, int value)
{
    if (qAbs(plot->xAxis->range().center()-value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
    {
        plot->xAxis->setRange(value/100.0, plot->xAxis->range().size(), Qt::AlignCenter);

        plot->replot();
    }
}

void MainWindow::verticalScrollBarChanged(QCustomPlot* plot, int value)
{
    if (qAbs(plot->yAxis->range().center()+value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
    {
        plot->yAxis->setRange(-value/100.0, plot->yAxis->range().size(), Qt::AlignCenter);
        plot->replot();
    }
}

void MainWindow::xAxisChanged(QScrollBar* scrollbar, QCPRange& range)
{
    if(range.lower < signal.xmin - 3*signal.original_range_x())
    {
        ui->originalPlot->xAxis->setRangeLower(signal.xmin - 3*signal.original_range_x());
        return;
    }
    if(range.upper > signal.xmax + 3*signal.original_range_x())
    {
        ui->originalPlot->xAxis->setRangeUpper(signal.xmax + 3*signal.original_range_x());
        return;
    }

    if(range.lower < signal.min_x() || range.upper > signal.max_x())
    {
        while(range.lower < signal.min_x())
        {
            signal.extend_left();
        }
        while(range.upper > signal.max_x())
        {
            signal.extend_right();
        }

        if(ui->originalPlot->graph() != nullptr)
        {
            ui->originalPlot->graph()->setData(this->signal.x, this->signal.y);
            ui->originalPlot->replot();
        }
    }

//    if(signal.min_x() + signal.original_range_x() <= range.lower || signal.max_x() - signal.original_range_x() >= range.upper)
//    {
//        while(signal.min_x() + signal.original_range_x() <= range.lower)
//        {
//            signal.shrink_left();
//        }

//        while(signal.max_x() - signal.original_range_x() >= range.upper)
//        {
//            signal.shrink_right();
//        }

//        if(ui->originalPlot->graph() != nullptr)
//        {
//            ui->originalPlot->graph()->setData(this->signal.x, this->signal.y);
//            ui->originalPlot->replot();
//        }
//    }

    scrollbar->setValue(qRound(range.center()*100.0)); // adjust position of scroll bar slider
    scrollbar->setPageStep(qRound(range.size()*100.0)); // adjust size of scroll bar slider

    std::cout << range.center() << std::endl;
    std::cout << range.lower << std::endl;
}

void MainWindow::yAxisChanged(QScrollBar* scrollbar, QCPRange& range)
{
    scrollbar->setValue(qRound(-range.center()*100.0)); // adjust position of scroll bar slider
    scrollbar->setPageStep(qRound(range.size()*100.0)); // adjust size of scroll bar slider

    std::cout << range.center() << std::endl;
    std::cout << range.lower << std::endl;
}


void MainWindow::displayOriginalSignal()
{
    ui->originalPlot->clearGraphs();
    ui->originalPlot->addGraph();

    ui->originalPlot->graph()->setPen(QPen(Qt::red));
    ui->originalPlot->graph()->setBrush(QBrush(QColor(255, 0, 0, 20)));

    ui->originalPlot->graph(0)->setData(this->signal.x, this->signal.y);
    //ui->originalPlot->axisRect()->setupFullAxesBox(true);

    QCPRange range(signal.min_x(), signal.range_x());
    ui->originalPlot->xAxis->setRange(range.bounded(signal.min_x() - 5, signal.max_x() + 5));
    ui->originalPlot->yAxis->setRange(signal.min_y(), signal.range_y(), Qt::AlignLeft);
    ui->originalPlot->graph()->setLineStyle(QCPGraph::lsLine);
    ui->originalPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::blue, Qt::blue, 3));

    ui->originalPlot->replot();
}

void MainWindow::displayMagnitudeSignal()
{
    ui->magnitudePlot->clearGraphs();
    ui->magnitudePlot->addGraph();

    ui->magnitudePlot->graph()->setPen(QPen(Qt::red));
    ui->magnitudePlot->graph()->setBrush(QBrush(QColor(255, 0, 0, 20)));


    QVector<double> mags;
    double magmax = std::numeric_limits<double>::min();
    double magmin = std::numeric_limits<double>::max();

    for(std::complex<double> c : signal.cplex)
    {
        double magnitude = sqrt(c.real() * c.real() + c.imag() * c.imag());
        if(magnitude > magmax)
        {
            magmax = magnitude;
        }
        if(magnitude < magmin)
        {
            magmin = magnitude;
        }

        mags.push_back(magnitude);
    }

    ui->magnitudePlot->graph(0)->setData(this->signal.x, mags);
    //ui->originalPlot->axisRect()->setupFullAxesBox(true);

    //ui->magnitudePlot->xAxis->setRange((signal.xmax - signal.xmin)/2.0, 3*signal.xmax, Qt::AlignCenter);
    //ui->magnitudePlot->yAxis->setRange((magmax - magmin)/2.0, 3*magmax, Qt::AlignCenter);

    ui->magnitudePlot->replot();
}

void MainWindow::displayPhaseSignal()
{
    ui->phasePlot->clearGraphs();
    ui->phasePlot->addGraph();

    ui->phasePlot->graph()->setPen(QPen(Qt::red));
    ui->phasePlot->graph()->setBrush(QBrush(QColor(255, 0, 0, 20)));


    QVector<double> phases;
    double phasemax = std::numeric_limits<double>::min();
    double phasemin = std::numeric_limits<double>::max();

    for(std::complex<double> c : signal.cplex)
    {
        double phase = atan2(c.imag(), c.real());
        if(phase > phasemax)
        {
            phasemax = phase;
        }
        if(phase < phasemin)
        {
            phasemin = phase;
        }

        phases.push_back(phase);
    }

    ui->phasePlot->graph(0)->setData(this->signal.x, phases);
    //ui->originalPlot->axisRect()->setupFullAxesBox(true);

    //ui->phasePlot->xAxis->setRange((signal.xmax - signal.xmin)/2.0, 3*signal.xmax, Qt::AlignCenter);
    //ui->phasePlot->yAxis->setRange((phasemax - phasemin)/2.0, 3*phasemax, Qt::AlignCenter);

    ui->phasePlot->replot();
}

void MainWindow::plotMousePress(QCustomPlot* plot)
{
    // if an axis is selected, only allow the direction of that axis to be dragged
    // if no axis is selected, both directions may be dragged

    if (plot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
        plot->axisRect()->setRangeDrag(plot->xAxis->orientation());
    else if (plot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
        plot->axisRect()->setRangeDrag(plot->yAxis->orientation());
    else
        plot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::plotMouseWheel(QCustomPlot* plot)
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



void MainWindow::contextMenuRequest(QPoint pos)
{
    QMenu *menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    menu->addAction("Default scale", this, SLOT(defaultSize()));

    menu->popup(ui->originalPlot->mapToGlobal(pos));
}

void MainWindow::defaultSize()
{
    std::cout << "This is the action!" << std::endl;
}


/**************************** MAGNITUDE GRAPH CALLBACKS ***********************************/
void MainWindow::magnitudeHorzScrollBarChanged(int value)
{
    horizontalScrollBarChanged(ui->magnitudePlot,value);
}

void MainWindow::magnitudeVertScrollBarChanged(int value)
{
    verticalScrollBarChanged(ui->magnitudePlot,value);
}

void MainWindow::magnitudePlotxAxisChanged(QCPRange range)
{
    xAxisChanged(ui->magnitudeHorizontalScrollBar,range);
}

void MainWindow::magnitudePlotyAxisChanged(QCPRange range)
{
    yAxisChanged(ui->magnitudeVerticalScrollBar,range);
}

void MainWindow::magnitudePlotMouseWheel()
{
    plotMouseWheel(ui->magnitudePlot);
}

void MainWindow::magnitudePlotMousePress()
{
    plotMousePress(ui->magnitudePlot);
}


/**************************** PHASE GRAPH CALLBACKS ***********************************/
void MainWindow::phaseHorzScrollBarChanged(int value)
{
    horizontalScrollBarChanged(ui->phasePlot,value);
}

void MainWindow::phaseVertScrollBarChanged(int value)
{
    verticalScrollBarChanged(ui->phasePlot,value);
}

void MainWindow::phasePlotxAxisChanged(QCPRange range)
{
    xAxisChanged(ui->phaseHorizontalScrollBar,range);
}

void MainWindow::phasePlotyAxisChanged(QCPRange range)
{
    yAxisChanged(ui->phaseVerticalScrollBar,range);
}

void MainWindow::phasePlotMouseWheel()
{
    plotMouseWheel(ui->phasePlot);
}

void MainWindow::phasePlotMousePress()
{
    plotMousePress(ui->phasePlot);
}


/**************************** ORIGINAL SIGNAL GRAPH CALLBACKS ***********************************/
void MainWindow::originalHorzScrollBarChanged(int value)
{
    horizontalScrollBarChanged(ui->originalPlot,value);
}

void MainWindow::originalVertScrollBarChanged(int value)
{
    verticalScrollBarChanged(ui->originalPlot,value);
}

void MainWindow::originalPlotxAxisChanged(QCPRange range)
{
    xAxisChanged(ui->originalHorizontalScrollBar,range);
}

void MainWindow::originalPlotyAxisChanged(QCPRange range)
{
    if(range.lower < signal.ymin - signal.original_range_y())
    {
        ui->originalPlot->yAxis->setRangeLower(signal.ymin - signal.original_range_y());
    }
    if(range.upper > signal.ymax + signal.original_range_y())
    {
        ui->originalPlot->yAxis->setRangeUpper(signal.ymax + signal.original_range_y());
    }

    yAxisChanged(ui->originalVerticalScrollBar,range);
}

void MainWindow::originalPlotMouseWheel()
{
    plotMouseWheel(ui->originalPlot);
}

void MainWindow::originalPlotMousePress()
{
    plotMousePress(ui->originalPlot);
}


/**************************** FILTERED SIGNAL GRAPH CALLBACKS ***********************************/
void MainWindow::filteredHorzScrollBarChanged(int value)
{
    horizontalScrollBarChanged(ui->filteredPlot,value);
}

void MainWindow::filteredVertScrollBarChanged(int value)
{
    verticalScrollBarChanged(ui->filteredPlot,value);
}

void MainWindow::filteredPlotxAxisChanged(QCPRange range)
{
    xAxisChanged(ui->filteredHorizontalScrollBar,range);
}

void MainWindow::filteredPlotyAxisChanged(QCPRange range)
{
    yAxisChanged(ui->filteredVerticalScrollBar,range);
}

void MainWindow::filteredPlotMouseWheel()
{
    plotMouseWheel(ui->filteredPlot);
}

void MainWindow::filteredPlotMousePress()
{
    plotMousePress(ui->filteredPlot);
}


/**************************** FREQUENCY GRAPH CALLBACKS ***********************************/

void MainWindow::frequencyHorzScrollBarChanged(int value)
{
    horizontalScrollBarChanged(ui->frequencyPlot,value);
}

void MainWindow::frequencyVertScrollBarChanged(int value)
{
    verticalScrollBarChanged(ui->frequencyPlot,value);
}

void MainWindow::frequencyPlotxAxisChanged(QCPRange range)
{
    xAxisChanged(ui->frequencyHorizontalScrollBar,range);
}

void MainWindow::frequencyPlotyAxisChanged(QCPRange range)
{
    yAxisChanged(ui->frequencyVerticalScrollBar,range);
}

void MainWindow::frequencyPlotMouseWheel()
{
    plotMouseWheel(ui->frequencyPlot);
}

void MainWindow::frequencyPlotMousePress()
{
    plotMousePress(ui->frequencyPlot);
}


