#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "aboutdialog.h"
#include "helpdialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    localization.loadLanguages(config.getLocalizationFolder());
    populateLanguages();
    setLanguage(config.currentLanguage);

    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exitApplication()));
    connect(ui->actionView_Help, SIGNAL(triggered()), this, SLOT(showHelpDialog()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFileDialog()));
    connect(ui->actionOfficial_Website, SIGNAL(triggered()), this, SLOT(showOfficialWebsite()));

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
    ui->magnitudePlot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->magnitudePlot, &QCustomPlot::customContextMenuRequested, [=](QPoint pos) {
        contextMenuRequest(ui->magnitudePlot,magnitude,pos);
     } );

    connect(ui->magnitudePlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(displayFrequency(QMouseEvent*)));



    connect(ui->phaseHorizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(phaseHorzScrollBarChanged(int)));
    connect(ui->phaseVerticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(phaseVertScrollBarChanged(int)));
    connect(ui->phasePlot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(phasePlotxAxisChanged(QCPRange)));
    connect(ui->phasePlot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(phasePlotyAxisChanged(QCPRange)));
    connect(ui->phasePlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(phasePlotMousePress()));
    connect(ui->phasePlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(phasePlotMouseWheel()));
    ui->phasePlot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->phasePlot, &QCustomPlot::customContextMenuRequested, [=](QPoint pos) {
        contextMenuRequest(ui->phasePlot,phase,pos);
     } );

    connect(ui->centeringCheckBox, SIGNAL(toggled(bool)), this, SLOT(centeringToggled(bool)));

    connect(ui->originalHorizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(originalHorzScrollBarChanged(int)));
    connect(ui->originalVerticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(originalVertScrollBarChanged(int)));
    connect(ui->originalPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(originalPlotxAxisChanged(QCPRange)));
    connect(ui->originalPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(originalPlotyAxisChanged(QCPRange)));
    connect(ui->originalPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(originalPlotMousePress()));
    connect(ui->originalPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(originalPlotMouseWheel()));
    // setup policy and connect slot for context menu popup:
    ui->originalPlot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->originalPlot, &QCustomPlot::customContextMenuRequested, [=](QPoint pos) {
        contextMenuRequest(ui->originalPlot,original,pos);
     } );

    connect(ui->frequencyHorizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(frequencyHorzScrollBarChanged(int)));
    connect(ui->frequencyVerticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(frequencyVertScrollBarChanged(int)));
    connect(ui->frequencyPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(frequencyPlotxAxisChanged(QCPRange)));
    connect(ui->frequencyPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(frequencyPlotyAxisChanged(QCPRange)));
    connect(ui->frequencyPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(frequencyPlotMousePress()));
    connect(ui->frequencyPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(frequencyPlotMouseWheel()));
    ui->frequencyPlot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->frequencyPlot, &QCustomPlot::customContextMenuRequested, [=](QPoint pos) {
        contextMenuRequest(ui->frequencyPlot,frequency,pos);
     } );

    connect(ui->filteredHorizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(filteredHorzScrollBarChanged(int)));
    connect(ui->filteredVerticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(filteredVertScrollBarChanged(int)));
    connect(ui->filteredPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(filteredPlotxAxisChanged(QCPRange)));
    connect(ui->filteredPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(filteredPlotyAxisChanged(QCPRange)));
    connect(ui->filteredPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(filteredPlotMousePress()));
    connect(ui->filteredPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(filteredPlotMouseWheel()));
    ui->filteredPlot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->filteredPlot, &QCustomPlot::customContextMenuRequested, [=](QPoint pos) {
        contextMenuRequest(ui->filteredPlot,filtered,pos);
     } );


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
    delete vert;
    delete ui;
}


void MainWindow::exitApplication()
{
    close();
}


void MainWindow::showAboutDialog()
{
    AboutDialog dialog(this,localization);
    dialog.setModal(true);
    dialog.exec();
}


void MainWindow::showHelpDialog()
{
    HelpDialog dialog(this,localization);
    dialog.setModal(true);
    dialog.exec();
}


void MainWindow::showOfficialWebsite()
{
    QString link = "http://github.com/janbella/FTutor";
    QDesktopServices::openUrl(QUrl(link));
}


