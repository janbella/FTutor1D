#include "fourierspiralwidget.h"

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


FourierSpiralWidget::FourierSpiralWidget(QWidget *parent) : QWidget(parent)
{
    QMatrix4x4 camera;

    QVector3D eye(10,-8,3);
    QVector3D center(-1.0f,-2.0f,-2.0f);
    QVector3D up(0.0f,1.0f,0.0f);

    camera.lookAt(eye, center, up);

    QMatrix4x4 ort;

    ort.setToIdentity();
    ort.ortho(-3.5,0,-1,1,0,1);

    QMatrix4x4 toImageSpace(this->width(), 0.0f,                 0.0f, this->width() / 2.0f + 100,
                            0.0f,          this->height(), 0.0f, this->height() / 2.0f,
                            0.0f,          0.0f,                 0.0f, 0.0f,
                            0.0f,          0.0f,                 0.0f, 1.0f );

    projection = toImageSpace * ort * camera;

    frequency = 0;
    magnitude = 0;
    maxMagnitude = 0;
    phase = 0;
    signalLength = 0;
    modify = false;
    displayingEnabled = false;
}

void FourierSpiralWidget::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);

    // draw white colour to the background
    painter.fillRect(0,0,this->width(), this->height(),Qt::white);

    drawBackground(painter);
    drawAxes(painter);
    drawTexts(painter);

    float magScale = 1;
    float phaseShift = 0;

    if(modify)
    {
        const float mag_max = (fabs(maxMagnitude) < 2.0f ||  !modify)? 2.0f : ceil(fabs(maxMagnitude));

        magScale = magnitude * 2.0f / mag_max;
        phaseShift = phase;
    }

    const float x_step = (max_x - min_x) / (signalLength + 1);

    if(displayingEnabled)
    {
        if(frequency == 0)
        {
            painter.setPen((Qt::black));
            painter.setBrush(QColor::fromRgb(0xFF,0xCC,0x00));

            painter.drawEllipse((projection * QVector4D(min_x, 0.0, -1.0f * magScale ,1.0)).toPointF(),2,2);


            // projection

            painter.setPen(QPen(QColor::fromRgb(0x33,0x99,0x00),2));
            painter.drawLine((projection * QVector4D(min_x + x_step, max_y + 0.5, -1.0f * magScale ,1.0)).toPointF(),
                             (projection * QVector4D(max_x - x_step, max_y + 0.5, -1.0f * magScale ,1.0)).toPointF());


            painter.setPen((Qt::black));
            painter.setBrush(QColor::fromRgb(0xFF,0xCC,0x00));

            for(int i = 0; i < signalLength; i++)
            {
                painter.drawEllipse((projection * QVector4D(min_x + (i + 1)*x_step, max_y + 0.5, -1.0f * magScale, 1.0)).toPointF(), 2.5, 2.5);
            }

            // projection
            painter.setPen(QPen(QColor::fromRgb(0xFF,0x33,0x00),2));

            painter.drawLine((projection * QVector4D(min_x + x_step, 0.0f,  min_z - 0.5f, 1.0)).toPointF(),
                             (projection * QVector4D(max_x - x_step, 0.0f,  min_z - 0.5f, 1.0)).toPointF());

            painter.setPen((Qt::black));
            painter.setBrush(QColor::fromRgb(0x00,0x33,0xFF));

            for(int i = 0; i < signalLength; i++)
            {
                painter.drawEllipse((projection * QVector4D(min_x + (i + 1)*x_step, 0.0f,  min_z - 0.5f, 1.0f)).toPointF(), 2.5, 2.5);
            }

            // basis function
            painter.setPen(QPen(QColor::fromRgb(0xCC,0,0xCC),3));

            painter.drawLine((projection * QVector4D(min_x + x_step, 0.0f, -1.0f * magScale, 1.0)).toPointF(),
                             (projection * QVector4D(max_x - x_step, 0.0f, -1.0f * magScale, 1.0)).toPointF());

            painter.setPen((Qt::black));
            painter.setBrush(Qt::yellow);

            for(int i = 0; i < signalLength; i++)
            {
                painter.drawEllipse((projection * QVector4D(min_x + (i + 1)*x_step, 0.0f, -1.0f * magScale, 1.0f)).toPointF(), 2.5, 2.5);
            }

        }
        else
        {
            const float xLength = (max_x - min_x) - x_step;
            const float numberPointsPerPeriod = 15;

            size_t numberPointsTotal = fabs(frequency) * numberPointsPerPeriod;
            float spacing = xLength / numberPointsTotal;

            QVector<float> y;
            QVector<float> z;

            QVector<float> dy;
            QVector<float> dz;

            y.reserve(numberPointsTotal + 1);
            z.reserve(numberPointsTotal + 1);

            for(size_t i = 0; i <= numberPointsTotal; i++)
            {
                float newY = -sinf(frequency * 2.0*M_PI/ (numberPointsTotal) * i + phaseShift ) * magScale;
                float newZ = -cosf(frequency * 2.0*M_PI/ (numberPointsTotal) * i + phaseShift ) * magScale;
                y.push_back(newY);
                z.push_back(newZ);
            }

            for(size_t i = 0; i < signalLength; i++)
            {
                float newY = -sinf(frequency * 2.0*M_PI/ (signalLength) * i + phaseShift ) * magScale;
                float newZ = -cosf(frequency * 2.0*M_PI/ (signalLength) * i + phaseShift ) * magScale;
                dy.push_back(newY);
                dz.push_back(newZ);
            }

            QVector<QPointF> points;


            // draw the circle to the background
            float theta_base = 2.0f * M_PI / 20.0f;

            for(int ii = 0; ii < 20; ii++)
            {
                points.push_back((projection * QVector4D(min_x, -cosf(theta_base * ii) * magScale, sinf(theta_base * ii) * magScale, 1.0f)).toPointF());
            }

            painter.setPen(QPen(QColor::fromRgb(255,128,128),1.5));
            painter.drawPolygon(&points[0],20);

            // draw points there.
            painter.setPen((Qt::black));
            painter.setBrush(QColor::fromRgb(0xFF,0xCC,0x00));
            for(size_t i = 0; i < signalLength; i++)
            {
                painter.drawEllipse((projection * QVector4D(min_x, dy[i], dz[i], 1.0)).toPointF(), 2.0,2.0);
            }



            // first draw projection to the lower plane - real projection of the basis function

            points.clear();
            for(size_t i = 0; i <= numberPointsTotal; i++)
            {
                points.push_back((projection * QVector4D(min_x + x_step + spacing * i, max_y + 0.5, z[i], 1)).toPointF());
            }
            painter.setPen(QPen(QColor::fromRgb(0x33,0x99,0x00),2));
            painter.drawPolyline(&points[0],numberPointsTotal + 1);


            painter.setPen((Qt::black));
            painter.setBrush(QColor::fromRgb(0xFF,0xCC,0x00));

            for(int i = 0; i < signalLength; i++)
            {
                painter.drawEllipse((projection * QVector4D(min_x + (i + 1)*x_step, max_y + 0.5f, dz[i], 1.0)).toPointF(), 2.5,2.5);
            }


            // second, draw projection to the further plane - imaginary projection of the basis function

            points.clear();

            for(size_t i = 0; i <= numberPointsTotal; i++)
            {
                points.push_back((projection * QVector4D(min_x + x_step + spacing * i, y[i], -2.5, 1)).toPointF());
            }
            painter.setPen(QPen(QColor::fromRgb(0xFF,0x33,0x00),2));
            painter.drawPolyline(&points[0],numberPointsTotal + 1);

            painter.setPen((Qt::black));
            painter.setBrush(QColor::fromRgb(0x00,0x33,0xFF));

            for(int i = 0; i < signalLength; i++)
            {
                painter.drawEllipse((projection * QVector4D(min_x + (i + 1)*x_step, dy[i], min_z - 0.5f, 1.0)).toPointF(), 2.5,2.5);
            }

            // last, draw the basis funtion to the space.

            points.clear();

            for(size_t i = 0; i <= numberPointsTotal; i++)
            {
                points.push_back((projection * QVector4D(min_x + x_step + spacing * i, y[i], z[i], 1)).toPointF());
            }


            painter.setPen(QPen(QColor::fromRgb(0xCC,0,0xCC),3));
            painter.drawPolyline(&points[0],numberPointsTotal + 1);

            painter.setPen((Qt::black));
            painter.setBrush(Qt::yellow);

            const float x_step = (max_x - min_x) / (signalLength + 1);

            for(int i = 0; i < signalLength; i++)
            {
                painter.drawEllipse((projection * QVector4D(min_x + (i + 1)*x_step, dy[i], dz[i], 1.0f)).toPointF(), 3,3);
            }
        }
    }
}


