#include "mainwindow.h"

#include "aboutdialog.h"
#include "helpdialog.h"

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
    createMenu();

    // other GUI

    centralWidget = new QWidget(this);

    magPhaseTabWidget = new QTabWidget(centralWidget);
    magPhaseTabWidget->setGeometry(QRect(10, 0, 470, 300));
    magPhaseTabWidget->setTabShape(QTabWidget::Rounded);
    magPhaseTabWidget->setUsesScrollButtons(false);
    magPhaseTabWidget->setTabBarAutoHide(false);

    magnitudeGraph = new DisplaySignalWidget(MAGNITUDE, false, centralWidget);

    magPhaseTabWidget->addTab(magnitudeGraph, QString());

    phaseGraph = new DisplaySignalWidget(PHASE, false, centralWidget);

    magPhaseTabWidget->addTab(phaseGraph, QString());

    centeringCheckBox = new QCheckBox(centralWidget);
    centeringCheckBox->setGeometry(QRect(385, 0, 100, 22));
    centeringCheckBox->setMinimumSize(QSize(100, 22));
    centeringCheckBox->setMaximumSize(QSize(170, 22));
    centeringCheckBox->setChecked(true);

    magPhaseTabWidget->setCurrentIndex(0);

    connect(magPhaseTabWidget, &QTabWidget::currentChanged, this, [=](int current)
    {
        if(current != 0) enableFilters(false);
        else if(!magnitude.empty()) enableFilters(true);
    });

    magnitudeGraph->enableCentering(true);
    phaseGraph->enableCentering(true);

    frequencySpectrumLabel = new QLabel(centralWidget);
    frequencySpectrumLabel->setGeometry(QRect(200, 0, 185, 22));

    fourierSpiral = new FourierSpiralWidget(centralWidget);
    fourierSpiral->setGeometry(510,30,470,270);

    normalizedCheckBox = new QCheckBox(centralWidget);
    normalizedCheckBox->setGeometry(865,0,115,22);
    normalizedCheckBox->setChecked(true);

    selectedFrequencyLabel = new QLabel(centralWidget);
    selectedFrequencyLabel->setGeometry(QRect(680, 0, 185, 22));

    line = new QFrame(centralWidget);

    line->setGeometry(QRect(0, 303, 1000, 3));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    originalSignalLabel = new QLabel(centralWidget);
    originalSignalLabel->setGeometry(QRect(200, 310, 185, 22));

    originalSignalGraph = new DisplaySignalWidget(ORIGINAL, true, centralWidget);
    originalSignalGraph->setGeometry(QRect(10, 340, 480, 300));

    filteredSignalLabel = new QLabel(centralWidget);
    filteredSignalLabel->setGeometry(QRect(700, 310, 185, 22));

    filteredGraph = new DisplaySignalWidget(FILTERED, false, centralWidget);
    filteredGraph->setGeometry(QRect(510, 340, 480, 300));

    originalSignalGraph->setSibling(filteredGraph);
    magnitudeGraph->setSibling(phaseGraph);

    setCentralWidget(centralWidget);

    statusBar = new QStatusBar(this);
    setStatusBar(statusBar);

    statusBarMessage = new QLabel(statusBar);
    statusBarMessage->setAlignment(Qt::AlignRight);
    statusBar->addWidget(statusBarMessage, 1);

    mainToolBar = new QToolBar(this);
    mainToolBar->setMovable(false);
    addToolBar(Qt::TopToolBarArea, mainToolBar);

    editModeContainer = new QWidget(this);
    editModeContainer->setGeometry(5,345,480,310);
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::red);
    editModeContainer->setAutoFillBackground(true);
    editModeContainer->setPalette(Pal);
    editModeGraph = new DisplaySignalWidget(EDIT_MODE, false,editModeContainer);
    editModeGraph->setGeometry(5,5,480,300);
    editModeFinishButton = new QPushButton(editModeContainer);
    editModeFinishButton->setGeometry(355,280,120,25);

    connect(editModeFinishButton, &QPushButton::clicked, this, &MainWindow::newSignalCreated);

    editModeCancelButton = new QPushButton(editModeContainer);
    editModeCancelButton->setGeometry(5,280,120,25);
    connect(editModeCancelButton, &QPushButton::clicked, this, &MainWindow::newSignalDiscarded);

    editModeContainer->setVisible(false);
    editModeContainer->setEnabled(false);

    connect(originalSignalGraph,&DisplaySignalWidget::openEditMode, this, [=]()
    {
        MainWindow::openEditMode(original);
    });

    localization.initFromDirectory(settings->value(QStringLiteral("localizationFolder")).toString());
    populateLanguagesMenu();

    connect(actionAbout, &QAction::triggered, this, &MainWindow::showAboutDialog);
    connect(actionExit, &QAction::triggered, this, &MainWindow::close);
    connect(actionViewHelp, &QAction::triggered, this, &MainWindow::showHelpDialog);

    connect(actionNew, &QAction::triggered, this,  [=]()
    {
        Signal emptySignal;

        MainWindow::openEditMode(emptySignal);
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

    connect(centeringCheckBox,&QCheckBox::toggled,magnitudeGraph,&DisplaySignalWidget::enableCentering);
    connect(centeringCheckBox,&QCheckBox::toggled,phaseGraph,&DisplaySignalWidget::enableCentering);

    connect(normalizedCheckBox, &QCheckBox::toggled, this, [=](bool val)
    {
        fourierSpiral->setNormalized(val);
    });

    connect(magnitudeGraph,&DisplaySignalWidget::mouseMoved,this,[=](int x, int y)
    {
        double f = (x <= magnitude.original_length() - x ? x : -(magnitude.original_length() - x));
        fourierSpiral->displayFrequency(f,y, phase.original[x],  magnitude.max_y(), phase.original_length());
    });

    connect(phaseGraph,&DisplaySignalWidget::mouseMoved,this,[=](int x, int y)
    {
        double f = (x <= phase.original_length() - x ? x : -(phase.original_length() - x));
        fourierSpiral->displayFrequency(f,magnitude.original[x],y, magnitude.max_y(), magnitude.original_length());
    });

    connect(magnitudeGraph,&DisplaySignalWidget::needUpdateFiltered, this, &MainWindow::updateFilteredSignalPlot);
    connect(phaseGraph,&DisplaySignalWidget::needUpdateFiltered, this, &MainWindow::updateFilteredSignalPlot);
    connect(editModeGraph,&DisplaySignalWidget::editModeNeedUpdate, this, &MainWindow::needUpdateMagPhaseFiltered);

    connect(magnitudeGraph, &DisplaySignalWidget::callForSaveState, this, &MainWindow::recordCurrentState);
    connect(phaseGraph, &DisplaySignalWidget::callForSaveState, this, &MainWindow::recordCurrentState);
    connect(editModeGraph, &DisplaySignalWidget::callForSaveEditModeState, this, &MainWindow::recordCurrentEditModeState);

    connect(editModeGraph, &DisplaySignalWidget::displayValue, this, [=](int x, int index)
    {
        statusBarMessage->setText(QStringLiteral("(") + QString::number(x,'f',6) + QStringLiteral("; ")  + QString::number(*(editSignal.original.begin() + index),'f',6) + QStringLiteral(")"));
    });

    connect(originalSignalGraph, &DisplaySignalWidget::displayValue, this, [=](int x, int index)
    {
        statusBarMessage->setText(QStringLiteral("(") + QString::number(x,'f',6) + QStringLiteral("; ")  + QString::number(*(original.original.begin() + index),'f',6) + QStringLiteral(")"));

    });

    connect(magnitudeGraph, &DisplaySignalWidget::displayValue, this, &MainWindow::showFrequencyInStatusBar);

    connect(phaseGraph, &DisplaySignalWidget::displayValue, this, &MainWindow::showFrequencyInStatusBar);

    connect(filteredGraph, &DisplaySignalWidget::displayValue, this, [=](int x, int index)
    {
        statusBarMessage->setText(QStringLiteral("(") + QString::number(x) + QStringLiteral("; ")
                                  + QString::number(*(filtered.original.begin() + index),'f',6) + QStringLiteral(")"));

    });

    connect(editModeGraph, &DisplaySignalWidget::mouseLeave, statusBarMessage, [=]()
    {
        statusBarMessage->clear();
    });
    connect(originalSignalGraph, &DisplaySignalWidget::mouseLeave, this, [=]() { statusBarMessage->clear(); });

    connect(magnitudeGraph, &DisplaySignalWidget::mouseLeave, this, [=]()
    {
        statusBarMessage->clear();
        fourierSpiral->clearFrequency();
    });

    connect(phaseGraph, &DisplaySignalWidget::mouseLeave, this, [=]()
    {
        statusBarMessage->clear();
        fourierSpiral->clearFrequency();
    });

    connect(filteredGraph, &DisplaySignalWidget::mouseLeave, this, [=]() { statusBarMessage->clear(); });

    connect(actionDefaultScale, &QAction::triggered, this, [=](bool)
    {
        originalSignalGraph->plotDefaultScale();
        magnitudeGraph->plotDefaultScale();
        phaseGraph->plotDefaultScale();
        filteredGraph->plotDefaultScale();
    });

    connect(actionDisplayLinesAll, &QAction::triggered, this, [=](bool checked)
    {
        originalSignalGraph->displayWithLines(checked);
        magnitudeGraph->displayWithLines(checked);
        phaseGraph->displayWithLines(checked);
        filteredGraph->displayWithLines(checked);
    });

    connect(actionAutoScalingAll, &QAction::triggered, this, [=](bool checked)
    {
        magnitudeGraph->setAutoScaling(checked);
        phaseGraph->setAutoScaling(checked);
        originalSignalGraph->setAutoScaling(checked);
        filteredGraph->setAutoScaling(checked);
    });

    connectFilterAction(actionFilterIdealLowPass, ILPF);
    connectFilterAction(actionFilterIdealHighPass, IHPF);
    connectFilterAction(actionFilterBandPass, BANDPASS);
    connectFilterAction(actionFilterGaussianLowPass, LPGAUSS);
    connectFilterAction(actionFilterGaussianHighPass, HPGAUSS);
    connectFilterAction(actionFilterButterworthLowPass, LPBUTTERWORTH);
    connectFilterAction(actionFilterButterworthHighPass, HPBUTTERWORTH);

    connect(actionUndo, &QAction::triggered,this,&MainWindow::undo);
    connect(actionRevertToOriginal, &QAction::triggered,this, &MainWindow::revertToOriginal);

    QString langName = settings->value(QStringLiteral("selectedLanguage")).toString();
    if(langName.isEmpty())
    {
        setDefaultTexts();
    }
    else
    {
        setLanguage(langName);
    }

    enableFilters(false);
    magnitudeGraph->setInteractionsEnabled(false);
    phaseGraph->setInteractionsEnabled(false);
    filteredGraph->setInteractionsEnabled(false);
    originalSignalGraph->setInteractionsEnabled(false);
    actionSave->setEnabled(false);
    actionUndo->setEnabled(false);
    actionRevertToOriginal->setEnabled(false);
    actionDefaultScale->setEnabled(false);
    actionDisplayLinesAll->setEnabled(false);
    actionAutoScalingAll->setEnabled(false);
}

void MainWindow::createMenu()
{
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


    actionNew = new QAction(menuFile);
    actionOpen = new QAction(menuFile);
    actionOpenPredefined = new QAction(menuFile);
    actionSave = new QAction(menuFile);
    actionExit = new QAction(menuFile);

    menuFile->addAction(actionNew);
    menuFile->addAction(actionOpen);
    menuFile->addAction(actionOpenPredefined);
    menuFile->addAction(actionSave);
    menuFile->addSeparator();
    menuFile->addAction(actionExit);

    actionUndo = new QAction(menuEdit);
    actionRevertToOriginal = new QAction(menuEdit);

    menuEdit->addAction(actionUndo);
    menuEdit->addAction(actionRevertToOriginal);

    actionFilterIdealLowPass = new QAction(menuFilters);
    actionFilterIdealHighPass = new QAction(menuFilters);
    actionFilterBandPass = new QAction(menuFilters);
    actionFilterGaussianLowPass = new QAction(menuFilters);
    actionFilterGaussianHighPass = new QAction(menuFilters);
    actionFilterButterworthLowPass = new QAction(menuFilters);
    actionFilterButterworthHighPass = new QAction(menuFilters);

    menuFilters->addAction(actionFilterIdealLowPass);
    menuFilters->addAction(actionFilterIdealHighPass);
    menuFilters->addAction(actionFilterBandPass);
    menuFilters->addAction(actionFilterGaussianLowPass);
    menuFilters->addAction(actionFilterGaussianHighPass);
    menuFilters->addAction(actionFilterButterworthLowPass);
    menuFilters->addAction(actionFilterButterworthHighPass);

    actionDefaultScale = new QAction(menuView);
    actionDisplayLinesAll = new QAction(menuView);
    actionDisplayLinesAll->setCheckable(true);
    actionDisplayLinesAll->setChecked(false);
    actionAutoScalingAll = new QAction(menuView);
    actionAutoScalingAll->setCheckable(true);
    actionAutoScalingAll->setChecked(true);

    menuView->addAction(actionDefaultScale);
    menuView->addAction(actionDisplayLinesAll);
    menuView->addAction(actionAutoScalingAll);

    actionViewHelp = new QAction(menuHelp);
    actionAbout = new QAction(menuHelp);

    menuHelp->addAction(actionViewHelp);
    menuHelp->addSeparator();
    menuHelp->addAction(actionAbout);

    setMenuBar(menuBar);
}

MainWindow::~MainWindow()
{
    settings->sync();

    delete magnitudeGraph;
    delete phaseGraph;

    delete fourierSpiral;

    delete originalSignalGraph;
    delete filteredGraph;

    delete line;

    delete magPhaseTabWidget;
    delete frequencySpectrumLabel;
    delete centeringCheckBox;

    delete selectedFrequencyLabel;
    delete normalizedCheckBox;

    delete statusBarMessage;
    delete statusBar;
    delete mainToolBar;

    delete centralWidget;

    delete actionNew;
    delete actionOpen;
    delete actionOpenPredefined;
    delete actionSave;
    delete actionExit;

    delete actionUndo;
    delete actionRevertToOriginal;

    delete actionFilterIdealLowPass;
    delete actionFilterIdealHighPass;
    delete actionFilterBandPass;
    delete actionFilterGaussianLowPass;
    delete actionFilterGaussianHighPass;
    delete actionFilterButterworthLowPass;
    delete actionFilterButterworthHighPass;

    delete actionDefaultScale;
    delete actionDisplayLinesAll;
    delete actionAutoScalingAll;

    delete actionViewHelp;
    delete actionAbout;

    delete menuFile;
    delete menuEdit;
    delete menuFilters;
    delete menuView;
    delete menuLanguage;
    delete menuHelp;

    delete menuBar;
}

void MainWindow::showAboutDialog()
{
    Translation* currentLanguage = localization.getCurrentLanguage();
    Translation* aboutDialogTranslation = nullptr;
    if(currentLanguage)
        aboutDialogTranslation = currentLanguage->getTranslationForWindow(QStringLiteral("AboutDialog"));
    AboutDialog dialog(this,aboutDialogTranslation, settings->value(QStringLiteral("appIcon")).toString());
    dialog.setModal(true);
    dialog.exec();
}


void MainWindow::showHelpDialog()
{
    Translation* currentLanguage = localization.getCurrentLanguage();
    Translation* helpDialogTranslation = nullptr;
    if(currentLanguage)
        helpDialogTranslation = currentLanguage->getTranslationForWindow(QStringLiteral("HelpDialog"));

    HelpDialog dialog(this,helpDialogTranslation);
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

        filtered = original;
        filteredGraph->displaySignal(&filtered);

        history.clear();
        actionUndo->setEnabled(false);

        enableFilters(true);
        magnitudeGraph->setInteractionsEnabled(true);
        phaseGraph->setInteractionsEnabled(true);
        filteredGraph->setInteractionsEnabled(true);
        originalSignalGraph->setInteractionsEnabled(true);
        actionSave->setEnabled(true);
        actionRevertToOriginal->setEnabled(true);
        actionDefaultScale->setEnabled(true);
        actionDisplayLinesAll->setEnabled(true);
        actionAutoScalingAll->setEnabled(true);
    }
}


