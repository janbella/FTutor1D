#include "mainwindow.h"

#include "aboutdialog.h"
#include "helpdialog.h"

#include "displaysignalwidgetinteractive.h"
#include "predefinedsignalsdialog.h"
#include "filterdialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // SETTINGS

    settings = new QSettings(QApplication::applicationDirPath() + QStringLiteral("/config.ini"),QSettings::IniFormat);

    if(!settings->contains(QStringLiteral("localizationFolder")))
    {
        settings->setValue(QStringLiteral("localizationFolder"), QApplication::applicationDirPath() + QStringLiteral("/translation"));
    }

    if(!settings->contains("predefinedSignalsFolder"))
    {
        settings->setValue(QStringLiteral("predefinedSignalsFolder"),QApplication::applicationDirPath() + QStringLiteral("/resources/signals"));
    }

    if(!settings->contains("appIcon"))
    {
        settings->setValue(QStringLiteral("appIcon"),QApplication::applicationDirPath() + QStringLiteral("/resources/Ftutor1D.ico"));
    }


    // WINDOW PROPERTIES

    // set window icon
    QIcon icon(settings->value(QStringLiteral("appIcon")).toString());
    setWindowIcon(icon);

    //window size
    setFixedSize(1000,660);

    // MENU

    actionOpen = new QAction(this);
    actionOpenPredefined = new QAction(this);
    actionSave = new QAction(this);
    actionExit = new QAction(this);

    actionUndo = new QAction(this);
    actionRevertToOriginal = new QAction(this);

    actionFilterIdealLowPass = new QAction(this);
    actionFilterIdealHighPass = new QAction(this);
    actionFilterBandPass = new QAction(this);
    actionFilterGaussianLowPass = new QAction(this);
    actionFilterGaussianHighPass = new QAction(this);
    actionFilterButterworthLowPass = new QAction(this);
    actionFilterButterworthHighPass = new QAction(this);

    actionDefaultScale = new QAction(this);
    actionDisplayLinesAll = new QAction(this);
    actionHideLinesAll = new QAction(this);
    actionAllowAutoScaling = new QAction(this);
    actionForbidAutoScaling = new QAction(this);

    actionViewHelp = new QAction(this);
    actionOfficialWebsite = new QAction(this);
    actionAbout = new QAction(this);

    menuBar = new QMenuBar(this);

    menuFile = new QMenu(menuBar);
    menuEdit = new QMenu(menuBar);
    menuFilters = new QMenu(menuBar);
    menuView = new QMenu(menuBar);
    menuLanguage = new QMenu(menuBar);
    menuHelp = new QMenu(menuBar);

    menuBar->addAction(menuFile->menuAction());
    menuBar->addAction(menuEdit->menuAction());
    menuBar->addAction(menuFilters->menuAction());
    menuBar->addAction(menuView->menuAction());
    menuBar->addAction(menuLanguage->menuAction());
    menuBar->addAction(menuHelp->menuAction());

    menuFile->addAction(actionOpen);
    menuFile->addAction(actionOpenPredefined);
    menuFile->addAction(actionSave);
    menuFile->addSeparator();
    menuFile->addAction(actionExit);

    menuEdit->addAction(actionUndo);
    menuEdit->addAction(actionRevertToOriginal);

    menuFilters->addAction(actionFilterIdealLowPass);
    menuFilters->addAction(actionFilterIdealHighPass);
    menuFilters->addAction(actionFilterBandPass);
    menuFilters->addAction(actionFilterGaussianLowPass);
    menuFilters->addAction(actionFilterGaussianHighPass);
    menuFilters->addAction(actionFilterButterworthLowPass);
    menuFilters->addAction(actionFilterButterworthHighPass);

    menuView->addAction(actionDefaultScale);
    menuView->addSeparator();
    menuView->addAction(actionDisplayLinesAll);
    menuView->addAction(actionHideLinesAll);
    menuView->addSeparator();

    menuView->addAction(actionAllowAutoScaling);
    menuView->addAction(actionForbidAutoScaling);


    menuHelp->addAction(actionViewHelp);
    menuHelp->addSeparator();
    menuHelp->addAction(actionOfficialWebsite);
    menuHelp->addAction(actionAbout);


    // other GUI

    centralWidget = new QWidget(this);

    magPhaseTabWidget = new QTabWidget(centralWidget);
    magPhaseTabWidget->setGeometry(QRect(10, 0, 470, 300));
    magPhaseTabWidget->setTabShape(QTabWidget::Rounded);
    magPhaseTabWidget->setUsesScrollButtons(false);
    magPhaseTabWidget->setTabBarAutoHide(false);

    magnitudeGraph = new DisplaySignalWidgetInteractive(false, true, centralWidget);

    magPhaseTabWidget->addTab(magnitudeGraph, QString());

    phaseGraph = new DisplaySignalWidgetInteractive(false, true, centralWidget);

    magPhaseTabWidget->addTab(phaseGraph, QString());

    centeringCheckBox = new QCheckBox(centralWidget);
    centeringCheckBox->setGeometry(QRect(385, 0, 100, 22));
    centeringCheckBox->setMinimumSize(QSize(100, 22));
    centeringCheckBox->setMaximumSize(QSize(170, 22));
    centeringCheckBox->setChecked(true);

    magnitudeGraph->enableCentering(true);
    phaseGraph->enableCentering(true);

    magPhaseTabWidget->setCurrentIndex(0);

    frequencySpectrumLabel = new QLabel(centralWidget);
    frequencySpectrumLabel->setGeometry(QRect(200, 0, 185, 22));

    sinCosTabWidget = new QTabWidget(centralWidget);
    sinCosTabWidget->setGeometry(QRect(510, 0, 470, 300));
    sinCosTabWidget->setTabShape(QTabWidget::Rounded);
    sinCosTabWidget->setUsesScrollButtons(false);
    sinCosTabWidget->setTabBarAutoHide(false);

    cosGraph = new DisplaySignalWidget(false, false, centralWidget);
    cosGraph->plot->setInteractions(QCP::Interactions());
    cosGraph->displayWithLines(true);

    sinCosTabWidget->addTab(cosGraph, QString());

    sinGraph = new DisplaySignalWidget(false, false, centralWidget);
    sinGraph->plot->setInteractions(QCP::Interactions());
    sinGraph->displayWithLines(true);

    sinCosTabWidget->addTab(sinGraph, QString());

    sinCosTabWidget->setCurrentIndex(0);

    selectedFrequencyLabel = new QLabel(centralWidget);
    selectedFrequencyLabel->setGeometry(QRect(700, 0, 185, 22));

    // no interactions allowed for frequency
    disconnect(sinGraph->plot, &QCustomPlot::customContextMenuRequested, sinGraph,&DisplaySignalWidget::contextMenuRequest);
    disconnect(cosGraph->plot, &QCustomPlot::customContextMenuRequested, cosGraph,&DisplaySignalWidget::contextMenuRequest);


    line = new QFrame(centralWidget);
    line->setGeometry(QRect(0, 303, 1000, 3));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    originalSignalGraph = new DisplaySignalWidget(true, true, centralWidget);
    originalSignalGraph->setGeometry(QRect(10, 300, 480, 300));

    filteredGraph = new DisplaySignalWidget(true, true, centralWidget);
    filteredGraph->setGeometry(QRect(510, 300, 480, 300));

    setCentralWidget(centralWidget);

    setMenuBar(menuBar);
    statusBar = new QStatusBar(this);
    statusBar->setObjectName(QStringLiteral("statusBar"));
    setStatusBar(statusBar);
    mainToolBar = new QToolBar(this);
    mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
    addToolBar(Qt::TopToolBarArea, mainToolBar);

    QMetaObject::connectSlotsByName(this);

    localization.initFromDirectory(settings->value(QStringLiteral("localizationFolder")).toString());
    populateLanguagesMenu();

    connect(actionAbout, &QAction::triggered, this, &MainWindow::showAboutDialog);
    connect(actionExit, &QAction::triggered, this, &MainWindow::exitApplication);
    connect(actionViewHelp, &QAction::triggered, this, &MainWindow::showHelpDialog);
    connect(actionOfficialWebsite, &QAction::triggered, this, [=]()
    {
        QString link = "http://github.com/janbella/FTutor";
        QDesktopServices::openUrl(QUrl(link));
    });

    connect(actionOpen, &QAction::triggered, this, [=](bool)
    {
        QString chosenFile = QFileDialog::getOpenFileName(this, QStringLiteral("Open signal..."));
        loadSignal(chosenFile.toStdString());
    });
    connect(actionOpenPredefined, &QAction::triggered, this, &MainWindow::openPredefinedSignalsDialog);
    connect(actionSave, &QAction::triggered, this, [=](bool){
        QString chosenFile = QFileDialog::getSaveFileName(this, QStringLiteral("Save Filtered Signal..."));
        filtered.save_file(chosenFile.toStdString());
    });


    connect(centeringCheckBox,&QCheckBox::toggled,magnitudeGraph,&DisplaySignalWidgetInteractive::enableCentering);
    connect(centeringCheckBox,&QCheckBox::toggled,phaseGraph,&DisplaySignalWidgetInteractive::enableCentering);
    connect(magnitudeGraph,&DisplaySignalWidgetInteractive::mouseMoved,this,&MainWindow::displayFrequency);
    connect(phaseGraph,&DisplaySignalWidgetInteractive::mouseMoved,this,&MainWindow::displayFrequency);

    connect(magnitudeGraph,&DisplaySignalWidgetInteractive::needUpdateFiltered, this, &MainWindow::updateFilteredSignalPlot);
    connect(phaseGraph,&DisplaySignalWidgetInteractive::needUpdateFiltered, this, &MainWindow::updateFilteredSignalPlot);

    connect(magnitudeGraph, &DisplaySignalWidgetInteractive::callForSaveState, this, &MainWindow::recordCurrentState);
    connect(phaseGraph, &DisplaySignalWidgetInteractive::callForSaveState, this, &MainWindow::recordCurrentState);

    connect(actionDefaultScale, &QAction::triggered, this, [=](bool)
    {
        originalSignalGraph->plotDefaultScale();
        cosGraph->plotDefaultScale();
        magnitudeGraph->plotDefaultScale();
        phaseGraph->plotDefaultScale();
        filteredGraph->plotDefaultScale();
    });

    connect(actionDisplayLinesAll, &QAction::triggered, this, [=](bool)
    {
        originalSignalGraph->displayWithLines(true);
        magnitudeGraph->displayWithLines(true);
        phaseGraph->displayWithLines(true);
        filteredGraph->displayWithLines(true);
    });

    connect(actionHideLinesAll, &QAction::triggered, this, [=](bool)
    {
        originalSignalGraph->displayWithLines(false);
        magnitudeGraph->displayWithLines(false);
        phaseGraph->displayWithLines(false);
        filteredGraph->displayWithLines(false);
    });

    connect(actionAllowAutoScaling, &QAction::triggered, this, [=](bool val)
    {
        magnitudeGraph->setAutoScaling(val);
        phaseGraph->setAutoScaling(val);
        originalSignalGraph->setAutoScaling(val);
        filteredGraph->setAutoScaling(val);
    });

    connect(actionForbidAutoScaling, &QAction::triggered, this, [=](bool val)
    {
        magnitudeGraph->setAutoScaling(val);
        phaseGraph->setAutoScaling(val);
        originalSignalGraph->setAutoScaling(val);
        filteredGraph->setAutoScaling(val);
    });


    connect(actionFilterIdealLowPass, &QAction::triggered,this, [=](bool)
    {
        if(magnitude.original_length() / 2.0 < 1)
        {
            noSignalWarning();
        }
        else
        {
            recordCurrentState();

            Translation* windowLanguage = localization.getCurrentLanguage()->getTranslationForWindow("FilterDialog");
            FilterDialog dialog(ILPF,magnitude,phase,windowLanguage,this);
            dialog.setModal(true);
            connect(&dialog,&FilterDialog::filterApplied, this, [=]()
            {
                resetAllGraphs();
            });
            if(dialog.exec() == QDialog::Rejected)
            {
                auto p = history.pop();
                delete p.first; delete p.second;
                if(history.empty()) actionUndo->setEnabled(false);
            }
        }
    });

    connect(actionFilterIdealHighPass, &QAction::triggered,this, [=](bool)
    {
        if(magnitude.original_length() / 2.0 < 1)
        {
            noSignalWarning();
        }
        else
        {
            recordCurrentState();

            Translation* windowLanguage = localization.getCurrentLanguage()->getTranslationForWindow("FilterDialog");
            FilterDialog dialog(IHPF,magnitude,phase,windowLanguage,this);
            dialog.setModal(true);
            connect(&dialog,&FilterDialog::filterApplied, this, [=]()
            {
                resetAllGraphs();
            });
            if(dialog.exec() == QDialog::Rejected)
            {
                auto p = history.pop();
                delete p.first; delete p.second;
                if(history.empty()) actionUndo->setEnabled(false);
            }
        }
    });

    connect(actionFilterBandPass, &QAction::triggered,this, [=](bool)
    {
        if(magnitude.original_length() / 2 == 0)
        {
            noSignalWarning();
        }
        else
        {
            recordCurrentState();

            Translation* windowLanguage = localization.getCurrentLanguage()->getTranslationForWindow("FilterDialog");
            FilterDialog dialog(BANDPASS,magnitude,phase,windowLanguage,this);
            dialog.setModal(true);
            connect(&dialog,&FilterDialog::filterApplied,[=](void)
            {
                resetAllGraphs();
            });
            if(dialog.exec() == QDialog::Rejected)
            {
                auto p = history.pop();
                delete p.first; delete p.second;
                if(history.empty()) actionUndo->setEnabled(false);
            }
        }
    });

    connect(actionFilterGaussianLowPass, &QAction::triggered,this, [=](bool)
    {
        if(magnitude.original_length() / 2 == 0)
        {
           noSignalWarning();
        }
        else
        {
            recordCurrentState();

            Translation* windowLanguage = localization.getCurrentLanguage()->getTranslationForWindow("FilterDialog");
            FilterDialog dialog(LPGAUSS,magnitude,phase,windowLanguage,this);
            dialog.setModal(true);
            connect(&dialog,&FilterDialog::filterApplied,[=]()
            {
                resetAllGraphs();
            });
            if(dialog.exec() == QDialog::Rejected)
            {
                auto p = history.pop();
                delete p.first; delete p.second;
                if(history.empty()) actionUndo->setEnabled(false);
            }
        }
    });

    connect(actionFilterGaussianHighPass, &QAction::triggered,this, [=](bool)
    {
        if(magnitude.original_length() / 2 == 0)
        {
            noSignalWarning();
        }
        else
        {
            recordCurrentState();

            Translation* windowLanguage = localization.getCurrentLanguage()->getTranslationForWindow("FilterDialog");
            FilterDialog dialog(HPGAUSS,magnitude,phase,windowLanguage,this);
            dialog.setModal(true);
            connect(&dialog,&FilterDialog::filterApplied,[=]()
            {
                resetAllGraphs();
            });
            if(dialog.exec() == QDialog::Rejected)
            {
                auto p = history.pop();
                delete p.first; delete p.second;
                if(history.empty()) actionUndo->setEnabled(false);
            }
        }
    });

    connect(actionFilterButterworthLowPass, &QAction::triggered,this, [=](bool)
    {
        if(magnitude.original_length() / 2 == 0)
        {
            noSignalWarning();
        }
        else
        {
            recordCurrentState();

            Translation* windowLanguage = localization.getCurrentLanguage()->getTranslationForWindow("FilterDialog");
            FilterDialog dialog(LPBUTTERWORTH,magnitude,phase,windowLanguage,this);
            dialog.setModal(true);
            connect(&dialog,&FilterDialog::filterApplied,[=]()
            {
                resetAllGraphs();
            });
            if(dialog.exec() == QDialog::Rejected)
            {
                auto p = history.pop();
                delete p.first; delete p.second;
                if(history.empty()) actionUndo->setEnabled(false);
            }
        }
    });

    connect(actionFilterButterworthHighPass, &QAction::triggered,this, [=](bool)
    {
        if(magnitude.original_length() / 2 == 0)
        {
            noSignalWarning();
        }
        else
        {
            recordCurrentState();

            Translation* windowLanguage = localization.getCurrentLanguage()->getTranslationForWindow("FilterDialog");
            FilterDialog dialog(HPBUTTERWORTH,magnitude,phase,windowLanguage,this);
            dialog.setModal(true);
            connect(&dialog,&FilterDialog::filterApplied,[=]()
            {
                resetAllGraphs();
            });
            if(dialog.exec() == QDialog::Rejected)
            {
                auto p = history.pop();
                delete p.first; delete p.second;
                if(history.empty()) actionUndo->setEnabled(false);
            }
        }
    });

    connect(actionUndo, &QAction::triggered,this,&MainWindow::undo);

    connect(actionRevertToOriginal, &QAction::triggered,this,[=](bool)
    {
        Signal::fourierTransform(original,magnitude,phase);

        magnitudeGraph->displaySignal(&magnitude);
        phaseGraph->displaySignal(&phase);
        originalSignalGraph->displaySignal(&original);
        filteredGraph->displaySignal(&original);

    });

    QString langName = settings->value(QStringLiteral("selectedLanguage")).toString();
    if(langName.isEmpty())
    {
        setDefaultTexts();
    }
    else
    {
        setLanguage(langName);
    }
}

