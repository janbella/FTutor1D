#include "fourierspiral2.h"

#include "glob_includes.h"

#include <QMatrix4x4>
#include <iostream>


// COORDINATE SYSTEMS:

// Internally, we use coordinate system (x, y, z)

//        y
//        ^
//        |
//        |
//        |/
//        |--------------------> x
//       /
//      /
//     /
//    v
//    z

// QPainter coordinate system

//    ---------> x
//   |
//   |
//   |
//   v
//   y


FourierSpiral2::FourierSpiral2(QWidget *parent) : QWidget(parent)
{
    QMatrix4x4 camera;

    QVector3D eye(10,-8,3);
    QVector3D center(-1.0f,-2.0f,-2.0f);
    QVector3D up(0.0f,1.0f,0.0f);

    camera.lookAt(eye, center, up);

    QMatrix4x4 ort;

    ort.setToIdentity();
    ort.ortho(-3.5,0,-1,1,0,1);
    //ort.viewport(-2,-1,4,2);

    QMatrix4x4 toImageSpace(this->width(), 0.0f,                  0.0f, this->width() / 2.0f + 100,
                            0.0f,                 this->height(), 0.0f, this->height() / 2.0f,
                            0.0f,                 0.0f,                  0.0f, 0.0f,
                            0.0f,                 0.0f,                  0.0f, 1.0f );

    projection = toImageSpace * ort * camera;
    frequency = 0;

}

void FourierSpiral2::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);

    drawBackground(painter);

    drawAxes(painter);

    drawTexts(painter);

    if(displayingEnabled)
    {
        if(frequency == 0)
        {
            painter.setPen(QColor::fromRgb(255,0,255));

            painter.drawLine((projection * QVector4D(0.0f,0.0f,-1.0f,1.0)).toPointF(),
                             (projection * QVector4D(2.0f*M_PI,0.0f,-1.0f,1.0)).toPointF());

            painter.setPen(QColor::fromRgb(255,0,128));

            painter.drawLine((projection * QVector4D(0.0f,0.0f,-2.5f,1.0)).toPointF(),
                             (projection * QVector4D(2.0f*M_PI,0.0f,-2.5f,1.0)).toPointF());


            painter.setPen(QColor::fromRgb(128,0,255));

            painter.drawLine((projection * QVector4D(0.0f,2.5f,-1.0f,1.0)).toPointF(),
                             (projection * QVector4D(2.0*M_PI,2.5f,-1.0f,1.0)).toPointF());

        }
        else
        {
            const float xLength = 2*M_PI;
            const float numberPoints = 15;

            size_t numberPointsTotal = fabs(frequency) * numberPoints;
            float spacing = xLength / numberPointsTotal;

            QVector<float> y;
            QVector<float> z;

            y.reserve(numberPointsTotal + 1);
            z.reserve(numberPointsTotal + 1);

            for(size_t i = 0; i <= numberPointsTotal; i++)
            {
                float newY =  -sinf(frequency * 2.0*M_PI/ numberPointsTotal * i );
                float newZ = -cosf(frequency * 2.0*M_PI/ numberPointsTotal * i );
                y.push_back(newY);
                z.push_back(newZ);
            }

            QVector<QPointF> points;

            for(size_t i = 0; i <= numberPointsTotal; i++)
            {
                points.push_back((projection * QVector4D(spacing * i, y[i], z[i], 1)).toPointF());
            }
            painter.setPen(QColor::fromRgb(255,0,255));
            painter.drawPolyline(&points[0],numberPointsTotal + 1);


            points.clear();

            for(size_t i = 0; i <= numberPointsTotal; i++)
            {
                points.push_back((projection * QVector4D(spacing * i, y[i], -2.5, 1)).toPointF());
            }
            painter.setPen(QColor::fromRgb(255,0,128));
            painter.drawPolyline(&points[0],numberPointsTotal + 1);


            points.clear();

            for(size_t i = 0; i <= numberPointsTotal; i++)
            {
                points.push_back((projection * QVector4D(spacing * i, 2.5, z[i], 1)).toPointF());
            }
            painter.setPen(QColor::fromRgb(128,0,255));
            painter.drawPolyline(&points[0],numberPointsTotal + 1);


            points.clear();

            float theta_base = 2.0f * M_PI / 20.0f;

            for(int ii = 0; ii < 20; ii++)
            {
                points.push_back((projection * QVector4D(-1, -cosf(theta_base * ii), sinf(theta_base * ii), 1.0f)).toPointF());
            }

            painter.setPen(QColor::fromRgb(255,128,128));
            painter.drawPolygon(&points[0],20);
        }
    }
}


