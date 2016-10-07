#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/**
 * @file mainwindow.h
 * @author Ján Bella <xbella1@fi.muni.cz>
 */

#include "displaysignalwidget.h"
#include "qcustomplot/qcustomplot.h"
#include "signal.h"
#include "localization.h"
#include "filterdialog.h"
#include "fourierspiralwidget.h"

namespace FT1D
{
/**
 * @brief The MainWindow class the main window of the application and the most of the app logic
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief MainWindow constructor
     * @param parent parent object, can be NULL
     */
    explicit MainWindow(QWidget* parent = 0);

    /**
     * Destructor
     */
    ~MainWindow();

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

    FourierSpiralWidget* fourierSpiral;

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

    // signals
    Signal original;
    Signal magnitude;
    Signal phase;
    Signal filtered;
    Signal editSignal;

    Signal prevOriginal;

    QSettings* settings;

    Localizations localization;

    QStack<QPair<Signal*, Signal*> > history;

    QStack<Signal*> editModeHistory;

    /**
     * @brief populateLanguagesMenu create Localizations, read the directory given by config and fill it
     */
    void populateLanguagesMenu();

    /**
     * @brief setLanguage set the current language
     * @param name name of the language to set
     */
    void setLanguage(const QString& name);

    /**
     * @brief createMenu adds all actions to the application menu
     */
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

    /**
     * @brief loadSignal read the file at @a path, if successful, the signal is displayed
     * @param path path to the file to load
     */
    void loadSignal(std::string path);

    /**
     * @brief saveFilteredSignal saves the signal visible in filtered graph to the file
     * @param path
     */
    void saveFilteredSignal(std::string path);

    /**
     * @brief updateFilteredSignalPlot updates the filtered graph
     */
    void updateFilteredSignalPlot();

    /**
     * @brief resetAllGraphs all graphs call display signal again.
     * @param shadowPrevious true, if the previous signal should be shadowed.
     */
    void resetAllGraphs(bool shadowPrevious);

    /**
     * @brief connectFilterAction
     * @param action
     * @param type
     */
    void connectFilterAction(QAction* action, FilterType type);

    /**
     * @brief needUpdateMagPhaseFiltered refreshes magnitude, phase and filtered graphs
     */
    void needUpdateMagPhaseFiltered();

    /**
     * @brief recordCurrentEditModeState save the signals in the history
     */
    void recordCurrentEditModeState();

    /**
     * @brief enableFilters enables or disables filter actions
     * @param val true for enabling
     */
    void enableFilters(bool val);

    /**
     * @brief newSignalCreated handles situation when we have a new signal made in edit mode
     */
    void newSignalCreated();

    /**
     * @brief newSignalDiscarded handles the situation when edit mode has been cancelled
     */
    void newSignalDiscarded();

    /**
     * @brief openEditMode handles opening of the edit mode
     * @param toEdit (output) a signal, which will be used as edit signal
     */
    void openEditMode(Signal& toEdit);

    /**
     * @brief showFrequencyInStatusBar show frequency in the status bar and update fourier spiral
     * @param x coordinate
     * @param index index in the original part
     */
    void showFrequencyInStatusBar(int x, int index);

    /**
     * @brief revertToOriginal stash all changes
     */
    void revertToOriginal();

    /**
     * @brief emptyHistoryStacks trash the history, keep the bin empty
     */
    void emptyHistoryStacks();


private slots:

    /**
     * @brief undo go one step back, based on history
     */
    void undo();

};
}
#endif // MAINWINDOW_H