MainWindow::~MainWindow()
{
    settings->sync();
    delete settings;

    delete actionOpen;
    delete actionOpenPredefined;
    delete actionSave;
    delete actionExit;

    delete actionUndo;
    delete actionRevertToOriginal;

    delete actionViewHelp;
    delete actionOfficialWebsite;
    delete actionAbout;

    delete actionFilterIdealLowPass;
    delete actionFilterIdealHighPass;
    delete actionFilterBandPass;
    delete actionFilterGaussianLowPass;

    delete actionDefaultScale;

    delete menuFile;
    delete menuEdit;
    delete menuLanguage;
    delete menuHelp;
    delete menuFilters;
    delete menuView;

    delete menuBar;

    delete statusBar;
    delete mainToolBar;

    delete magnitudeGraph;
    delete phaseGraph;
    delete originalSignalGraph;
    delete filteredGraph;
    delete cosGraph;

    delete line;
    delete magPhaseTabWidget;
    delete frequencySpectrumLabel;
    delete centeringCheckBox;

    delete centralWidget;
}


void MainWindow::exitApplication()
{
    close();
}


void MainWindow::showAboutDialog()
{
    Translation* lang = localization.getCurrentLanguage()->getTranslationForWindow("AboutDialog");
    AboutDialog dialog(this,lang, settings->value(QStringLiteral("appIcon")).toString());
    dialog.setModal(true);
    dialog.exec();
}


