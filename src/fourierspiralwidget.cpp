#include "fourierspiralwidget.h"

#include <QVector3D>
#include <QMatrix4x4>

FourierSpiralWidget::FourierSpiralWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    setFixedSize(470,270);
    setCursor(QCursor(Qt::CrossCursor));

}

FourierSpiralWidget::~FourierSpiralWidget()
{

}

void FourierSpiralWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(1, 1, 1, 1);
}

void FourierSpiralWidget::resizeGL(int w, int h)
{
    // there shall be no resizing.
}

void FourierSpiralWidget::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);


    glLoadIdentity();

    glOrtho(-10,10,-10,10,0,10);

    glRotatef(45, 1,0,0);
    glRotatef(-45, 0,1,0);

    glTranslatef(-0.5,-0.5,-0.5);


    glMatrixMode(GL_MODELVIEW);

    glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex3f(0,0,0);
        glVertex3f(1,0,0);

        glColor3f(0,1,0);
        glVertex3f(0,0,0);
        glVertex3f(0,1,0);

        glColor3f(0,0,1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,1);

    glEnd();

}


void FourierSpiralWidget::displayFrequency(double x, double y, bool scaleWithY)
{
//    int length = original.original_length();
//    Signal impulse = original.makeImpulse(x, sqrt(length));  // sqrt(length) so that IFT would be in range -1 - 1
//    Signal zero;
//    zero.zeroSignal(length);

//    Signal::inverseFourierTransform(impulse,zero,cosinusFrequency);
//    Signal::inverseFourierTransform(impulse,zero,sinusFrequency,false);

//    Signal sinusWave;
//    Signal cosinusWave;

//    int frequency = (x <= length - x ? x : -(length - x));

//    sinusWave.sinusWave(frequency, length, 20);
//    cosinusWave.cosinusWave(frequency, length, 20);
//    sinGraph->displayFrequency(&sinusFrequency,&sinusWave);
//    cosGraph->displayFrequency(&cosinusFrequency,&cosinusWave);
}

//void drawAxes()
//{
//    glBegin(GL_LINES);
//        glColor3b(255,0,0);
//        glVertex3i(0,0,0);
//        glVertex3i(1,0,0);
//        glColor3b(0,255,0);
//        glVertex3i(0,0,0);
//        glVertex3i(0,1,0);
//        glColor3b(0,0,255);
//        glVertex3i(0,0,0);
//        glVertex3i(0,0,1);
//    glEnd();
//}