void FourierSpiral2::drawAxes(QPainter& painter)
{
    //    QVector4D p(0.0f, 0.0f, 0.0f, 1.0f);
    //    QVector4D p1(1.0f, 0.0f, 0.0f, 1.0f);
    //    QVector4D p2(0.0f, -1.0f, 0.0f, 1.0f);
    //    QVector4D p3(0.0f, 0.0f, 1.0f, 1.0f);

    painter.save();

    painter.setPen(QColor::fromRgb(255,0,0));
    painter.drawLine((projection * QVector4D(-1,2.5,2.5,1.0)).toPointF(),(projection * QVector4D(7.2,2.5,2.5,1.0)).toPointF());

    //    painter.drawPolygon(arrow,3,Qt::WindingFill);

    QPainterPath path1;
    path1.moveTo ((projection * QVector4D(7.2,2.5,2.4,1.0)).toPointF());
    path1.lineTo ((projection * QVector4D(7.2,2.5,2.6,1.0)).toPointF());
    path1.lineTo ((projection * QVector4D(7.5,2.5,2.5,1.0)).toPointF());
    path1.lineTo ((projection * QVector4D(7.2,2.5,2.4,1.0)).toPointF());
    painter.setPen (Qt :: NoPen);
    painter.fillPath (path1, QBrush (QColor ("red")));

    painter.setPen(QColor("green"));
    painter.drawLine((projection * QVector4D(-1,-2.5,2.5,1.0)).toPointF(),(projection * QVector4D(-1,2.5,2.5,1.0)).toPointF());

    QPainterPath path2;
    path2.moveTo ((projection * QVector4D(-1,-2.5,2.4,1.0)).toPointF());
    path2.lineTo ((projection * QVector4D(-1,-2.8,2.5,1.0)).toPointF());
    path2.lineTo ((projection * QVector4D(-1,-2.5,2.6,1.0)).toPointF());
    path2.lineTo ((projection * QVector4D(-1,-2.5,2.4,1.0)).toPointF());
    painter.setPen (Qt :: NoPen);
    painter.fillPath (path2, QBrush (QColor ("green")));

    painter.setPen(QColor::fromRgb(0,0,255));
    painter.drawLine(QLine((projection * QVector4D(7,2.5, 2.5,  1.0)).toPoint(),(projection * QVector4D(7,2.5,-2.7,1.0)).toPoint()));

    QPainterPath path3;
    path3.moveTo ((projection * QVector4D(6.9,2.5,-2.7,1.0)).toPointF());
    path3.lineTo ((projection * QVector4D(7,2.5,-3.0,1.0)).toPointF());
    path3.lineTo ((projection * QVector4D(7.1,2.5,-2.7,1.0)).toPointF());
    path3.lineTo ((projection * QVector4D(6.9,2.5,-2.7,1.0)).toPointF());
    painter.setPen (Qt :: NoPen);
    painter.fillPath (path3, QBrush (QColor ("blue")));

    painter.restore();
}

void FourierSpiral2::drawBackground(QPainter& painter)
{
    painter.save();

    QBrush brush(QColor::fromRgb(0,0,0));
    QPen pen(brush,0.5,Qt::DashLine);
    painter.setPen(pen);
    //painter.setBrush(brush);

    int i = 0;
    QVector4D a(0.0f, 2.5f, 2.5f, 1.0f);
    QVector4D b(0.0f, 2.5f, -2.5f, 1.0f);

    // XZ
    for(i = -1; i < 8; i++ )
    {
        a.setX(i);
        b.setX(i);
        painter.drawLine((projection * a).toPointF(), (projection * b).toPointF());
    }

    a = QVector4D(-1.0f,2.5f,0.0f, 1.0f);
    b = QVector4D(7.0f,2.5f,0.0f, 1.0f);

    for(i = -2; i < 3; i++ )
    {
        a.setZ(i);
        b.setZ(i);
        painter.drawLine((projection * a).toPointF(), (projection * b).toPointF());
    }

    // XY

    a = QVector4D(0.0f,-2.5,-2.5, 1.0f);
    b = QVector4D(0.0f,2.5,-2.5,1.0f);

    for(i = -1; i < 8; i++ )
    {
        a.setX(i);
        b.setX(i);
        painter.drawLine((projection * a).toPointF(), (projection * b).toPointF());
    }

    a = QVector4D(-1.0f, 0.0f, -2.5f, 1.0f);
    b = QVector4D(7.0f, 0.0f, -2.5f, 1.0f);


    for(i = -2; i < 3; i++ )
    {
        a.setY(-i);
        b.setY(-i);
        painter.drawLine((projection * a).toPointF(), (projection * b).toPointF());
    }

    // YZ

    a = QVector4D(-1,0.0f,-2.5, 1.0f);
    b = QVector4D(-1,0.0f,2.5,1.0f);

    for(i = -2; i < 3; i++ )
    {
        a.setY(-i);
        b.setY(-i);
        painter.drawLine((projection * a).toPointF(), (projection * b).toPointF());
    }

    a = QVector4D(-1,-2.5,0.0f, 1.0f);
    b = QVector4D(-1,2.5,0.0f,1.0f);

    for(i = -2; i < 3; i++ )
    {
        a.setZ(i);
        b.setZ(i);
        painter.drawLine((projection * a).toPointF(), (projection * b).toPointF());
    }

    painter.restore();
}

void FourierSpiral2::drawTexts(QPainter& painter)
{
    painter.save();
    painter.setPen(QColor("black"));
    painter.drawText(0, 12, "Imag");
    painter.drawText(1, 50, "2");
    painter.drawText(1, 75, "1");
    painter.drawText(1, 100, "0");
    painter.drawText(-1, 125, "-1");
    painter.drawText(-1, 150, "-2");

    painter.drawText(30, 190, "0");
    painter.drawText(52, 201, "1");
    painter.drawText(74, 212, "2");
    painter.drawText(96, 223, "3");
    painter.drawText(118, 235, "4");
    painter.drawText(140, 247, "5");
    painter.drawText(162, 258, "6");
    painter.drawText(184, 268, "7");
    painter.drawText(52, 250, "T");

    painter.drawText(230, 270, "-2");
    painter.drawText(281, 265, "-1");
    painter.drawText(334, 260, "0");
    painter.drawText(387, 255, "1");
    painter.drawText(440, 251, "2");
    painter.drawText(435, 270, "Real");

    painter.restore();
}


void FourierSpiral2::displayFrequency(double frequency, double magnitudeVal, double phaseVal)
{
    this->frequency = frequency;
    this->magnitude = magnitudeVal;
    this->phase = phaseVal;

    displayingEnabled = true;

    update();
}

void FourierSpiral2::setNormalized(bool value)
{
    modify = !value;
}

void FourierSpiral2::clearFrequency()
{
    displayingEnabled = false;
}