void MainWindow::openPredefinedSignalsDialog()
{
    Translation* tr = localization.getCurrentLanguage();
    if(tr)
    {
        tr = tr->getTranslationForWindow(QStringLiteral("PredefinedSignalsDialog"));
    }
    PredefinedSignalsDialog dialog(this, settings->value(QStringLiteral("predefinedSignalsFolder")).toString(),tr);
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

        connect(action, &QAction::triggered, [=]()
        {
            setLanguage(language);
        } );
        menuLanguage->addAction(action);
    }
}


void MainWindow::setDefaultTexts()
{
    setWindowTitle(QStringLiteral("FTutor1D: One Dimensional Fourier Transform Tutor"));

    menuFile->setTitle(QStringLiteral("File"));

    actionNew->setText(QStringLiteral("New"));
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
    actionAutoScalingAll->setText(QStringLiteral("Allow autoscaling"));

    menuLanguage->setTitle(QStringLiteral("Language"));

    menuHelp->setTitle(QStringLiteral("Help"));

    actionViewHelp->setText(QStringLiteral("View Help"));
    actionAbout->setText(QStringLiteral("About"));

    magPhaseTabWidget->setTabText(0, QStringLiteral("Magnitude"));
    magPhaseTabWidget->setTabText(1, QStringLiteral("Phase"));

    frequencySpectrumLabel->setText(QStringLiteral("Frequency spectrum"));
    centeringCheckBox->setText(QStringLiteral("Centering"));

    selectedFrequencyLabel->setText(QStringLiteral("Selected frequency"));
    normalizedCheckBox->setText(QStringLiteral("Normalized"));

    originalSignalLabel->setText(QStringLiteral("Original signal"));
    filteredSignalLabel->setText(QStringLiteral("Filtered signal"));

    magnitudeGraph->setDefaultTexts();
    phaseGraph->setDefaultTexts();
    originalSignalGraph->setDefaultTexts();
    filteredGraph->setDefaultTexts();

    editModeFinishButton->setText(QStringLiteral("Finish editing"));
    editModeCancelButton->setText(QStringLiteral("Cancel"));
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

    actionNew->setText(menuFileLanguage->getChildElementText(QStringLiteral("actionNew")));
    if(actionNew->text().isEmpty()) actionNew->setText(QStringLiteral("New"));
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

    actionAutoScalingAll->setText(menuViewLanguage->getChildElementText(QStringLiteral("actionAutoScalingAll")));
    if(actionAutoScalingAll->text().isEmpty()) actionAutoScalingAll->setText(QStringLiteral("Allow autoscaling"));


    menuLanguage->setTitle(menuLanguageLanguage->getTitle());
    if(menuLanguage->title().isEmpty()) menuLanguage->setTitle(QStringLiteral("Language"));


    menuHelp->setTitle(menuHelpLanguage->getTitle());
    if(menuHelp->title().isEmpty()) menuHelp->setTitle(QStringLiteral("Language"));

    actionViewHelp->setText(menuHelpLanguage->getChildElementText(QStringLiteral("actionViewHelp")));
    if(actionViewHelp->text().isEmpty()) actionViewHelp->setText(QStringLiteral("Help"));

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

    selectedFrequencyLabel->setText(language->getChildElementText(QStringLiteral("selectedFrequencyLabel")));
    if(selectedFrequencyLabel->text().isEmpty()) selectedFrequencyLabel->setText(QStringLiteral("Selected frequency"));

    normalizedCheckBox->setText(language->getChildElementText(QStringLiteral("normalizedCheckBox")));
    if(normalizedCheckBox->text().isEmpty()) normalizedCheckBox->setText(QStringLiteral("Normalized"));


    originalSignalLabel->setText(language->getChildElementText(QStringLiteral("originalSignalLabel")));
    if(originalSignalLabel->text().isEmpty()) originalSignalLabel->setText(QStringLiteral("Original signal"));

    filteredSignalLabel->setText(language->getChildElementText(QStringLiteral("filteredSignalLabel")));
    if(filteredSignalLabel->text().isEmpty()) filteredSignalLabel->setText(QStringLiteral("Filtered signal"));

    editModeFinishButton->setText(language->getChildElementText(QStringLiteral("editModeFinishButton")));
    if(editModeFinishButton->text().isEmpty()) editModeFinishButton->setText(QStringLiteral("Finish editing"));

    editModeCancelButton->setText(language->getChildElementText(QStringLiteral("editModeCancelButton")));
    if(editModeCancelButton->text().isEmpty()) editModeCancelButton->setText(QStringLiteral("Cancel"));


    Translation* magnitudeGraphLanguage = language->getTranslationForElement(QStringLiteral("magnitudeGraph"));
    Translation* phaseGraphLanguage = language->getTranslationForElement(QStringLiteral("phaseGraph"));
    Translation* filteredGraphLanguage = language->getTranslationForElement(QStringLiteral("filteredGraph"));
    Translation* originalGraphLanguage = language->getTranslationForElement(QStringLiteral("originalGraph"));

    magnitudeGraph->setLocalizedTexts(magnitudeGraphLanguage);
    phaseGraph->setLocalizedTexts(phaseGraphLanguage);
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
    delete originalGraphLanguage;
    delete filteredGraphLanguage;
}


