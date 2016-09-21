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

    void displayFrequency(double frequency, double magnitudeVal, double phaseVal);

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

    bool displayingEnabled;

    bool modify;


};

#endif // FOURIERSPIRAL2_H
