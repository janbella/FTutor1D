#include "fourierspiralwidget.h"

#include <QVector3D>
#include <QPainter>
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void FourierSpiralWidget::resizeGL(int w, int h)
{
    // there shall be no resizing.
}

void DrawCircle(float cx, float cy, int num_segments)
{
    glBegin(GL_LINE_LOOP);
    for(int ii = 0; ii < num_segments; ii++)
    {
        float theta = 2.0f * M_PI * float(ii) / float(num_segments);//get the current angle

        float x = cosf(theta);//calculate the x component
        float y = sinf(theta);//calculate the y component

        glVertex3f(-1,x + cx, y + cy);//output vertex

    }
    glEnd();
}

void FourierSpiralWidget::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    glOrtho(-3,6,-6,3,-3,40);
    //glViewport(10,10,10,10);
    glRotatef(-90, 0,1,0);

    glTranslatef(-20,0,0);

    glRotatef(25, 0,1,0);

    glTranslatef(0,-3,0);
    glRotatef(25, 25,0,-65);

    glTranslatef(0,3,0);

    glRotatef(2, 1,0,0);

    glMatrixMode(GL_MODELVIEW);
    glPointSize(10);

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


   displayFrequency(0,0,0,false);

   DrawCircle(0,0,20);

   QPainter painter(this);
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
    double xLength = 2*M_PI;
    double numberPoints = 100;

    double numberPointsTotal = frequency == 0 ? numberPoints : fabs(frequency) * numberPoints;
    double spacing = xLength / numberPointsTotal;

    if(frequency != 0)
    {
        glBegin(GL_LINE_STRIP);
        glColor3f(1,0,1);
        glLineWidth(1);

        for(int i = 0; i <= numberPointsTotal; i++)
        {
            double newY = sin(frequency * 2.0*M_PI/ numberPointsTotal * i );
            double newZ = -cos(frequency * 2.0*M_PI/ numberPointsTotal * i );
            glVertex3d(spacing*(i),newY,newZ);
        }
        glEnd();

        glBegin(GL_LINE_STRIP);
        glColor3f(1,0,0.5);

        for(int i = 0; i <= numberPointsTotal; i++)
        {
            double newY = sin(frequency * 2.0*M_PI/ numberPointsTotal * i );
            glVertex3d(spacing*(i),newY,-2.5);
        }
        glEnd();

        glBegin(GL_LINE_STRIP);
        glColor3f(0.5,0,1);

        for(int i = 0; i <= numberPointsTotal; i++)
        {
            double newZ = -cos(frequency * 2.0*M_PI/ numberPointsTotal * i );
            glVertex3d(spacing*(i),-2.5,newZ);
        }
        glEnd();
    }
    else
    {
        glBegin(GL_LINES);
        glColor3f(1,0,1);

        glVertex3d(0,0,1);
        glVertex3d(spacing*(numberPointsTotal),0,1);

        glVertex3d(0,0,-2.5);
        glVertex3d(spacing*(numberPointsTotal),0,-2.5);

        glVertex3d(0,-2.5,0);
        glVertex3d(spacing*(numberPointsTotal),-2.5,0);

        glEnd();
    }
}

