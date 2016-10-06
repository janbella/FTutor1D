#ifndef FOURIERSPIRALWIDGET_H
#define FOURIERSPIRALWIDGET_H

/**
 * @file fourierspiralwidget.h
 * @author Ján Bella <xbella1@fi.muni.cz>
 */

#include <QWidget>
#include <QMatrix4x4>

/**
 * @brief The FourierSpiralWidget class
 */
class FourierSpiralWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief FourierSpiralWidget constructor
     * @param parent parent object
     */
    explicit FourierSpiralWidget(QWidget *parent = 0);

    /**
     * @brief displayFrequency the main callback, sets the variables for proper display
     * @param frequency what frequency is to be displayed
     * @param magnitudeVal the magnitude of the selected frequency
     * @param phaseVal the phase of the selected frequency
     * @param signalLength length of the signal for which is the basis function to be displayed
     */
    void displayFrequency(double frequency, double magnitudeVal, double phaseVal, double maxMagnitudeVal, int signalLength);

    /**
     * @brief setNormalized sets, whether or not to scale and shift the basis function
     * @param value true means yes, false stands for no
     */
    void setNormalized(bool value);

    /**
     * @brief clearFrequency sets displaying enabled to false. Repaint is called.
     */
    void clearFrequency();

    /**
     * @brief setMagnitudeAndPhase updates magnitude and phase values. If modify is true, repaint is called.
     * @param mag current magnitude value
     * @param pha current phase value
     */
    void setMagnitudeAndPhase(double mag, double pha);

    /**
     * @brief newSignal changes length of currently loaded signal. Used, when no basis function is displayed,
     * but new signal is loaded in the rest of the application and axes of this widget has to be rescaled. Calls repaint.
     * @param length length of the new signal.
     */
    void newSignal(int length);

protected:

    /**
     * The main painting callback
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    /**
     * @brief drawAxes draws x, y, z axes
     * @param painter painter to use for drawing
     */
    void drawAxes(QPainter& painter);

    /**
     * @brief drawBackground draws background
     * @param painter painter to use for drawing
     */
    void drawBackground(QPainter& painter);

    /**
     * @brief drawTexts draws texts - names of the axes.
     * @param painter painter to use for drawing
     */
    void drawTexts(QPainter& painter);

private:

    // matrix to use to transform the space coordinates to widget coordinates
    QMatrix4x4 projection;

    // what frequency is displayed
    int frequency;
    double magnitude;
    double phase;
    size_t signalLength;

    // maximal magnitude is used to setup the axes scale properly.
    double maxMagnitude;

    bool displayingEnabled;
    bool modify;


    // ugly, should be rewritten
    // boundaries I originally accounted with when designing the projection.
    // now everything must be scaled in between.
    const float min_x = -1.0f;
    const float max_x = 7.0f;

    const float max_y = 2.0f;
    const float min_y = -2.0;

    const float min_z = -2.0f;
    const float max_z = 2.0f;

};

#endif // FOURIERSPIRALWIDGET_H