void MainWindow::showHelpDialog()
{
    Translation* lang = localization.getCurrentLanguage()->getTranslationForWindow("HelpDialog");

    HelpDialog dialog(this,lang);
    dialog.setModal(true);
    dialog.exec();
}


void MainWindow::loadSignal(std::string path)
{
    if(this->original.load_file(path))
    {
        original.fourierTransform(original,magnitude,phase);

        magnitudeGraph->displaySignal(&magnitude);
        phaseGraph->displaySignal(&phase);
        originalSignalGraph->displaySignal(&original);
        filteredGraph->displaySignal(&original);

        history.clear();
        actionUndo->setEnabled(false);
    }
}

void MainWindow::openPredefinedSignalsDialog()
{
    Translation* tr = localization.getCurrentLanguage();
    if(tr)
    {
        tr = tr->getTranslationForWindow(QStringLiteral("PredefinedSignalsDialog"));
    }
    PredefinedSignalsDialog dialog(this, settings->value("predefinedSignalsFolder").toString(),tr);
    dialog.setModal(true);
    connect(&dialog,&PredefinedSignalsDialog::signalChosen,[=](QString filename)
    {
        loadSignal(filename.toStdString());
    });
    dialog.exec();
    delete tr;
}


void MainWindow::populateLanguagesMenu()
{
    for(QString language : localization.getAvailableLanguages())
    {
        QAction* action = new QAction(language,this);

        connect(action, &QAction::triggered, [=]() {
            setLanguage(language);
        } );
        menuLanguage->addAction(action);
    }
}

