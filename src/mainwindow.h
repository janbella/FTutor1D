#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/**
 * @file mainwindow.h
 * @author Ján Bella <xbella1@fi.muni.cz>
 */


#include <array>

#include "glob_includes.h"
#include "displaysignalwidget.h"

#include "qcustomplot/qcustomplot.h"
#include "signal.h"
#include "localization.h"
#include "filterdialog.h"
#include "fourierspiralwidget.h"
#include "fourierspiral2.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

protected:

    /**
     * @brief keyPressEvent responce to pressing a key on the keyboard
     * @param event
     */
    //    void keyPressEvent(QKeyEvent *event);

public slots:

    /**
     * @brief showAboutDialog action that shows about application dialog
     */
    void showAboutDialog();

    /**
     * @brief showHelpDialog action that shows application help dialog
     */
    void showHelpDialog();

    /**
     * @brief openPredefinedSignalsDialog
     */
    void openPredefinedSignalsDialog();

    /**
     * @brief recordCurrentState
     */
    void recordCurrentState();


private:
    // application menu
    QAction* actionNew;
    QAction* actionOpen;
    QAction* actionOpenPredefined;
    QAction* actionSave;
    QAction* actionExit;

    QAction* actionUndo;
    QAction* actionRevertToOriginal;

    QAction* actionFilterIdealLowPass;
    QAction* actionFilterIdealHighPass;
    QAction* actionFilterBandPass;
    QAction* actionFilterGaussianLowPass;
    QAction* actionFilterGaussianHighPass;
    QAction* actionFilterButterworthLowPass;
    QAction* actionFilterButterworthHighPass;

    QAction* actionDefaultScale;
    QAction* actionDisplayLinesAll;
    QAction* actionAutoScalingAll;

    QAction* actionViewHelp;
    QAction* actionAbout;

    QMenuBar* menuBar;
    QMenu* menuFile;
    QMenu* menuEdit;
    QMenu* menuFilters;
    QMenu* menuView;
    QMenu* menuLanguage;
    QMenu* menuHelp;

    DisplaySignalWidget* magnitudeGraph;
    DisplaySignalWidget* phaseGraph;

    FourierSpiralWidget* fourierSpiralGraph;
    FourierSpiral2* fourierSpiral;

    DisplaySignalWidget* originalSignalGraph;
    DisplaySignalWidget* filteredGraph;

    DisplaySignalWidget* editModeGraph;
    QPushButton* editModeFinishButton;
    QPushButton* editModeCancelButton;
    QWidget* editModeContainer;

    QWidget* centralWidget;
    QFrame* line;

    QTabWidget* magPhaseTabWidget;
    QLabel* frequencySpectrumLabel;
    QCheckBox* centeringCheckBox;

    QLabel* selectedFrequencyLabel;
    QCheckBox* normalizedCheckBox;


    QLabel* originalSignalLabel;
    QLabel* filteredSignalLabel;

    QStatusBar* statusBar;
    QToolBar* mainToolBar;

    QLabel* statusBarMessage;

    Signal original;
    Signal magnitude;
    Signal phase;
    Signal filtered;
    Signal cosinusFrequency;
    Signal sinusFrequency;
    Signal editSignal;

    Signal prevOriginal;

    QSettings* settings;

    Localization localization;

    QStack<QPair<Signal*, Signal*> > history;

    QStack<Signal*> editModeHistory;

    void populateLanguagesMenu();

    void setLanguage(QString name);

    void createMenu();

    /**
     * @brief setDefaultTexts sets defaults values to each text or title or label in the window.
     */
    void setDefaultTexts();

    /**
     * @brief setLocalizedTexts sets text, title or label values according to given Translation object`
     * @param language Translation object used to set texts
     */
    void setLocalizedTexts(const Translation* language);

    void loadSignal(std::string path);

    void saveFilteredSignal(std::string path);

    void updateFilteredSignalPlot();

    void resetAllGraphs(bool shadowPrevious);

    void noSignalWarning();

    void connectFilterAction(QAction* action, FilterType type);

    void needUpdateMagPhaseFiltered();

    void recordCurrentEditModeState();

    void enableFilters(bool val);

    void newSignalCreated();

    void newSignalDiscarded();

    void openEditMode();

    void showFrequencyInStatusBar(int x, int index);

    void revertToOriginal();

    void emptyHistoryStacks();


private slots:

    void undo();

};

#endif // MAINWINDOW_H
