#include "predefinedsignalsdialog.h"

using namespace FT1D;

PredefinedSignalsDialog::PredefinedSignalsDialog(QWidget *parent, QString signalsFolder, const Translation* translation) : QDialog(parent)
{
    setFixedSize(QSize(850, 440));
    setModal(true);

    setupSignalButton(sinusPushButton,QRect(10, 10, 200, 150), signalsFolder + QStringLiteral("/sinus.png"));
    setupSignalButton(cosinusPushButton,QRect(220, 10, 200, 150), signalsFolder + QStringLiteral("/cosinus.png"));
    setupSignalButton(gaussianPushButton,QRect(430, 10, 200, 150), signalsFolder + QStringLiteral("/gaussian.png"));
    setupSignalButton(impulsePushButton,QRect(640, 10, 200, 150), signalsFolder + QStringLiteral("/impulse.png"));
    setupSignalButton(stepPushButton,QRect(10, 210, 200, 150), signalsFolder + QStringLiteral("/step.png"));
    setupSignalButton(rampPushButton,QRect(220, 210, 200, 150), signalsFolder + QStringLiteral("/ramp.png"));
    setupSignalButton(constantPushButton,QRect(430, 210, 200, 150), signalsFolder + QStringLiteral("/constant.png"));
    setupSignalButton(sincPushButton,QRect(640, 210, 200, 150), signalsFolder + QStringLiteral("/sinc.png"));

    setupRadioButtons(sinusGroupBox, sinusRadioButton16, sinusRadioButton32, QRect(10,170,200,20), sinusPushButton, signalsFolder + QStringLiteral("/sinus_16.png"), signalsFolder + QStringLiteral("/sinus_32.png"));
    setupRadioButtons(cosinusGroupBox, cosinusRadioButton16, cosinusRadioButton32, QRect(220,170,200,20), cosinusPushButton, signalsFolder + QStringLiteral("/cosinus_16.png"), signalsFolder + QStringLiteral("/cosinus_32.png"));
    setupRadioButtons(gaussianGroupBox, gaussianRadioButton16, gaussianRadioButton32, QRect(430,170,200,20),gaussianPushButton, signalsFolder + QStringLiteral("/gaussian_16.png"), signalsFolder + QStringLiteral("/gaussian_32.png"));
    setupRadioButtons(impulseGroupBox, impulseRadioButton16, impulseRadioButton32, QRect(640,170,200,20), impulsePushButton, signalsFolder + QStringLiteral("/impulse_7_16.png"), signalsFolder + QStringLiteral("/impulse_15_32.png"));
    setupRadioButtons(stepGroupBox, stepRadioButton16, stepRadioButton32, QRect(10,370,200,20), stepPushButton, signalsFolder + QStringLiteral("/step_16.png"), signalsFolder + QStringLiteral("/step_32.png"));
    setupRadioButtons(rampGroupBox, rampRadioButton16, rampRadioButton32, QRect(220,370,200,20), rampPushButton, signalsFolder + QStringLiteral("/ramp_16.png"), signalsFolder + QStringLiteral("/ramp_32.png"));
    setupRadioButtons(constantGroupBox, constantRadioButton16, constantRadioButton32, QRect(430,370,200,20), constantPushButton, signalsFolder + QStringLiteral("/constant_1_16.png"), signalsFolder + QStringLiteral("/constant_1_32.png"));
    setupRadioButtons(sincGroupBox, sincRadioButton16, sincRadioButton32, QRect(640,370,200,20), sincPushButton, signalsFolder + QStringLiteral("/sinc_16.png"), signalsFolder + QStringLiteral("/sinc_32.png"));

    cancelButton = new QPushButton(QStringLiteral("Cancel"),this);
    cancelButton->setGeometry(740,400,100,30);

    connect(sinusPushButton, &QPushButton::clicked, [=](bool) {
        if(sinusRadioButton16->isChecked())
            emit signalChosen(signalsFolder + QStringLiteral("/sinus_16.ft1d"));
        else emit signalChosen(signalsFolder + QStringLiteral("/sinus_32.ft1d"));
        accept();
    } );

    connect(cosinusPushButton, &QPushButton::clicked, [=](bool) {
        if(cosinusRadioButton16->isChecked())
            emit signalChosen(signalsFolder + QStringLiteral("/cosinus_16.ft1d"));
        else emit signalChosen(signalsFolder + QStringLiteral("/cosinus_32.ft1d"));
        accept();
    } );

    connect(gaussianPushButton, &QPushButton::clicked, [=](bool) {
        if(gaussianRadioButton16->isChecked())
            emit signalChosen(signalsFolder + QStringLiteral("/gaussian_16.ft1d"));
        else emit signalChosen(signalsFolder + QStringLiteral("/gaussian_32.ft1d"));
        accept();
    } );

    connect(impulsePushButton, &QPushButton::clicked, [=](bool) {
        if(impulseRadioButton16->isChecked())
            emit signalChosen(signalsFolder + QStringLiteral("/impulse_7_16.ft1d"));
        else emit signalChosen(signalsFolder + QStringLiteral("/impulse_15_32.ft1d"));
        accept();
    } );

    connect(stepPushButton, &QPushButton::clicked, [=](bool) {
        if(stepRadioButton16->isChecked())
            emit signalChosen(signalsFolder + QStringLiteral("/step_16.ft1d"));
        else emit signalChosen(signalsFolder + QStringLiteral("/step_32.ft1d"));
        accept();
    } );

    connect(rampPushButton, &QPushButton::clicked, [=](bool) {
        if(rampRadioButton16->isChecked())
            emit signalChosen(signalsFolder + QStringLiteral("/ramp_16.ft1d"));
        else emit signalChosen(signalsFolder + QStringLiteral("/ramp_32.ft1d"));
        accept();
    } );

    connect(constantPushButton, &QPushButton::clicked, [=](bool) {
        if(constantRadioButton16->isChecked())
            emit signalChosen(signalsFolder + QStringLiteral("/constant_1_16.ft1d"));
        else emit signalChosen(signalsFolder + QStringLiteral("/constant_1_32.ft1d"));
        accept();
    } );

    connect(sincPushButton, &QPushButton::clicked, [=](bool) {
        if(sincRadioButton16->isChecked())
            emit signalChosen(signalsFolder + QStringLiteral("/sinc_16.ft1d"));
        else emit signalChosen(signalsFolder + QStringLiteral("/sinc_32.ft1d"));
        accept();
    } );

    connect(cancelButton, &QPushButton::clicked,this,&QDialog::reject);


    if(translation)
    {
        setLocalizedTexts(translation);
    }
    else
    {
        setDefaultTexts();
    }
}