void MainWindow::setDefaultTexts()
{
    setWindowTitle(QStringLiteral("FTutor1D: One Dimensional Fourier Transform Tutor"));

    menuFile->setTitle(QStringLiteral("File"));

    actionOpen->setText(QStringLiteral("Open"));
    actionOpenPredefined->setText(QStringLiteral("Open predefined signal"));
    actionSave->setText(QStringLiteral("Save"));
    actionExit->setText(QStringLiteral("Exit"));

    menuEdit->setTitle(QStringLiteral("Edit"));

    actionUndo->setText(QStringLiteral("Undo"));
    actionRevertToOriginal->setText(QStringLiteral("Revert to original"));

    menuFilters->setTitle(QStringLiteral("Filters"));

    actionFilterIdealLowPass->setText(QStringLiteral("Ideal low-pass"));
    actionFilterIdealHighPass->setText(QStringLiteral("Ideal high-pass"));
    actionFilterBandPass->setText(QStringLiteral("Band-pass"));
    actionFilterGaussianLowPass->setText(QStringLiteral("Gaussian low-pass"));
    actionFilterGaussianHighPass->setText(QStringLiteral("Gaussian high-pass"));
    actionFilterButterworthLowPass->setText(QStringLiteral("Butterworth low-pass"));
    actionFilterButterworthHighPass->setText(QStringLiteral("Butterworth high-pass"));

    menuView->setTitle(QStringLiteral("View"));

    actionDefaultScale->setText(QStringLiteral("Default scale"));
    actionDisplayLinesAll->setText(QStringLiteral("Display with lines"));
    actionHideLinesAll->setText(QStringLiteral("Hide lines"));
    actionAllowAutoScaling->setText(QStringLiteral("Allow autoscaling"));
    actionForbidAutoScaling->setText(QStringLiteral("Forbid autoscaling"));

    menuLanguage->setTitle(QStringLiteral("Language"));

    menuHelp->setTitle(QStringLiteral("Help"));

    actionViewHelp->setText(QStringLiteral("View Help"));
    actionOfficialWebsite->setText(QStringLiteral("Official website"));
    actionAbout->setText(QStringLiteral("About"));

    magPhaseTabWidget->setTabText(0, QStringLiteral("Magnitude"));
    magPhaseTabWidget->setTabText(1, QStringLiteral("Phase"));

    sinCosTabWidget->setTabText(0, QStringLiteral("Cosinus"));
    sinCosTabWidget->setTabText(1, QStringLiteral("Sinus"));

    frequencySpectrumLabel->setText(QStringLiteral("Frequency spectrum"));
    centeringCheckBox->setText(QStringLiteral("Centering"));

    selectedFrequencyLabel->setText(QStringLiteral("Selected frequency"));

    magnitudeGraph->setDefaultTexts();
    phaseGraph->setDefaultTexts();
    cosGraph->setDefaultTexts();
    sinGraph->setDefaultTexts();
    originalSignalGraph->setDefaultTexts();
    filteredGraph->setDefaultTexts();
}

