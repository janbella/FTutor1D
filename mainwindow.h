#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "QCustomPlot/qcustomplot.h"
#include "signal.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:

    /**
     * @brief keyPressEvent responce to pressing a key on the keyboard
     * @param event
     */
//    void keyPressEvent(QKeyEvent *event);

//    void mousePressEvent(QMouseEvent* event);

//    void mouseReleaseEvent(QMouseEvent* event);

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
     * @brief showOfficialWebsite action that opens application official website in the web browser
     */
    void showOfficialWebsite();

    /**
     * @brief openImageDialog action that shows open custom image dialog
     */
    void openFileDialog();

private:
    Ui::MainWindow *ui;

    Signal original;
    Signal magnitude;
    Signal phase;
    Signal filtered;
    Signal frequency;

    void horizontalScrollBarChanged(QCustomPlot* plot, int value);
    void verticalScrollBarChanged(QCustomPlot* plot, int value);
    void xAxisChanged(QCustomPlot* plot, QScrollBar* scrollbar, Signal& signal, QCPRange& range);
    void yAxisChanged(QCustomPlot* plot, QScrollBar* scrollbar, Signal& signal, QCPRange& range);

    //void selectionChanged(QCustomPlot* plot);
    void plotMousePress(QCustomPlot* plot);
    void plotMouseWheel(QCustomPlot* plot);

    void displaySignal(QCustomPlot* plot, Signal& signal);

//    void displayOriginalSignal();
//    void displayMagnitudeSignal();
//    void displayPhaseSignal();

private slots:
  void magnitudeHorzScrollBarChanged(int value);
  void magnitudeVertScrollBarChanged(int value);
  void magnitudePlotxAxisChanged(QCPRange range);
  void magnitudePlotyAxisChanged(QCPRange range);
  void magnitudePlotMouseWheel();
  void magnitudePlotMousePress();

  void phaseHorzScrollBarChanged(int value);
  void phaseVertScrollBarChanged(int value);
  void phasePlotxAxisChanged(QCPRange range);
  void phasePlotyAxisChanged(QCPRange range);
  void phasePlotMouseWheel();
  void phasePlotMousePress();

  void originalHorzScrollBarChanged(int value);
  void originalVertScrollBarChanged(int value);
  void originalPlotxAxisChanged(QCPRange range);
  void originalPlotyAxisChanged(QCPRange range);
  void originalPlotMouseWheel();
  void originalPlotMousePress();

  void filteredHorzScrollBarChanged(int value);
  void filteredVertScrollBarChanged(int value);
  void filteredPlotxAxisChanged(QCPRange range);
  void filteredPlotyAxisChanged(QCPRange range);
  void filteredPlotMouseWheel();
  void filteredPlotMousePress();

  void frequencyHorzScrollBarChanged(int value);
  void frequencyVertScrollBarChanged(int value);
  void frequencyPlotxAxisChanged(QCPRange range);
  void frequencyPlotyAxisChanged(QCPRange range);
  void frequencyPlotMouseWheel();
  void frequencyPlotMousePress();

  void defaultSize();

  void contextMenuRequest(QPoint pos);

};

#endif // MAINWINDOW_H
