#ifndef FOURIERSPIRALWIDGET_H
#define FOURIERSPIRALWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>


class FourierSpiralWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit FourierSpiralWidget(QWidget *parent = 0);

    virtual ~FourierSpiralWidget();

    void displayFrequency(double frequency, double magnitudeVal, double phaseVal, bool modify);
signals:

public slots:

private:
     QOpenGLShaderProgram* m_program;

    void initializeGL() Q_DECL_OVERRIDE;

    void resizeGL(int w, int h) Q_DECL_OVERRIDE;

    void paintGL() Q_DECL_OVERRIDE;

public:
    int frequency;
    int magnitude;
    int phase;
    bool applyCoefs;
};

#endif // FOURIERSPIRALWIDGET_H