void FourierSpiralWidget::drawAxes(QPainter& painter)
{
    painter.save();

    painter.setPen(Qt::red);
    painter.drawLine((projection * QVector4D(min_x, max_y + 0.5, max_z + 0.5,1.0)).toPointF(),
                     (projection * QVector4D(max_x + 0.2, max_y + 0.5, max_z + 0.5,1.0)).toPointF());

    QPainterPath path1;
    path1.moveTo ((projection * QVector4D(max_x + 0.2, max_y + 0.5, max_z + 0.5 - 0.1, 1.0)).toPointF());
    path1.lineTo ((projection * QVector4D(max_x + 0.2, max_y + 0.5, max_z + 0.5 + 0.1, 1.0)).toPointF());
    path1.lineTo ((projection * QVector4D(max_x + 0.2 + 0.3, max_y + 0.5, max_z + 0.5 - 0.1, 1.0)).toPointF());
    path1.lineTo ((projection * QVector4D(max_x + 0.2, max_y + 0.5, max_z + 0.5 - 0.1, 1.0)).toPointF());
    painter.setPen (Qt :: NoPen);
    painter.fillPath (path1, QBrush (Qt::red));

    painter.setPen(Qt::green);
    painter.drawLine((projection * QVector4D(min_x, min_y - 0.5, max_z + 0.5,1.0)).toPointF(),
                     (projection * QVector4D(min_x, max_y + 0.5, max_z + 0.5,1.0)).toPointF());

    QPainterPath path2; //y je nahor, z je nabok
    path2.moveTo ((projection * QVector4D(min_x, min_y - 0.5, max_z + 0.5 - 0.1, 1.0)).toPointF());
    path2.lineTo ((projection * QVector4D(min_x, min_y - 0.5 - 0.3, max_z + 0.5 + 0.0, 1.0)).toPointF());
    path2.lineTo ((projection * QVector4D(min_x, min_y - 0.5, max_z + 0.5 + 0.1, 1.0)).toPointF());
    path2.lineTo ((projection * QVector4D(min_x, min_y - 0.5, max_z + 0.5 - 0.1, 1.0)).toPointF());
    painter.setPen (Qt :: NoPen);
    painter.fillPath (path2, QBrush (Qt::green));

    painter.setPen(Qt::blue);
    painter.drawLine(QLine((projection * QVector4D(max_x, max_y + 0.5, max_z + 0.5, 1.0)).toPoint(),
                           (projection * QVector4D(max_x, max_y + 0.5, min_z - 0.7, 1.0)).toPoint()));

    QPainterPath path3;
    path3.moveTo ((projection * QVector4D(max_x - 0.1, max_y + 0.5, min_z - 0.5, 1.0)).toPointF());
    path3.lineTo ((projection * QVector4D(max_x + 0.0, max_y + 0.5, min_z - 0.5 - 0.3,1.0)).toPointF());
    path3.lineTo ((projection * QVector4D(max_x + 0.1, max_y + 0.5, min_z - 0.5 ,1.0)).toPointF());
    path3.lineTo ((projection * QVector4D(max_x - 0.1, max_y + 0.5, min_z - 0.5 ,1.0)).toPointF());
    painter.setPen (Qt :: NoPen);
    painter.fillPath (path3, QBrush (Qt::blue));

    painter.restore();
}

