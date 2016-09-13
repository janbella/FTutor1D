#include "fourierspiralwidget.h"

#include <QVector3D>
#include <QPainter>
#include <QMatrix4x4>

FourierSpiralWidget::FourierSpiralWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    applyCoefs = false;

    setFixedSize(470,270);
    setCursor(QCursor(Qt::CrossCursor));

    painter = nullptr;



}

FourierSpiralWidget::~FourierSpiralWidget()
{

}

void FourierSpiralWidget::initializeGL()
{
    initializeOpenGLFunctions();



    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // setup the "camera"
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-3,6,-6,3,-3,40);
    glRotatef(-90, 0,1,0);
    glTranslatef(-20,0,0);
    glRotatef(25, 0,1,0);
    glTranslatef(0,-3,0);
    glRotatef(25, 25,0,-65);
    glTranslatef(0,3,0);
    glRotatef(2, 1,0,0);

    displayListBackground = glGenLists(1);
    displayListAxes = glGenLists(1);
    displayListSignals = glGenLists(1);

    glNewList(displayListBackground, GL_COMPILE);
        glLineStipple(1, 0xAAAA);
        glEnable(GL_LINE_STIPPLE);

        glBegin(GL_LINES);
            glColor3f(0,0,0);
            for(int i = -1; i < 8; i++ )
            {
                glVertex3f(i,-2.5,-2.5);
                glVertex3f(i,-2.5,2.5);
            }

            for(int i = -2; i < 3; i++ )
            {
                glVertex3f(-1,-2.5,i);
                glVertex3f(7,-2.5,i);
            }

            for(int i = -1; i < 8; i++ )
            {
                glVertex3f(i,-2.5,-2.5);
                glVertex3f(i,2.5,-2.5);
            }

            for(int i = -2; i < 3; i++ )
            {
                glVertex3f(-1,i,-2.5);
                glVertex3f(7,i,-2.5);
            }

            for(int i = -2; i < 3; i++ )
            {
                glVertex3f(-1,i,-2.5);
                glVertex3f(-1,i,2.5);
            }

            for(int i = -2; i < 3; i++ )
            {
                glVertex3f(-1,-2.5,i);
                glVertex3f(-1,2.5,i);
            }
        glEnd();
    glDisable(GL_LINE_STIPPLE);
    glEndList();


    glNewList(displayListAxes, GL_COMPILE);
        glBegin(GL_LINES);
            glColor3f(1,0,0);   // red is x
            glVertex3f(-1,-2.5,2);
            glVertex3f(7.2,-2.5,2);
        glEnd();

         glBegin(GL_TRIANGLES);
            glVertex3f(7.2,-2.5,1.9);
            glVertex3f(7.2,-2.5,2.1);
            glVertex3f(7.5,-2.5,2);
         glEnd();

        glBegin(GL_LINES);
            glColor3f(0,1,0);   // green is y
            glVertex3f(-1,-2.5,2);
            glVertex3f(-1,2.5,2);
        glEnd();

        glBegin(GL_TRIANGLES);
           glVertex3f(-1,2.5,1.9);
           glVertex3f(-1,2.8,2);
           glVertex3f(-1,2.5,2.1);
        glEnd();

        glBegin(GL_LINES);
            glColor3f(0,0,1);   // z is blue
            glVertex3f(7,-2.5,2);
            glVertex3f(7,-2.5,-2.5);
        glEnd();

        glBegin(GL_TRIANGLES);
           glVertex3f(6.9,-2.5,-2.5);
           glVertex3f(7,-2.5,-2.8);
           glVertex3f(7.1,-2.5,-2.5);
        glEnd();
    glEndList();
}

void FourierSpiralWidget::resizeGL(int w, int h)
{
    // there shall be no resizing.
}