PredefinedSignalsDialog::~PredefinedSignalsDialog()
{
    delete sinusPushButton;
    delete cosinusPushButton;
    delete impulsePushButton;
    delete gaussianPushButton;
    delete stepPushButton;
    delete constantPushButton;
    delete rampPushButton;
    delete sincPushButton;
    delete cancelButton;

    delete sinusRadioButton16;
    delete sinusRadioButton32;
    delete cosinusRadioButton16;
    delete cosinusRadioButton32;
    delete impulseRadioButton16;
    delete impulseRadioButton32;
    delete gaussianRadioButton16;
    delete gaussianRadioButton32;
    delete stepRadioButton16;
    delete stepRadioButton32;
    delete constantRadioButton16;
    delete constantRadioButton32;
    delete rampRadioButton16;
    delete rampRadioButton32;
    delete sincRadioButton16;
    delete sincRadioButton32;

    delete sinusGroupBox;
    delete cosinusGroupBox;
    delete impulseGroupBox;
    delete gaussianGroupBox;
    delete stepGroupBox;
    delete constantGroupBox;
    delete rampGroupBox;
    delete sincGroupBox;
}

void PredefinedSignalsDialog::setupSignalButton(QPushButton*& signalButton, QRect geometry, QString imageResouce)
{
    signalButton = new QPushButton(this);
    signalButton->setGeometry(geometry);
    signalButton->setMinimumSize(QSize(100, 100));

    QPixmap pixmap(imageResouce);

    QIcon icon(pixmap);
    signalButton->setIcon(icon);
    signalButton->setIconSize(QSize(200, 150));
    signalButton->setFixedSize(QSize(200, 150));
}

void PredefinedSignalsDialog::setupRadioButtons(QGroupBox*& groupBox, QRadioButton*& radio16, QRadioButton*& radio32, QRect geometry, QPushButton* pushButton, const QString imageResouce16, const QString imageResouce32)
{
    groupBox = new QGroupBox(this);
    groupBox->setGeometry(geometry);

    radio16 = new QRadioButton(groupBox);
    radio16->setText(QStringLiteral("16"));
    radio16->setGeometry(40,0,50,20);

    radio32 = new QRadioButton(groupBox);
    radio32->setText(QStringLiteral("32"));
    radio32->setGeometry(110,0,50,20);

    connect(radio16, &QRadioButton::clicked, this, [=]()
    {
        QPixmap pixmap(imageResouce16);

        QIcon icon(pixmap);
        pushButton->setIcon(icon);
        pushButton->setIconSize(QSize(200, 150));
        pushButton->setFixedSize(QSize(200, 150));
    });


    connect(radio32, &QRadioButton::clicked, this, [=]()
    {
        QPixmap pixmap(imageResouce32);

        QIcon icon(pixmap);
        pushButton->setIcon(icon);
        pushButton->setIconSize(QSize(200, 150));
        pushButton->setFixedSize(QSize(200, 150));
    });

    radio16->click();
}


void PredefinedSignalsDialog::setDefaultTexts()
{
    setWindowTitle(QStringLiteral("Choose one of the prepared functions..."));
    cancelButton->setText(QStringLiteral("Cancel"));
}


void PredefinedSignalsDialog::setLocalizedTexts(const Translation* language)
{
    setWindowTitle(language->getTitle());
    if(windowTitle().isEmpty()) setWindowTitle(QStringLiteral("Choose one of the prepared functions..."));

    cancelButton->setText(language->getChildElementText(QStringLiteral("cancelButton")));
    if(cancelButton->text().isEmpty()) cancelButton->setText(QStringLiteral("Cancel"));
}


