#include "fourierspiral2.h"

#include "glob_includes.h"

#include <QMatrix4x4>
#include <iostream>
#include "signal.h"

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
    magnitude = 0;
    phase = 0;
    signalLength = 0;
    modify = false;

    this->setStyleSheet("FourierSpiral2 { background-color: green; }");

}

void FourierSpiral2::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);

    painter.setBackground(QBrush(QColor::fromRgb(255,255,255)));

    drawBackground(painter);

    drawAxes(painter);

    drawTexts(painter);

    const float x_step = (max_x - min_x) / (signalLength + 1);

    if(displayingEnabled)
    {
        if(frequency == 0)
        {
            painter.setPen(QColor::fromRgb(255,0,255));

            painter.drawLine((projection * QVector4D(min_x + x_step,0.0f,-1.0f,1.0)).toPointF(),
                             (projection * QVector4D(max_x - x_step,0.0f,-1.0f,1.0)).toPointF());

            painter.setPen(QColor::fromRgb(255,0,128));

            painter.drawLine((projection * QVector4D(min_x + x_step,0.0f,-2.5f,1.0)).toPointF(),
                             (projection * QVector4D(max_x - x_step,0.0f,-2.5f,1.0)).toPointF());


            painter.setPen(QColor::fromRgb(128,0,255));

            painter.drawLine((projection * QVector4D(min_x + x_step,2.5f,-1.0f,1.0)).toPointF(),
                             (projection * QVector4D(max_x - x_step,2.5f,-1.0f,1.0)).toPointF());

        }
        else
        {
            const float xLength = (max_x - min_x) - x_step;
            const float numberPointsPerPeriod = 15;

            size_t numberPointsTotal = fabs(frequency) * numberPointsPerPeriod;
            float spacing = xLength / numberPointsTotal;

            QVector<float> y;
            QVector<float> z;

            y.reserve(numberPointsTotal + 1);
            z.reserve(numberPointsTotal + 1);

            for(size_t i = 0; i <= numberPointsTotal; i++)
            {
                float newY = -sinf(frequency * 2.0*M_PI/ (numberPointsTotal) * i );
                float newZ = -cosf(frequency * 2.0*M_PI/ (numberPointsTotal) * i );
                y.push_back(newY);
                z.push_back(newZ);
            }

            QVector<QPointF> points;

            for(size_t i = 0; i <= numberPointsTotal; i++)
            {
                points.push_back((projection * QVector4D(min_x + x_step + spacing * i, y[i], z[i], 1)).toPointF());
            }

            painter.setPen(QPen(QColor::fromRgb(255,0,255),2));
            painter.drawPolyline(&points[0],numberPointsTotal + 1);


            points.clear();

            for(size_t i = 0; i <= numberPointsTotal; i++)
            {
                points.push_back((projection * QVector4D(min_x + x_step + spacing * i, y[i], -2.5, 1)).toPointF());
            }
            painter.setPen(QColor::fromRgb(255,0,128));
            painter.drawPolyline(&points[0],numberPointsTotal + 1);


            points.clear();

            for(size_t i = 0; i <= numberPointsTotal; i++)
            {
                points.push_back((projection * QVector4D(min_x + x_step + spacing * i, 2.5, z[i], 1)).toPointF());
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

        painter.setBrush(Qt::red);

        for(int i = 0; i < signalLength; i++)
        {
            painter.drawEllipse((projection * discretePoints[i]).toPointF(), 2,2);
        }

        painter.setBrush(Qt::yellow);

        for(int i = 0; i < signalLength; i++)
        {
            painter.drawEllipse((projection * QVector4D(min_x + (i + 1)*x_step, discretePoints[i].y(), min_z - 0.5f, 1.0)).toPointF(), 2,2);
        }

        for(int i = 0; i < signalLength; i++)
        {
            painter.drawEllipse((projection * QVector4D(min_x + (i + 1)*x_step, max_y + 0.5f, discretePoints[i].z(), 1.0)).toPointF(), 2,2);
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

    // Ugly thing: because setting the projection and viewport correctly is somehow difficult
    // To reflect the length of the signal, use the interpolation within the set interval

    const float mag_max = (abs(magnitude) < 2.0f ||  !modify)? 2.0f : ceil(abs(magnitude));
    const float mag_min = -mag_max;

    const float x_step = (max_x - min_x) / (signalLength + 1);
    const float mag_step = (max_y - min_y) / (mag_max - mag_min);


    QVector4D a(0.0f, max_y + 0.5f, max_z + 0.5f, 1.0f);
    QVector4D b(0.0f, max_y + 0.5f, min_z - 0.5f, 1.0f);

    float i = min_x;

    while(i <= max_x)
    {
        a.setX(i);
        b.setX(i);
        painter.drawLine((projection * a).toPointF(), (projection * b).toPointF());
        i += x_step;
    }
    // XZ

    a = QVector4D(min_x,max_y + 0.5f, 0.0f, 1.0f);
    b = QVector4D(max_x,max_y + 0.5f, 0.0f, 1.0f);

    i = min_z;

    while(i <= max_z)
    {
        a.setZ(i);
        b.setZ(i);
        painter.drawLine((projection * a).toPointF(), (projection * b).toPointF());
        i += mag_step;
    }

    // XY

    a = QVector4D(0.0f,min_y - 0.5f,min_z - 0.5f, 1.0f);
    b = QVector4D(0.0f,max_y + 0.5f,min_z - 0.5f, 1.0f);

    i = min_x;
    while(i <= max_x)
    {
        a.setX(i);
        b.setX(i);
        painter.drawLine((projection * a).toPointF(), (projection * b).toPointF());
        i += x_step;
    }

    a = QVector4D(min_x, 0.0f, min_z - 0.5f, 1.0f);
    b = QVector4D(max_x, 0.0f, min_z - 0.5f, 1.0f);

    i = min_y;
    while(i <= max_y)
    {
        a.setY(-i);
        b.setY(-i);
        painter.drawLine((projection * a).toPointF(), (projection * b).toPointF());
        i+= mag_step;
    }

    // YZ

    a = QVector4D(min_x,0.0f,min_z - 0.5f, 1.0f);
    b = QVector4D(min_x,0.0f,max_z + 0.5f, 1.0f);

    i = min_y;
    while(i <= max_y)
    {
        a.setY(-i);
        b.setY(-i);
        painter.drawLine((projection * a).toPointF(), (projection * b).toPointF());
        i+= mag_step;
    }

    a = QVector4D(min_x, min_y - 0.5f, 0.0f, 1.0f);
    b = QVector4D(min_x, max_y + 0.5f, 0.0f, 1.0f);

    i = min_z;
    while(i <= max_z)
    {
        a.setZ(i);
        b.setZ(i);
        painter.drawLine((projection * a).toPointF(), (projection * b).toPointF());
        i+= mag_step;
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


void FourierSpiral2::displayFrequency(double frequency, double magnitudeVal, double phaseVal, int signal_length)
{
    this->frequency = frequency;
    this->magnitude = magnitudeVal;
    this->phase = phaseVal;
    this->signalLength = signal_length;

    displayingEnabled = true;

    Signal impulse;
    impulse.zeroSignal(signalLength);
    while(frequency < 0)
    {
        frequency += signal_length;
    }
    while(frequency >= signal_length)
    {
        frequency -= signal_length;
    }

    if(modify)
    {
        impulse = impulse.makeImpulse(frequency, magnitudeVal);
    }
    else
    {
        impulse = impulse.makeImpulse(frequency, sqrt(signalLength));  // sqrt(length) so that IFT would be in range -1 - 1
    }

    Signal zero;
    QVector<double>  orig_x;
    zero.zeroSignal(signalLength);

    Signal cosinusFrequency;
    Signal sinusFrequency;

    Signal::inverseFourierTransform(impulse,zero,cosinusFrequency);
    Signal::inverseFourierTransform(impulse,zero,sinusFrequency,orig_x, false);

    discretePoints.clear();
    discretePoints.reserve(signalLength);

    const float x_step = (max_x - min_x) / (signalLength + 1);

    for(int i = 0; i < signalLength; i++)
    {
        discretePoints.push_back(QVector4D(min_x + (i + 1)*x_step, -sinusFrequency.original[i], -cosinusFrequency.original[i],  1.0 ));
    }

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