void MainWindow::setLanguage(QString name)
{
    if(!name.isEmpty() && localization.setLanguage(name))
    {
        settings->setValue(QStringLiteral("selectedLanguage"),name);
        Translation* lang = localization.getCurrentLanguage();
        Translation* tr = lang->getTranslationForWindow(QStringLiteral("MainWindow"));

        setLocalizedTexts(tr);
        delete tr;
    }
}

void MainWindow::setLocalizedTexts(const Translation* language)
{
    if(language == nullptr)
    {
        setDefaultTexts();
    }

    setWindowTitle(language->getTitle());
    if(windowTitle().isEmpty()) setWindowTitle(QStringLiteral("FTutor1D: One Dimensional Fourier Transform Tutor"));


    Translation* menuBarLanguage = language->getTranslationForElement(QStringLiteral("menuBar"));

    Translation* menuFileLanguage = menuBarLanguage->getTranslationForElement(QStringLiteral("menuFile"));
    Translation* menuEditLanguage = menuBarLanguage->getTranslationForElement(QStringLiteral("menuEdit"));
    Translation* menuFiltersLanguage = menuBarLanguage->getTranslationForElement(QStringLiteral("menuFilters"));
    Translation* menuViewLanguage = menuBarLanguage->getTranslationForElement(QStringLiteral("menuView"));
    Translation* menuLanguageLanguage = menuBarLanguage->getTranslationForElement(QStringLiteral("menuLanguage"));
    Translation* menuHelpLanguage = menuBarLanguage->getTranslationForElement(QStringLiteral("menuHelp"));

    menuFile->setTitle(menuFileLanguage->getTitle());
    if(menuFile->title().isEmpty()) menuFile->setTitle(QStringLiteral("File"));

    actionOpen->setText(menuFileLanguage->getChildElementText(QStringLiteral("actionOpen")));
    if(actionOpen->text().isEmpty()) actionOpen->setText(QStringLiteral("Open"));
    actionOpenPredefined->setText(menuFileLanguage->getChildElementText(QStringLiteral("actionOpenPredefined")));
    if(actionOpenPredefined->text().isEmpty()) actionOpenPredefined->setText(QStringLiteral("Open predefined signal"));
    actionSave->setText(menuFileLanguage->getChildElementText(QStringLiteral("actionSave")));
    if(actionSave->text().isEmpty()) actionSave->setText(QStringLiteral("Save"));
    actionExit->setText(menuFileLanguage->getChildElementText(QStringLiteral("actionExit")));
    if(actionExit->text().isEmpty()) actionExit->setText(QStringLiteral("Exit"));

    menuEdit->setTitle(menuEditLanguage->getTitle());
    if(menuEdit->title().isEmpty()) menuEdit->setTitle(QStringLiteral("Edit"));

    actionUndo->setText(menuEditLanguage->getChildElementText(QStringLiteral("actionUndo")));
    if(actionUndo->text().isEmpty()) actionUndo->setText(QStringLiteral("Undo"));
    actionRevertToOriginal->setText(menuEditLanguage->getChildElementText(QStringLiteral("actionRevertToOriginal")));
    if(actionRevertToOriginal->text().isEmpty()) actionRevertToOriginal->setText(QStringLiteral("Revert to original"));


    menuFilters->setTitle(menuFiltersLanguage->getTitle());
    if(menuFilters->title().isEmpty()) menuFilters->setTitle(QStringLiteral("Filters"));

    actionFilterIdealLowPass->setText(menuFiltersLanguage->getChildElementText(QStringLiteral("actionFilterIdealLowPass")));
    if(actionFilterIdealLowPass->text().isEmpty()) actionFilterIdealLowPass->setText(QStringLiteral("Ideal low-pass"));

    actionFilterIdealHighPass->setText(menuFiltersLanguage->getChildElementText(QStringLiteral("actionFilterIdealHighPass")));
    if(actionFilterIdealHighPass->text().isEmpty()) actionFilterIdealHighPass->setText(QStringLiteral("Ideal high-pass"));

    actionFilterBandPass->setText(menuFiltersLanguage->getChildElementText(QStringLiteral("actionFilterBandPass")));
    if(actionFilterBandPass->text().isEmpty()) actionFilterBandPass->setText(QStringLiteral("Band-pass"));

    actionFilterGaussianLowPass->setText(menuFiltersLanguage->getChildElementText(QStringLiteral("actionFilterGaussianLowPass")));
    if(actionFilterGaussianLowPass->text().isEmpty()) actionFilterGaussianLowPass->setText(QStringLiteral("Gaussian low-pass"));

    actionFilterGaussianHighPass->setText(menuFiltersLanguage->getChildElementText(QStringLiteral("actionFilterGaussianHighPass")));
    if(actionFilterGaussianHighPass->text().isEmpty()) actionFilterGaussianHighPass->setText(QStringLiteral("Gaussian high-pass"));

    actionFilterButterworthLowPass->setText(menuFiltersLanguage->getChildElementText(QStringLiteral("actionFilterButterworthLowPass")));
    if(actionFilterButterworthLowPass->text().isEmpty()) actionFilterButterworthLowPass->setText(QStringLiteral("Butterworth low-pass"));

    actionFilterButterworthHighPass->setText(menuFiltersLanguage->getChildElementText(QStringLiteral("actionFilterButterworthHighPass")));
    if(actionFilterButterworthHighPass->text().isEmpty()) actionFilterButterworthHighPass->setText(QStringLiteral("Butterworth high-pass"));


    menuView->setTitle(menuViewLanguage->getTitle());
    if(menuView->title().isEmpty()) menuView->setTitle(QStringLiteral("View"));

    actionDefaultScale->setText(menuViewLanguage->getChildElementText(QStringLiteral("actionDefaultScale")));
    if(actionDefaultScale->text().isEmpty()) actionDefaultScale->setText(QStringLiteral("Default scale"));

    actionDisplayLinesAll->setText(menuViewLanguage->getChildElementText(QStringLiteral("actionDisplayLinesAll")));
    if(actionDisplayLinesAll->text().isEmpty()) actionDisplayLinesAll->setText(QStringLiteral("Display with lines"));

    actionHideLinesAll->setText(menuViewLanguage->getChildElementText(QStringLiteral("actionHideLinesAll")));
    if(actionHideLinesAll->text().isEmpty()) actionHideLinesAll->setText(QStringLiteral("Hide lines"));

    actionAllowAutoScaling->setText(menuViewLanguage->getChildElementText(QStringLiteral("actionAllowAutoScaling")));
    if(actionAllowAutoScaling->text().isEmpty()) actionAllowAutoScaling->setText(QStringLiteral("Allow autoscaling"));

    actionForbidAutoScaling->setText(menuViewLanguage->getChildElementText(QStringLiteral("actionForbidAutoScaling")));
    if(actionForbidAutoScaling->text().isEmpty()) actionForbidAutoScaling->setText(QStringLiteral("Forbid autoscaling"));


    menuLanguage->setTitle(menuLanguageLanguage->getTitle());
    if(menuLanguage->title().isEmpty()) menuLanguage->setTitle(QStringLiteral("Language"));


    menuHelp->setTitle(menuHelpLanguage->getTitle());
    if(menuHelp->title().isEmpty()) menuHelp->setTitle(QStringLiteral("Language"));

    actionViewHelp->setText(menuHelpLanguage->getChildElementText(QStringLiteral("actionViewHelp")));
    if(actionViewHelp->text().isEmpty()) actionViewHelp->setText(QStringLiteral("Help"));

    actionOfficialWebsite->setText(menuHelpLanguage->getChildElementText(QStringLiteral("actionOfficialWebsite")));
    if(actionOfficialWebsite->text().isEmpty()) actionOfficialWebsite->setText(QStringLiteral("Official website"));

    actionAbout->setText(menuHelpLanguage->getChildElementText(QStringLiteral("actionAbout")));
    if(actionAbout->text().isEmpty()) actionAbout->setText(QStringLiteral("About"));


    Translation* magPhaseTabWidgetLanguage = language->getTranslationForElement(QStringLiteral("magPhaseTabWidget"));

    magPhaseTabWidget->setTabText(0, magPhaseTabWidgetLanguage->getChildElementText(0));
    if(magPhaseTabWidget->tabText(0).isEmpty()) magPhaseTabWidget->setTabText(0, QStringLiteral("Magnitude"));

    magPhaseTabWidget->setTabText(1, magPhaseTabWidgetLanguage->getChildElementText(1));
    if(magPhaseTabWidget->tabText(1).isEmpty()) magPhaseTabWidget->setTabText(1, QStringLiteral("Phase"));

    frequencySpectrumLabel->setText(language->getChildElementText(QStringLiteral("frequencySpectrumLabel")));
    if(frequencySpectrumLabel->text().isEmpty()) frequencySpectrumLabel->setText(QStringLiteral("Frequency spectrum"));
    centeringCheckBox->setText(language->getChildElementText(QStringLiteral("centeringCheckBox")));
    if(centeringCheckBox->text().isEmpty()) centeringCheckBox->setText(QStringLiteral("Centering"));


    Translation* sinCosTabWidgetLanguage = language->getTranslationForElement(QStringLiteral("sinCosTabWidget"));

    sinCosTabWidget->setTabText(0, sinCosTabWidgetLanguage->getChildElementText(0));
    if(sinCosTabWidget->tabText(0).isEmpty()) sinCosTabWidget->setTabText(0, QStringLiteral("Cosinus"));

    sinCosTabWidget->setTabText(1, sinCosTabWidgetLanguage->getChildElementText(1));
    if(sinCosTabWidget->tabText(1).isEmpty()) sinCosTabWidget->setTabText(1, QStringLiteral("Sinus"));

    selectedFrequencyLabel->setText(language->getChildElementText(QStringLiteral("selectedFrequencyLabel")));
    if(selectedFrequencyLabel->text().isEmpty()) selectedFrequencyLabel->setText(QStringLiteral("Selected frequency"));


    Translation* magnitudeGraphLanguage = language->getTranslationForElement(QStringLiteral("magnitudeGraph"));
    Translation* phaseGraphLanguage = language->getTranslationForElement(QStringLiteral("phaseGraph"));
    Translation* filteredGraphLanguage = language->getTranslationForElement(QStringLiteral("filteredGraph"));
    Translation* originalGraphLanguage = language->getTranslationForElement(QStringLiteral("originalGraph"));
    Translation* cosGraphLanguage = language->getTranslationForElement(QStringLiteral("cosGraph"));
    Translation* sinGraphLanguage = language->getTranslationForElement(QStringLiteral("sinGraph"));

    magnitudeGraph->setLocalizedTexts(magnitudeGraphLanguage);
    phaseGraph->setLocalizedTexts(phaseGraphLanguage);
    cosGraph->setLocalizedTexts(cosGraphLanguage);
    sinGraph->setLocalizedTexts(sinGraphLanguage);
    originalSignalGraph->setLocalizedTexts(originalGraphLanguage);
    filteredGraph->setLocalizedTexts(filteredGraphLanguage);

    delete menuBarLanguage;

    delete menuFileLanguage;
    delete menuEditLanguage;
    delete menuFiltersLanguage;
    delete menuViewLanguage;
    delete menuLanguageLanguage ;
    delete menuHelpLanguage;
    delete magPhaseTabWidgetLanguage;
    delete sinCosTabWidgetLanguage;

    delete magnitudeGraphLanguage;
    delete phaseGraphLanguage;
    delete sinGraphLanguage;
    delete cosGraphLanguage;
    delete originalGraphLanguage;
    delete filteredGraphLanguage;
}



