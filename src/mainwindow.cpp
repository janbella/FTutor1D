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

    magnitudeGraph = new DisplaySignalWidget(BASIC_INTERACTION, FREQUENCY, false, centralWidget);

    magPhaseTabWidget->addTab(magnitudeGraph, QString());

    phaseGraph = new DisplaySignalWidget(BASIC_INTERACTION, FREQUENCY, false, centralWidget);

    magPhaseTabWidget->addTab(phaseGraph, QString());

    centeringCheckBox = new QCheckBox(centralWidget);
    centeringCheckBox->setGeometry(QRect(385, 0, 100, 22));
    centeringCheckBox->setMinimumSize(QSize(100, 22));
    centeringCheckBox->setMaximumSize(QSize(170, 22));
    centeringCheckBox->setChecked(true);

    magPhaseTabWidget->setCurrentIndex(0);

    connect(magPhaseTabWidget, &QTabWidget::currentChanged, this, [=](int current)
    {
        if(current != 0)
        {
            actionFilterBandPass->setEnabled(false);
            actionFilterButterworthHighPass->setEnabled(false);
            actionFilterButterworthLowPass->setEnabled(false);
            actionFilterGaussianHighPass->setEnabled(false);
            actionFilterGaussianLowPass->setEnabled(false);
            actionFilterIdealHighPass->setEnabled(false);
            actionFilterIdealLowPass->setEnabled(false);
        }
        else if(!magnitude.empty())
        {
            actionFilterBandPass->setEnabled(true);
            actionFilterButterworthHighPass->setEnabled(true);
            actionFilterButterworthLowPass->setEnabled(true);
            actionFilterGaussianHighPass->setEnabled(true);
            actionFilterGaussianLowPass->setEnabled(true);
            actionFilterIdealHighPass->setEnabled(true);
            actionFilterIdealLowPass->setEnabled(true);
        }

    });

    magnitudeGraph->enableCentering(true);
    phaseGraph->enableCentering(true);

    frequencySpectrumLabel = new QLabel(centralWidget);
    frequencySpectrumLabel->setGeometry(QRect(200, 0, 185, 22));

    sinCosTabWidget = new QTabWidget(centralWidget);
    sinCosTabWidget->setGeometry(QRect(510, 0, 470, 300));
    sinCosTabWidget->setTabShape(QTabWidget::Rounded);
    sinCosTabWidget->setUsesScrollButtons(false);
    sinCosTabWidget->setTabBarAutoHide(false);

    cosGraph = new DisplaySignalWidget(FREQUENCY_NO_INTERACTION, TIME, false, centralWidget);

    cosGraph->displayWithLines(true);

    sinCosTabWidget->addTab(cosGraph, QString());

    sinGraph = new DisplaySignalWidget(FREQUENCY_NO_INTERACTION, TIME, false, centralWidget);

    sinGraph->displayWithLines(true);

    sinCosTabWidget->addTab(sinGraph, QString());

    sinCosTabWidget->setCurrentIndex(0);

    selectedFrequencyLabel = new QLabel(centralWidget);
    selectedFrequencyLabel->setGeometry(QRect(700, 0, 185, 22));

    line = new QFrame(centralWidget);

    line->setGeometry(QRect(0, 303, 1000, 3));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    originalSignalLabel = new QLabel(centralWidget);
    originalSignalLabel->setGeometry(QRect(200, 310, 185, 22));

    originalSignalGraph = new DisplaySignalWidget(BASIC, TIME, true, centralWidget);
    originalSignalGraph->setGeometry(QRect(10, 340, 480, 300));

    filteredSignalLabel = new QLabel(centralWidget);
    filteredSignalLabel->setGeometry(QRect(700, 310, 185, 22));

    filteredGraph = new DisplaySignalWidget(BASIC, TIME, false, centralWidget);
    filteredGraph->setGeometry(QRect(510, 340, 480, 300));

    originalSignalGraph->setSibling(filteredGraph);
    magnitudeGraph->setSibling(phaseGraph);
    sinGraph->setSibling(cosGraph);


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
    editModeGraph = new DisplaySignalWidget(EDIT_MODE, TIME, false,editModeContainer);
    editModeGraph->setGeometry(5,5,480,300);
    editModeFinishButton = new QPushButton(editModeContainer);
    editModeFinishButton->setGeometry(355,280,120,25);
    connect(editModeFinishButton, &QPushButton::clicked, this, [=](bool)
    {
        editModeContainer->setVisible(false);
        editModeContainer->setEnabled(false);

        magnitudeGraph->setEnabled(true);
        phaseGraph->setEnabled(true);

        while(!editModeHistory.isEmpty())
        {
            Signal* sig = editModeHistory.pop();
            delete sig;
        }

    });

    editModeCancelButton = new QPushButton(editModeContainer);
    editModeCancelButton->setGeometry(5,280,120,25);
    connect(editModeCancelButton, &QPushButton::clicked, this, [=](bool)
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

        magnitudeGraph->setEnabled(true);
        phaseGraph->setEnabled(true);
    });

    editModeContainer->setVisible(false);
    editModeContainer->setEnabled(false);

    connect(originalSignalGraph,&DisplaySignalWidget::openEditMode, this, [=]()
    {
        prevOriginal = original;
        editModeContainer->setVisible(true);
        editModeContainer->setEnabled(true);
        editSignal = original;
        editSignal.reset();
        editModeGraph->displaySignal(&editSignal);
        magnitudeGraph->setEnabled(false);
        phaseGraph->setEnabled(false);
    });

    localization.initFromDirectory(settings->value(QStringLiteral("localizationFolder")).toString());
    populateLanguagesMenu();

    connect(actionAbout, &QAction::triggered, this, &MainWindow::showAboutDialog);
    connect(actionExit, &QAction::triggered, this, &MainWindow::exitApplication);
    connect(actionViewHelp, &QAction::triggered, this, &MainWindow::showHelpDialog);

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
    connect(magnitudeGraph,&DisplaySignalWidget::mouseMoved,this,&MainWindow::displayFrequency);
    connect(phaseGraph,&DisplaySignalWidget::mouseMoved,this,&MainWindow::displayFrequency);

    connect(magnitudeGraph,&DisplaySignalWidget::needUpdateFiltered, this, &MainWindow::updateFilteredSignalPlot);
    connect(phaseGraph,&DisplaySignalWidget::needUpdateFiltered, this, &MainWindow::updateFilteredSignalPlot);
    connect(editModeGraph,&DisplaySignalWidget::editModeNeedUpdate, this, &MainWindow::needUpdateMagPhaseFiltered);

    connect(magnitudeGraph, &DisplaySignalWidget::callForSaveState, this, &MainWindow::recordCurrentState);
    connect(phaseGraph, &DisplaySignalWidget::callForSaveState, this, &MainWindow::recordCurrentState);
    connect(editModeGraph, &DisplaySignalWidget::callForSaveEditModeState, this, &MainWindow::recordCurrentEditModeState);

    connect(editModeGraph, &DisplaySignalWidget::displayValue, this, [=](int index)
    {
        std::stringstream ss;
        ss << "Value under cursor: " << *(editSignal.original.begin() + index);
        //statusBar->showMessage(QString::fromStdString(ss.str()));
        statusBarMessage->setText(QString::fromStdString(ss.str()));
    });

    connect(originalSignalGraph, &DisplaySignalWidget::displayValue, this, [=](int index)
    {
        std::stringstream ss;
        ss << "Value under cursor: " << *(original.original.begin() + index);
        //statusBar->showMessage(QString::fromStdString(ss.str()));
        statusBarMessage->setText(QString::fromStdString(ss.str()));
    });

    connect(magnitudeGraph, &DisplaySignalWidget::displayValue, this, [=](int index)
    {
        double mag = *(magnitude.original.begin() + index);
        double pha = *(phase.original.begin() + index);
        double cospha = cos(pha);
        double sinpha = sin(pha);
        double real = cospha == 0 ? 0 : mag / cospha;
        double imag = sinpha == 0 ? 0 : mag / sinpha;
        std::stringstream ss;
        ss << "Value under cursor: " << real << std::showpos << imag << 'i';
        //statusBar->showMessage(QString::fromStdString(ss.str()));
        statusBarMessage->setText(QString::fromStdString(ss.str()));
    });

    connect(phaseGraph, &DisplaySignalWidget::displayValue, this, [=](int index)
    {
        double mag = *(magnitude.original.begin() + index);
        double pha = *(phase.original.begin() + index);
        double cospha = cos(pha);
        double sinpha = sin(pha);
        double real = cospha == 0 ? 0 : mag / cospha;
        double imag = sinpha == 0 ? 0 : mag / sinpha;
        std::stringstream ss;
        ss << "Value under cursor: " << real << std::showpos << imag << 'i';
        //statusBar->showMessage(QString::fromStdString(ss.str()));
        statusBarMessage->setText(QString::fromStdString(ss.str()));
    });

    connect(filteredGraph, &DisplaySignalWidget::displayValue, this, [=](int index)
    {
        std::stringstream ss;
        ss << "Value under cursor: " << *(filtered.original.begin() + index);
        //statusBar->showMessage(QString::fromStdString(ss.str()));
        statusBarMessage->setText(QString::fromStdString(ss.str()));
    });

    connect(editModeGraph, &DisplaySignalWidget::mouseLeave, this, [=]()
    {
        statusBar->clearMessage();
        statusBarMessage->setText(QStringLiteral(""));
    });
    connect(originalSignalGraph, &DisplaySignalWidget::mouseLeave, this, [=]()
    {
        statusBar->clearMessage();
    });
    connect(magnitudeGraph, &DisplaySignalWidget::mouseLeave, this, [=]()
    {
        statusBar->clearMessage();
    });
    connect(phaseGraph, &DisplaySignalWidget::mouseLeave, this, [=]()
    {
        statusBar->clearMessage();
    });

    connect(filteredGraph, &DisplaySignalWidget::mouseLeave, this, [=]()
    {
        statusBar->clearMessage();
    });


    connect(actionDefaultScale, &QAction::triggered, this, [=](bool)
    {
        originalSignalGraph->plotDefaultScale();
        cosGraph->plotDefaultScale();
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

    connect(actionRevertToOriginal, &QAction::triggered,this,[=](bool)
    {
        Signal::fourierTransform(original,magnitude,phase);

        original = *editModeHistory.pop();
        Signal::fourierTransform(editSignal,magnitude,phase);
        filtered = Signal(original);
        magnitudeGraph->displaySignal(&magnitude);
        phaseGraph->displaySignal(&phase);
        filteredGraph->displaySignal(&filtered);
        original = *editModeHistory.pop();
        Signal::fourierTransform(editSignal,magnitude,phase);
        filtered = Signal(original);
        magnitudeGraph->displaySignal(&magnitude);
        phaseGraph->displaySignal(&phase);
        filteredGraph->displaySignal(&filtered);
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

    enableGui(false);
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


    actionOpen = new QAction(menuFile);
    actionOpenPredefined = new QAction(menuFile);
    actionSave = new QAction(menuFile);
    actionExit = new QAction(menuFile);

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
    delete cosGraph;
    delete sinGraph;
    delete originalSignalGraph;
    delete filteredGraph;

    delete line;

    delete magPhaseTabWidget;
    delete frequencySpectrumLabel;
    delete centeringCheckBox;

    delete sinCosTabWidget;
    delete selectedFrequencyLabel;

    delete statusBarMessage;
    delete statusBar;
    delete mainToolBar;

    delete centralWidget;

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


void MainWindow::exitApplication()
{
    close();
}


void MainWindow::showAboutDialog()
{
    Translation* lang = localization.getCurrentLanguage()->getTranslationForWindow(QStringLiteral("AboutDialog"));
    AboutDialog dialog(this,lang, settings->value(QStringLiteral("appIcon")).toString());
    dialog.setModal(true);
    dialog.exec();
}


void MainWindow::showHelpDialog()
{
    Translation* lang = localization.getCurrentLanguage()->getTranslationForWindow(QStringLiteral("HelpDialog"));

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

        filtered = original;
        filteredGraph->displaySignal(&filtered);

        history.clear();
        actionUndo->setEnabled(false);
        enableGui(true);
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

    sinCosTabWidget->setTabText(0, QStringLiteral("Cosinus"));
    sinCosTabWidget->setTabText(1, QStringLiteral("Sinus"));

    frequencySpectrumLabel->setText(QStringLiteral("Frequency spectrum"));
    centeringCheckBox->setText(QStringLiteral("Centering"));

    selectedFrequencyLabel->setText(QStringLiteral("Selected frequency"));

    originalSignalLabel->setText(QStringLiteral("Original signal"));
    filteredSignalLabel->setText(QStringLiteral("Filtered signal"));

    magnitudeGraph->setDefaultTexts();
    phaseGraph->setDefaultTexts();
    cosGraph->setDefaultTexts();
    sinGraph->setDefaultTexts();
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

    sinCosTabWidget->setTabText(0, sinCosTabWidgetLanguage->getChildElementText(0));
    if(sinCosTabWidget->tabText(0).isEmpty()) sinCosTabWidget->setTabText(0, QStringLiteral("Cosinus"));

    sinCosTabWidget->setTabText(1, sinCosTabWidgetLanguage->getChildElementText(1));
    if(sinCosTabWidget->tabText(1).isEmpty()) sinCosTabWidget->setTabText(1, QStringLiteral("Sinus"));

    selectedFrequencyLabel->setText(language->getChildElementText(QStringLiteral("selectedFrequencyLabel")));
    if(selectedFrequencyLabel->text().isEmpty()) selectedFrequencyLabel->setText(QStringLiteral("Selected frequency"));

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
    zero.zeroSignal(length);

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


void MainWindow::resetAllGraphs(bool shadowPrevious)
{
    Signal::inverseFourierTransform(magnitude,phase,filtered);
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
        Signal::inverseFourierTransform(magnitude,phase,filtered);
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

            Translation* windowLanguage = localization.getCurrentLanguage()->getTranslationForWindow("FilterDialog");
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


void MainWindow::enableGui(bool val)
{
    magnitudeGraph->setInteractionsEnabled(val);
    phaseGraph->setInteractionsEnabled(val);
    originalSignalGraph->setInteractionsEnabled(val);
    filteredGraph->setInteractionsEnabled(val);
    actionUndo->setEnabled(val);
    actionSave->setEnabled(val);
    actionRevertToOriginal->setEnabled(val);
    actionFilterIdealLowPass->setEnabled(val);
    actionFilterIdealHighPass->setEnabled(val);
    actionFilterBandPass->setEnabled(val);
    actionFilterGaussianLowPass->setEnabled(val);
    actionFilterGaussianHighPass->setEnabled(val);
    actionFilterButterworthLowPass->setEnabled(val);
    actionFilterButterworthHighPass->setEnabled(val);
}
