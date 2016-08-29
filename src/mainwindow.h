#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "glob_includes.h"
#include "displaysignalwidget.h"

#include "qcustomplot/qcustomplot.h"
#include "signal.h"
#include "localization.h"
#include "filterdialog.h"

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
     * @brief exitApplication action that exits application
     */
    void exitApplication();

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

    void recordCurrentState();


private:
    // application menu
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
    QAction* actionHideLinesAll;
    QAction* actionAllowAutoScaling;
    QAction* actionForbidAutoScaling;

    QAction* actionViewHelp;
    QAction* actionOfficialWebsite;
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

    DisplaySignalWidget* cosGraph;
    DisplaySignalWidget* sinGraph;

    DisplaySignalWidget* originalSignalGraph;
    DisplaySignalWidget* filteredGraph;

    DisplaySignalWidget* editModeGraph;
    QPushButton* editModeButton;
    QWidget* editModeContainer;

    QWidget* centralWidget;
    QFrame* line;

    QTabWidget* magPhaseTabWidget;
    QLabel* frequencySpectrumLabel;
    QCheckBox* centeringCheckBox;

    QTabWidget* sinCosTabWidget;
    QLabel* selectedFrequencyLabel;

    QLabel* originalSignalLabel;
    QLabel* filteredSignalLabel;

    QStatusBar* statusBar;
    QToolBar* mainToolBar;

    Signal original;
    Signal magnitude;
    Signal phase;
    Signal filtered;
    Signal cosinusFrequency;
    Signal sinusFrequency;
    Signal editSignal;

    QSettings* settings;

    Localization localization;

    QStack<QPair<Signal*, Signal*> > history;

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

    void resetAllGraphs();

    void noSignalWarning();

    void connectFilterAction(QAction* action, FilterType type);



private slots:

    void displayFrequency(double x, double y);

    void undo();

};

#endif // MAINWINDOW_H