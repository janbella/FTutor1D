#ifndef FOURIERSPIRAL2_H
#define FOURIERSPIRAL2_H

#include <QWidget>
#include <QOpenGLWidget>

#include <QMatrix4x4>

class FourierSpiral2 : public QWidget
{
    Q_OBJECT
public:
    explicit FourierSpiral2(QWidget *parent = 0);

    void displayFrequency(double frequency, double magnitudeVal, double phaseVal, int signalLength);

    void setNormalized(bool value);

    void clearFrequency();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    void drawAxes(QPainter& painter);

    void drawBackground(QPainter& painter);

    void drawTexts(QPainter& painter);


signals:

public slots:

private:
    QMatrix4x4 projection;

    int frequency;
    double magnitude;
    double phase;
    int signalLength;

    bool displayingEnabled;

    bool modify;

    std::vector<QVector4D> discretePoints;

    const float min_x = -1.0f;
    const float max_x = 7.0f;

    const float max_y = 2.0f;
    const float min_y = -2.0;

    const float min_z = -2.0f;
    const float max_z = 2.0f;

};

#endif // FOURIERSPIRAL2_H