void MainWindow::updateFilteredSignalPlot()
{
    Signal::inverseFourierTransform(magnitude,phase,filtered,original.original.keys().toVector());

    magnitudeGraph->plotReplot();
    phaseGraph->plotReplot();
    filteredGraph->displaySignal(&filtered);
    //fourierSpiral->setMagnitude(magnitude.max_y());
}


void MainWindow::resetAllGraphs(bool shadowPrevious)
{
    Signal::inverseFourierTransform(magnitude,phase,filtered,original.original.keys().toVector());
    //Signal::fourierTransform(filtered,magnitude,phase);

    //magnitude.reset();
    //phase.reset();
    filtered.reset();

    magnitudeGraph->displaySignal(&magnitude, shadowPrevious);
    phaseGraph->displaySignal(&phase, shadowPrevious);
    filteredGraph->displaySignal(&filtered, shadowPrevious);
}


void MainWindow::recordCurrentState()
{
    history.push(QPair<Signal*, Signal*>(new Signal(magnitude), new Signal(phase)));
    actionUndo->setEnabled(true);
}


void MainWindow::undo()
{
    if(!editModeHistory.empty())
    {
        Signal* sig = editModeHistory.pop();
        editSignal = *sig;
        delete sig;

        editModeGraph->displaySignal(&editSignal);

        original = editSignal;

        Signal::fourierTransform(editSignal,magnitude,phase);
        Signal::inverseFourierTransform(magnitude,phase,filtered,original.original.keys().toVector());
        resetAllGraphs(false);
        if(editModeHistory.empty())
        {
            actionUndo->setEnabled(false);
        }
    }
    else if(!history.empty())
    {
        QPair<Signal*, Signal*> toSet = history.pop();

        magnitude = *(toSet.first);
        phase =  *(toSet.second);

        delete toSet.first;
        delete toSet.second;

        resetAllGraphs(false);
        if(history.empty())
        {
            actionUndo->setEnabled(false);
        }
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


void MainWindow::connectFilterAction(QAction* action, FilterType type)
{
    connect(action, &QAction::triggered,this, [=](bool)
    {
        if(magnitude.original_length() / 2.0 < 1)
        {
            noSignalWarning();
        }
        else
        {
            recordCurrentState();

            Translation* currentLanguage = localization.getCurrentLanguage();
            Translation* windowLanguage = nullptr;
            if(currentLanguage)
                windowLanguage = currentLanguage->getTranslationForWindow(QStringLiteral("FilterDialog"));

            FilterDialog dialog(type,magnitude,phase,windowLanguage,this);
            dialog.setModal(true);
            connect(&dialog,&FilterDialog::filterApplied, this, [=]()
            {
                resetAllGraphs(true);
            });
            if(dialog.exec() == QDialog::Rejected)
            {
                auto p = history.pop();
                delete p.first; delete p.second;
                if(history.empty()) actionUndo->setEnabled(false);
            }
        }
    });
}


void MainWindow::needUpdateMagPhaseFiltered()
{
    Signal::fourierTransform(editSignal,magnitude,phase);
    editSignal.reset();
    magnitude.reset();
    phase.reset();
    original = (editSignal);
    filtered = original;
    originalSignalGraph->displaySignal(&original);
    magnitudeGraph->displaySignal(&magnitude);
    phaseGraph->displaySignal(&phase);
    filteredGraph->displaySignal(&filtered);
}


void MainWindow::recordCurrentEditModeState()
{
    editModeHistory.push(new Signal(original));
    actionUndo->setEnabled(true);
}

void MainWindow::enableFilters(bool val)
{
    actionFilterBandPass->setEnabled(val);
    actionFilterButterworthHighPass->setEnabled(val);
    actionFilterButterworthLowPass->setEnabled(val);
    actionFilterGaussianHighPass->setEnabled(val);
    actionFilterGaussianLowPass->setEnabled(val);
    actionFilterIdealHighPass->setEnabled(val);
    actionFilterIdealLowPass->setEnabled(val);
}

void MainWindow::newSignalCreated()
{
    editSignal.findYMinMax();
    original = editSignal;
    originalSignalGraph->displaySignal(&original);

    editModeContainer->setVisible(false);
    editModeContainer->setEnabled(false);

    emptyHistoryStacks();

    if(magPhaseTabWidget->currentIndex() == 0 && !original.empty())
    {
        enableFilters(true);
    }

    magnitudeGraph->setEnabled(true);
    phaseGraph->setEnabled(true);
    filteredGraph->setEnabled(true);

    filtered.findYMinMax();
    magnitude.extend_left();
    magnitude.extend_right();
    phase.extend_left();
    phase.extend_right();


    filteredGraph->plotDefaultScale();
    magnitudeGraph->plotDefaultScale();
    phaseGraph->plotDefaultScale();

    actionNew->setEnabled(true);
    actionOpen->setEnabled(true);
    actionOpenPredefined->setEnabled(true);

    if(original.empty())
    {
        magnitudeGraph->setInteractionsEnabled(false);
        phaseGraph->setInteractionsEnabled(false);
        filteredGraph->setInteractionsEnabled(false);
        originalSignalGraph->setInteractionsEnabled(false);
    }
    else
    {
        actionRevertToOriginal->setEnabled(true);
        actionSave->setEnabled(true);
        actionDefaultScale->setEnabled(true);
        actionDisplayLinesAll->setEnabled(true);
        actionAutoScalingAll->setEnabled(true);

        magnitudeGraph->setInteractionsEnabled(true);
        phaseGraph->setInteractionsEnabled(true);
        filteredGraph->setInteractionsEnabled(true);
        originalSignalGraph->setInteractionsEnabled(true);
    }
}

void MainWindow::newSignalDiscarded()
{
    while(!editModeHistory.empty())
    {
        delete editModeHistory.pop();
    }
    if(history.empty())
    {
        actionUndo->setEnabled(false);
    }
    editSignal = prevOriginal;
    needUpdateMagPhaseFiltered();

    editModeContainer->setVisible(false);
    editModeContainer->setEnabled(false);

    if(magPhaseTabWidget->currentIndex() == 0 && !original.empty())
    {
        enableFilters(true);
    }

    magnitudeGraph->setEnabled(true);
    phaseGraph->setEnabled(true);
    filteredGraph->setEnabled(true);

    actionNew->setEnabled(true);
    actionOpen->setEnabled(true);
    actionOpenPredefined->setEnabled(true);

    if(original.empty())
    {
        magnitudeGraph->setInteractionsEnabled(false);
        phaseGraph->setInteractionsEnabled(false);
        filteredGraph->setInteractionsEnabled(false);
        originalSignalGraph->setInteractionsEnabled(false);
    }
    else
    {
        actionRevertToOriginal->setEnabled(true);
        actionSave->setEnabled(true);
        actionDefaultScale->setEnabled(true);
        actionDisplayLinesAll->setEnabled(true);
        actionAutoScalingAll->setEnabled(true);

        magnitudeGraph->setInteractionsEnabled(true);
        phaseGraph->setInteractionsEnabled(true);
        filteredGraph->setInteractionsEnabled(true);
        originalSignalGraph->setInteractionsEnabled(true);
    }

}

void MainWindow::openEditMode(Signal& toEdit)
{
    fourierSpiral->clearFrequency();
    filtered = Signal();
    magnitudeGraph->setEnabled(false);
    phaseGraph->setEnabled(false);
    filteredGraph->setEnabled(false);
    enableFilters(false);
    actionNew->setEnabled(false);
    actionOpen->setEnabled(false);
    actionOpenPredefined->setEnabled(false);
    actionSave->setEnabled(false);
    actionRevertToOriginal->setEnabled(false);
    actionDefaultScale->setEnabled(false);
    actionDisplayLinesAll->setEnabled(false);
    actionAutoScalingAll->setEnabled(false);

    prevOriginal = original;

    editModeContainer->setVisible(true);
    editModeContainer->setEnabled(true);
    editSignal = toEdit;
    editSignal.reset();
    editModeGraph->displaySignal(&editSignal);

    Signal::fourierTransform(editSignal,magnitude,phase);
    magnitudeGraph->displaySignal(&magnitude);
    phaseGraph->displaySignal(&phase);

}

void MainWindow::showFrequencyInStatusBar(int x, int index)
{
    double mag = *(magnitude.original.begin() + index);
    double pha = *(phase.original.begin() + index);
    double cospha = cosf(*(phase.original.begin() + index));
    double sinpha = sinf(*(phase.original.begin() + index));
    double real = cospha == 0 ? 0 : mag * cospha;
    double imag = sinpha == 0 ? 0 : mag * sinpha;
    std::stringstream ss;
    ss << "(ω, F(ω)) = (" << std::fixed << std::setprecision(3) << x << ", "  << real << std::showpos << imag << "i)";
    statusBarMessage->setText(QString::fromStdString(ss.str()));

    fourierSpiral->setMagnitudeAndPhase(mag,pha);
}

void MainWindow::revertToOriginal()
{
    emptyHistoryStacks();
    Signal::fourierTransform(original,magnitude,phase);
    filtered = Signal(original);
    magnitudeGraph->displaySignal(&magnitude);
    phaseGraph->displaySignal(&phase);
    filteredGraph->displaySignal(&filtered);
}

void MainWindow::emptyHistoryStacks()
{
    while(!editModeHistory.empty())
    {
        delete editModeHistory.pop();
    }

    while(!history.empty())
    {
        QPair<Signal*, Signal*> toDelete = history.pop();
        delete toDelete.first;
        delete toDelete.second;
    }

    actionUndo->setEnabled(false);
}
