#include "displaysignalwidget.h"
#include "ui_displaysignalwidget.h"

DisplaySignalWidget::DisplaySignalWidget(QWidget *parent) :  QFrame(parent)
{
    if (objectName().isEmpty())
    {
        setObjectName(QStringLiteral("DisplaySignalWidget"));
    }
    resize(470, 300);

    setFrameShape(QFrame::StyledPanel);
    setFrameShadow(QFrame::Raised);
    plot = new QCustomPlot(this);
    plot->setObjectName(QStringLiteral("plot"));
    plot->setGeometry(QRect(0, 30, 450, 250));
    plot->setCursor(QCursor(Qt::CrossCursor));
    verticalScrollBar = new QScrollBar(this);
    verticalScrollBar->setObjectName(QStringLiteral("verticalScrollBar"));
    verticalScrollBar->setGeometry(QRect(450, 30, 16, 250));
    verticalScrollBar->setOrientation(Qt::Vertical);
    horizontalScrollBar = new QScrollBar(this);
    horizontalScrollBar->setObjectName(QStringLiteral("horizontalScrollBar"));
    horizontalScrollBar->setGeometry(QRect(0, 280, 450, 16));
    horizontalScrollBar->setOrientation(Qt::Horizontal);
    label = new QLabel(this);
    label->setObjectName(QStringLiteral("label"));
    label->setGeometry(QRect(0, 0, 470, 20));
    QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
    label->setSizePolicy(sizePolicy1);
    label->setAlignment(Qt::AlignCenter);

    QMetaObject::connectSlotsByName(this);
}

DisplaySignalWidget::~DisplaySignalWidget()
{
    delete label;
    delete verticalScrollBar;
    delete horizontalScrollBar;
    delete plot;
}