void FourierSpiralWidget::drawBackground(QPainter& painter)
{
    painter.save();

    QBrush brush(QColor::fromRgb(0,0,0));
    QPen pen(brush,0.5,Qt::DashLine);
    painter.setPen(pen);

    // Ugly thing: because setting the projection and viewport correctly is somehow difficult
    // To reflect the length of the signal, use the interpolation within the set interval

    const float mag_max = (fabs(maxMagnitude) < 2.0f ||  !modify)? 2.0f : ceil(fabs(maxMagnitude));
    const float mag_min = -mag_max;

    const float x_step = (max_x - min_x) / (signalLength + 1);
    const float mag_step = (max_y - min_y) / (mag_max - mag_min);

    int timeLabelStep = 1.0f / x_step;
    int magLabelStep = 1.0f / mag_step;
    if(magLabelStep <= 1) magLabelStep = 1;
    else
    {
        magLabelStep -= magLabelStep % 2;
        if(magLabelStep > 4)
        {
            magLabelStep -= magLabelStep % 4;
        }
    }

    QVector4D a(0.0f, max_y + 0.5f, max_z + 0.5f, 1.0f);
    QVector4D b(0.0f, max_y + 0.5f, min_z - 0.5f, 1.0f);

    float p = min_x;

    int i = -1;
    int j = 0;

    // XZ

    while(p <= max_x + 0.01)
    {
        a.setX(p);
        b.setX(p);

        QPointF pa = (projection * a).toPointF();

        painter.drawLine(pa, (projection * b).toPointF());

        if(timeLabelStep!= 0 && p != min_x && (j + 1) % timeLabelStep == 0)
        {
            pa.setX(pa.x() - 25);
            pa.setY(pa.y() + 10);
            if(i < 0 || i > 9)
            {
                painter.drawText(pa,QString::number(i));
            }
            else
            {
                painter.drawText(pa,QString("  ") + QString::number(i));
            }
        }
        p += x_step;
        i++;
        j++;
    }

    a = QVector4D(min_x,max_y + 0.5f, 0.0f, 1.0f);
    b = QVector4D(max_x,max_y + 0.5f, 0.0f, 1.0f);

    p = min_z;

    i = 0;
    j = 0;

    p = (max_z + min_z) / 2.0;


    while(p <= max_z + 0.01)
    {
        a.setZ(p);
        b.setZ(p);

        QPointF pb = (projection * b).toPointF();

        painter.drawLine((projection * a).toPointF(), pb);


        if(magLabelStep!= 0 && (j) % magLabelStep == 0)
        {
            pb.setY(pb.y() + 15);
            painter.drawText(pb,QString::number(-i));
        }
        p += mag_step;
        i++;
        j++;
    }

    p = (max_z + min_z) / 2.0;
    p -= mag_step;
    i = -1;
    j = 1;

    while(p >= min_z - 0.01)
    {
        a.setZ(p);
        b.setZ(p);

        QPointF pb = (projection * b).toPointF();

        painter.drawLine((projection * a).toPointF(), pb);


        if(magLabelStep!= 0 && (j) % magLabelStep == 0)
        {
            pb.setY(pb.y() + 15);
            painter.drawText(pb,QString::number(-i));
        }
        p -= mag_step;
        i--;
        j++;
    }


    // XY

    a = QVector4D(0.0f,min_y - 0.5f,min_z - 0.5f, 1.0f);
    b = QVector4D(0.0f,max_y + 0.5f,min_z - 0.5f, 1.0f);

    p = min_x;
    while(p <= max_x + 0.01)
    {
        a.setX(p);
        b.setX(p);
        painter.drawLine((projection * a).toPointF(), (projection * b).toPointF());
        p += x_step;
    }

    a = QVector4D(min_x, 0.0f, min_z - 0.5f, 1.0f);
    b = QVector4D(max_x, 0.0f, min_z - 0.5f, 1.0f);

    p = min_y;
    while(p <= max_y + 0.01)
    {
        a.setY(-p);
        b.setY(-p);
        painter.drawLine((projection * a).toPointF(), (projection * b).toPointF());
        p+= mag_step;
    }


    // YZ

    a = QVector4D(min_x,0.0f,min_z - 0.5f, 1.0f);
    b = QVector4D(min_x,0.0f,max_z + 0.5f, 1.0f);

    p = (max_y + min_y) / 2.0f;


    i = 0;
    j = 0;

    while(p <= max_y + 0.01)
    {
        a.setY(-p);
        b.setY(-p);

        QPointF pb = (projection * b).toPointF();

        painter.drawLine((projection * a).toPointF(), pb);

        if(magLabelStep!= 0 && (j) % magLabelStep == 0)
        {
            pb.setX(pb.x() - 16);
            pb.setY(pb.y() + 5);
            if(p < 0)
            {
                painter.drawText(pb,QString::number(i));
            }
            else
            {
                painter.drawText(pb,QString(" ") + QString::number(i));
            }
        }

        p+= mag_step;
        i++;
        j++;
    }

    p = (max_y + min_y) / 2.0f;
    p -= mag_step;

    i = -1;
    j = 1;


    while(p >= min_y - 0.01)
    {
        a.setY(-p);
        b.setY(-p);

        QPointF pb = (projection * b).toPointF();

        painter.drawLine((projection * a).toPointF(), pb);

        if(magLabelStep!= 0 && (j) % magLabelStep == 0)
        {
            pb.setX(pb.x() - 16);
            pb.setY(pb.y() + 5);
            if(p < 0)
            {
                painter.drawText(pb,QString::number(i));
            }
            else
            {
                painter.drawText(pb,QString(" ") + QString::number(i));
            }
        }

        p -= mag_step;
        i--;
        j++;
    }

    a = QVector4D(min_x, min_y - 0.5f, 0.0f, 1.0f);
    b = QVector4D(min_x, max_y + 0.5f, 0.0f, 1.0f);

    p = min_z;
    while(p <= max_z + 0.01)
    {
        a.setZ(p);
        b.setZ(p);
        painter.drawLine((projection * a).toPointF(), (projection * b).toPointF());
        p+= mag_step;
    }
    painter.restore();
}

void FourierSpiralWidget::drawTexts(QPainter& painter)
{
    painter.save();
    painter.setPen(QColor("black"));
    painter.drawText(1, 13, "Imag");
    painter.drawText(52, 250, "T");
    painter.drawText(435, 268, "Real");
    painter.restore();
}


void FourierSpiralWidget::displayFrequency(double frequency, double magnitudeVal, double phaseVal, double maxMagnitudeVal, int signal_length)
{
    this->frequency = frequency;
    this->magnitude = magnitudeVal;
    this->phase = phaseVal;
    this->signalLength = signal_length;
    this->maxMagnitude = maxMagnitudeVal;

    displayingEnabled = true;

    update();
}

void FourierSpiralWidget::setNormalized(bool value)
{
    modify = !value;
    repaint();
}

void FourierSpiralWidget::clearFrequency()
{
    displayingEnabled = false;
    repaint();
}