void FourierSpiralWidget::paintGL()
{
    QPainter painter(this);
  //  painter.beginNativePainting();

    glMatrixMode(GL_MODELVIEW);
    glPointSize(10);

    glCallList(displayListBackground);
    glCallList(displayListAxes);
    glCallList(displayListSignals);

//    painter.endNativePainting();

    painter.drawText(8, 20, "Imag");
    painter.drawText(8, 60, "2");
    painter.drawText(8, 85, "1");
    painter.drawText(9, 110, "0");
    painter.drawText(6, 135, "-1");
    painter.drawText(6, 160, "-2");

    painter.drawText(30, 190, "0");
    painter.drawText(52, 201, "1");
    painter.drawText(74, 212, "2");
    painter.drawText(96, 223, "3");
    painter.drawText(118, 235, "4");
    painter.drawText(140, 247, "5");
    painter.drawText(162, 258, "6");
    painter.drawText(184, 268, "7");
    painter.drawText(52, 250, "Time");

    //painter.drawText(200, 160, "-2");
    painter.drawText(260, 268, "-1");
    painter.drawText(310, 262, "0");
    painter.drawText(360, 257, "1");
    painter.drawText(410, 251, "2");
    painter.drawText(435, 251, "Real");

    painter.end();
}


void FourierSpiralWidget::displayFrequency(double frequency, double magnitudeVal, double phaseVal, bool modify)
{
    if(!modify)
    {
        magnitudeVal = 1;
        phaseVal = 0;
    }

    if(frequency == 0)
    {
        glNewList(displayListSignals,GL_COMPILE);
            glBegin(GL_LINES);
            glColor3f(1.0f,0.0f,1.0f);

            glVertex3f(0.0f,0.0f,-1.0f * magnitudeVal);
            glVertex3f(2.0f*M_PI,0.0f,-1.0f * magnitudeVal);

            glColor3f(1.0f,0.0f,0.5f);
            glVertex3f(0.0f,0.0f,-2.5f);
            glVertex3f(2.0f*M_PI,0.0f,-2.5f);

            glColor3f(0.5f,0.0f,1.0f);
            glVertex3f(0.0f,-2.5f,-1.0f);
            glVertex3f(2.0*M_PI,-2.5f,-1.0f);

            glEnd();
        glEndList();
    }
    else
    {
        const float xLength = 2*M_PI;
        const float numberPoints = 15;

        size_t numberPointsTotal = fabs(frequency) * numberPoints;
        float spacing = xLength / numberPointsTotal;

        std::vector<float> y;
        std::vector<float> z;

        y.reserve(numberPointsTotal + 1);
        z.reserve(numberPointsTotal + 1);

        for(size_t i = 0; i <= numberPointsTotal; i++)
        {
            float newY =  sinf(frequency * 2.0*M_PI/ numberPointsTotal * i ) * magnitudeVal;
            float newZ = -cosf(frequency * 2.0*M_PI/ numberPointsTotal * i ) * magnitudeVal;
            y.push_back(newY);
            z.push_back(newZ);
        }

        glNewList(displayListSignals,GL_COMPILE);

        glBegin(GL_LINE_STRIP);
            glColor3f(1.0f,0.0f,1.0f);
            glLineWidth(1.0f);

            for(size_t i = 0; i <= numberPointsTotal; i++)
            {
                glVertex3f(spacing*(i),y[i],z[i]);
            }
        glEnd();

        glBegin(GL_LINE_STRIP);
        glColor3f(1.0f,0.0f,0.5f);

        for(size_t i = 0; i <= numberPointsTotal; i++)
        {
            glVertex3f(spacing*(i),y[i],-2.5);
        }
        glEnd();

        glBegin(GL_LINE_STRIP);
        glColor3f(0.5,0.0f,1.0f);

        for(size_t i = 0; i <= numberPointsTotal; i++)
        {
            glVertex3f(spacing*(i),-2.5,z[i]);
        }
        glEnd();


        glBegin(GL_LINE_LOOP);
        glColor3f(1.0,0.5f,0.5f);

        float theta_base = 2.0f * M_PI / 20.0f;
        for(int ii = 0; ii < 20; ii++)
        {
            glVertex3f(-1, cosf(theta_base * ii), sinf(theta_base * ii));//output vertex
        }
        glEnd();
        glEndList();
    }
    update();
}

void FourierSpiralWidget::clearFrequency()
{
    glNewList(displayListSignals, GL_COMPILE);
    glEndList();
    update();
}