void MainWindow::openFileDialog()
{
    QString chosenFile = QFileDialog::getOpenFileName(this, tr("Open signal..."));
    if(this->original.load_file(chosenFile.toStdString()))
    {
        ui->magnitudePlot->clearGraphs();
        ui->magnitudePlot->clearItems();
        ui->magnitudePlot->clearPlottables();
        ui->magnitudePlot->clearFocus();
        if (ui->magnitudePlot->layer("grid"))
        {
            //ui->magnitudePlot->layer("grid")->deleteLater();
            ui->magnitudePlot->removeLayer(ui->magnitudePlot->layer("grid"));
        }
        if (ui->magnitudePlot->layer("sectionBackground"))
        {
           // ui->magnitudePlot->layer("sectionBackground")->deleteLater();
            ui->magnitudePlot->removeLayer(ui->magnitudePlot->layer("sectionBackground"));
        }
        if (ui->magnitudePlot->layer("freq"))
        {
            //ui->magnitudePlot->layer("freq")->deleteLater();
            ui->magnitudePlot->removeLayer(ui->magnitudePlot->layer("freq"));
        }

        ui->magnitudePlot->clearItems();
        vert = nullptr; // delete is called in clearItems


        original.fourierTransform(original,magnitude,phase);

        displaySignal(ui->originalPlot,this->original, true);
        displaySignal(ui->magnitudePlot,this->magnitude, true, centering);
        displaySignal(ui->phasePlot,this->phase, true, centering);
        defaultScale(ui->originalPlot,this->original,false);
        defaultScale(ui->magnitudePlot,this->magnitude,true);
        defaultScale(ui->phasePlot,this->phase,true);
    }
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

void MainWindow::xAxisChanged(QCustomPlot* plot, QScrollBar* scrollbar, Signal& signal, QCPRange& range)
{
    if(range.lower < signal.allowed_min_x())
    {
        plot->xAxis->setRangeLower(signal.allowed_min_x());
        return;
    }
    if(range.upper > signal.allowed_max_x())
    {
        plot->xAxis->setRangeUpper(signal.allowed_max_x());
        return;
    }

    if(range.lower < signal.current_min_x() || range.upper > signal.current_max_x())
    {
        while(range.lower < signal.current_min_x())
        {
            signal.extend_left();
        }
        while(range.upper > signal.current_max_x())
        {
            signal.extend_right();
        }

        if(plot->graph() != nullptr)
        {
            plot->graph()->setData(signal.x, signal.y);
            plot->replot();
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
}

void MainWindow::yAxisChanged(QCustomPlot* plot, QScrollBar* scrollbar, Signal& signal, QCPRange& range)
{
    if(range.lower < signal.ymin - signal.original_range_y())
    {
        plot->yAxis->setRangeLower(signal.ymin - signal.original_range_y());
        return;
    }
    if(range.upper > signal.ymax + signal.original_range_y())
    {
        plot->yAxis->setRangeUpper(signal.ymax + signal.original_range_y());
        return;
    }
    scrollbar->setValue(qRound(-range.center()*100.0)); // adjust position of scroll bar slider
    scrollbar->setPageStep(qRound(range.size()*100.0)); // adjust size of scroll bar slider
}


void MainWindow::displaySignal(QCustomPlot* plot, Signal& signal, bool background, bool centering)
{
    plot->clearGraphs();
    plot->clearItems();

    plot->addGraph();

    plot->graph()->setPen(QPen(Qt::red));
    plot->graph()->setBrush(QBrush(QColor(255, 0, 0, 20)));

    plot->graph(0)->setData(signal.x, signal.y);
    //plot->axisRect()->setupFullAxesBox(true);

    if(plot->layer("sectionBackground"))
    {
        plot->removeLayer(plot->layer("sectionBackground"));
    }

    plot->xAxis->setRange(signal.current_min_x(), signal.current_range_x(), Qt::AlignLeft);
    plot->yAxis->setRange(signal.current_min_y(), signal.current_range_y(), Qt::AlignLeft);
    plot->graph()->setLineStyle(QCPGraph::lsNone);
    plot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::blue, Qt::blue, 3));

    if(background)
    {
        QCPItemRect* section = new QCPItemRect(plot);
        section->topLeft->setType(QCPItemPosition::ptPlotCoords);
        section->topLeft->setAxes(plot->xAxis, plot->yAxis);
        section->bottomRight->setType(QCPItemPosition::ptPlotCoords);
        section->bottomRight->setAxes(plot->xAxis, plot->yAxis);
        if(centering)
        {
            section->topLeft->setCoords(signal.xmin + signal.original_range_x() / 2.0,99999);
            section->bottomRight->setCoords(signal.xmax + signal.original_range_x() / 2.0,-99999);
        }
        else
        {
            section->topLeft->setCoords(signal.xmin,99999);
            section->bottomRight->setCoords(signal.xmax,-99999);
        }
        section->setBrush(QBrush(QColor(0,200,0,100)));
        section->setPen(Qt::NoPen);

        plot->addLayer("sectionBackground", plot->layer("grid"), QCustomPlot::limBelow);
        section->setLayer("sectionBackground");
        plot->addItem(section);
    }
    plot->replot();
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



void MainWindow::contextMenuRequest(QCustomPlot* plot, Signal& signal, QPoint pos)
{
    QMenu *menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);


    // very ugly, I need to find a way how to call defaultScale(plot, signal) as a callback to each
    if(plot == ui->originalPlot)
    {
        menu->addAction("Default scale", this, SLOT(originalPlotDefautlScale()));
    }
    else if(plot == ui->filteredPlot)
    {
        menu->addAction("Default scale", this, SLOT(filteredPlotDefautlScale()));
    }
    else if(plot == ui->frequencyPlot)
    {
        menu->addAction("Default scale", this, SLOT(frequencyPlotDefautlScale()));
    }
    else if (plot == ui->magnitudePlot)
    {
        menu->addAction("Default scale", this, SLOT(magnitudePlotDefautlScale()));
    }
    else
    {
        menu->addAction("Default scale", this, SLOT(phasePlotDefautlScale()));
    }


    menu->popup(plot->mapToGlobal(pos));
}

void MainWindow::defaultScale(QCustomPlot* plot, Signal& signal, bool centering)
{
    if(centering)
    {

        plot->xAxis->setRange(signal.xmin + signal.original_range_x() / 2.0 - signal.original_range_x() * 0.05, signal.original_range_x() * 1.1,Qt::AlignLeft);
        plot->yAxis->setRange(signal.ymin - signal.original_range_y() * 0.05, signal.original_range_y() + signal.original_range_y() * 1.1,Qt::AlignLeft);
    }
    else
    {
        plot->xAxis->setRange(signal.xmin - signal.original_range_x() * 0.05, signal.original_range_x() * 1.1,Qt::AlignLeft);
        plot->yAxis->setRange(signal.ymin - signal.original_range_y() * 0.05, signal.original_range_y() * 1.1,Qt::AlignLeft);
    }
    plot->replot();
}


void MainWindow::populateLanguages()
{
    for(QString language : localization.getLanguages())
    {
        QAction* action = new QAction(language,this);

        connect(action, &QAction::triggered, [=]() {
            setLanguage(language);
         } );
        ui->menuLanguage->addAction(action);
    }

}


void MainWindow::setLanguage(QString name)
{
    if(name.length() == 0)
        return;
    localization.loadFile(config.getLocalizationFolder(),name);
    config.currentLanguage = name;

    this->setWindowTitle(localization("MainWindowTitle","FTutor"));
    ui->menuFile->setTitle(localization("menuFile","File"));
    ui->actionOpen->setText(localization("actionOpen","Open"));
    ui->actionSave->setText(localization("actionSave","Save"));
    ui->actionExit->setText(localization("actionExit","Exit"));
    ui->menuEdit->setTitle(localization("menuEdit","Edit"));
    ui->actionUndo->setText(localization("actionUndo","Undo"));
    ui->actionRevert_to_original->setText(localization("actionRevert_to_original","Revert to original"));
    ui->menuLanguage->setTitle(localization("menuLanguage","Language"));
    ui->menuFilters->setTitle(localization("menuFilters","Filters"));
    ui->menuHelp->setTitle(localization("menuHelp","Help"));
    ui->actionView_Help->setText(localization("Based on FTutor2D Sby Petr Sečkář","View Help"));
    ui->actionOfficial_Website->setText(localization("actionOfficial_Website","Official Website"));
    ui->actionAbout->setText(localization("actionAbout","About..."));
    ui->tabWidget->setTabText(0, localization("magnitudeLabel","Magnitude"));
    ui->tabWidget->setTabText(1, localization("phaseLabel","Phase"));
    ui->frequencyLabel->setText(localization("frequencyLabel","Current frequency"));
    ui->originalLabel->setText(localization("originalLabel","Original signal"));
    ui->filteredLabel->setText(localization("filteredLabel","Filtered signal"));
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
    xAxisChanged(ui->magnitudePlot,ui->magnitudeHorizontalScrollBar,magnitude,range);
}

void MainWindow::magnitudePlotyAxisChanged(QCPRange range)
{
    yAxisChanged(ui->magnitudePlot,ui->magnitudeVerticalScrollBar,magnitude,range);
}

void MainWindow::magnitudePlotMouseWheel()
{
    plotMouseWheel(ui->magnitudePlot);
}

void MainWindow::magnitudePlotMousePress()
{
    plotMousePress(ui->magnitudePlot);
}

void MainWindow::magnitudePlotDefautlScale()
{
    defaultScale(ui->magnitudePlot,magnitude, centering);
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
    xAxisChanged(ui->phasePlot,ui->phaseHorizontalScrollBar,phase,range);
}

void MainWindow::phasePlotyAxisChanged(QCPRange range)
{
    yAxisChanged(ui->phasePlot,ui->phaseVerticalScrollBar,phase,range);
}

void MainWindow::phasePlotMouseWheel()
{
    plotMouseWheel(ui->phasePlot);
}

void MainWindow::phasePlotMousePress()
{
    plotMousePress(ui->phasePlot);
}


void MainWindow::phasePlotDefautlScale()
{
    defaultScale(ui->phasePlot,phase, centering);
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
    xAxisChanged(ui->originalPlot,ui->originalHorizontalScrollBar,original,range);
}

void MainWindow::originalPlotyAxisChanged(QCPRange range)
{
    yAxisChanged(ui->originalPlot,ui->originalVerticalScrollBar,original,range);
}

void MainWindow::originalPlotMouseWheel()
{
    plotMouseWheel(ui->originalPlot);
}

void MainWindow::originalPlotMousePress()
{
    plotMousePress(ui->originalPlot);
}


void MainWindow::originalPlotDefautlScale()
{
    defaultScale(ui->originalPlot,original);
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
    xAxisChanged(ui->filteredPlot,ui->filteredHorizontalScrollBar,filtered,range);
}

void MainWindow::filteredPlotyAxisChanged(QCPRange range)
{
    yAxisChanged(ui->filteredPlot,ui->filteredVerticalScrollBar,filtered,range);
}

void MainWindow::filteredPlotMouseWheel()
{
    plotMouseWheel(ui->filteredPlot);
}

void MainWindow::filteredPlotMousePress()
{
    plotMousePress(ui->filteredPlot);
}


void MainWindow::filteredPlotDefautlScale()
{
    defaultScale(ui->filteredPlot,filtered);
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
    xAxisChanged(ui->frequencyPlot,ui->frequencyHorizontalScrollBar,frequency,range);
}

void MainWindow::frequencyPlotyAxisChanged(QCPRange range)
{
    yAxisChanged(ui->frequencyPlot,ui->frequencyVerticalScrollBar,frequency,range);
}

void MainWindow::frequencyPlotMouseWheel()
{
    plotMouseWheel(ui->frequencyPlot);
}

void MainWindow::frequencyPlotMousePress()
{
    plotMousePress(ui->frequencyPlot);
}


void MainWindow::frequencyPlotDefautlScale()
{
    defaultScale(ui->frequencyPlot,frequency);
}

void MainWindow::centeringToggled(bool toggled)
{
    this->centering = toggled;

    displaySignal(ui->magnitudePlot,this->magnitude, true, centering);
    displaySignal(ui->phasePlot,this->phase, true, centering);

    defaultScale(ui->magnitudePlot, this->magnitude, centering);
    defaultScale(ui->phasePlot, this->phase, centering);
}

void MainWindow::displayFrequency(QMouseEvent * event)
{
    if(!ui->magnitudePlot->graph())
    {
        return;
    }

    if(!vert)
    {
        vert = new QCPItemLine(ui->magnitudePlot);
        QPen pen;
        pen.setColor( Qt::green );
        vert->setPen( pen );
        QPen penSelect;
        penSelect.setColor( Qt::red );
        vert->setSelectedPen( penSelect );
        vert->setSelectable( true );
        vert->setSelected( true );
    }

    double x = ui->magnitudePlot->xAxis->pixelToCoord(event->pos().x());
    QMap<double, QCPData>::iterator i = ui->magnitudePlot->graph()->data()->lowerBound(x);

    if(i != ui->magnitudePlot->graph()->data()->end())
    {
        vert->start->setCoords(i->key, QCPRange::minRange);
        vert->end->setCoords(i->key, QCPRange::maxRange);

        if(!ui->magnitudePlot->layer("freq"))
        {
            ui->magnitudePlot->addLayer("freq",ui->magnitudePlot->layer("grid"),QCustomPlot::limAbove);
            vert->setLayer("freq");
            ui->magnitudePlot->addItem(vert);

        }
        ui->magnitudePlot->replot();

        Signal signal;
        signal.sinusWave(i->key);
        displaySignal(ui->frequencyPlot,signal);
    }
    //defaultScale(ui->frequencyPlot,signal,false);
}