void MainWindow::displayFrequency(double x, double y)
{
    int length = original.original_length();
    Signal impulse = original.makeImpulse(x, sqrt(length));  // sqrt(length) so that IFT would be in range -1 - 1
    Signal zero;
    zero.zeroSignal(original.original_length());

    Signal::inverseFourierTransform(impulse,zero,cosinusFrequency);
    Signal::inverseFourierTransform(impulse,zero,sinusFrequency,false);

    Signal sinusWave;
    Signal cosinusWave;

    int frequency = (x <= length - x ? x : -(length - x));

    sinusWave.sinusWave(frequency, length, 20);
    cosinusWave.cosinusWave(frequency, length, 20);
    sinGraph->displayFrequency(&sinusFrequency,&sinusWave);
    cosGraph->displayFrequency(&cosinusFrequency,&cosinusWave);
}


void MainWindow::updateFilteredSignalPlot()
{
    Signal::inverseFourierTransform(magnitude,phase,filtered);

    magnitudeGraph->plotReplot();
    phaseGraph->plotReplot();
    filteredGraph->displaySignal(&filtered);
}

void MainWindow::resetAllGraphs()
{
    Signal::inverseFourierTransform(magnitude,phase,filtered);
    //Signal::fourierTransform(filtered,magnitude,phase);

    //magnitude.reset();
    //phase.reset();
    filtered.reset();

    magnitudeGraph->displaySignal(&magnitude);
    phaseGraph->displaySignal(&phase);
    filteredGraph->displaySignal(&filtered);
}

void MainWindow::recordCurrentState()
{
    history.push(QPair<Signal*, Signal*>(new Signal(magnitude), new Signal(phase)));
    actionUndo->setEnabled(true);
}

void MainWindow::undo()
{
    if(history.empty())
    {
        actionUndo->setEnabled(false);
        return;
    }

    QPair<Signal*, Signal*> toSet = history.pop();

    magnitude = *(toSet.first);
    phase =  *(toSet.second);

    delete toSet.first;
    delete toSet.second;

    resetAllGraphs();
    if(history.empty())
    {
        actionUndo->setEnabled(false);
    }
}

void MainWindow::noSignalWarning()
{
    Translation * tr1 = localization.getCurrentLanguage()->getTranslationForElement(QStringLiteral("MessageBox"));
    Translation * tr2 = tr1->getTranslationForUseCase(QStringLiteral("WarningNoSignal"));
    QString title = tr2->getTitle();
    QString sentence = tr2->getText();
    delete tr1; delete tr2;
    if(sentence.isEmpty()) sentence = QStringLiteral("There's no signal to filter! Load a file first.");

    QMessageBox mbx(QMessageBox::Icon::Warning, title, sentence, QMessageBox::Ok, this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    mbx.exec();
}
