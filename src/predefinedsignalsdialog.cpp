#include "predefinedsignalsdialog.h"


PredefinedSignalsDialog::PredefinedSignalsDialog(QWidget *parent, QString signalsFolder, const Translation* translation) : QDialog(parent)
{
    setFixedSize(QSize(850, 410));
    setModal(true);

    setupSignalButton(sinusPushButton,QRect(10, 10, 200, 150), QStringLiteral("sinusPushButton"),signalsFolder + QStringLiteral("/sinus.png"));
    setupSignalButton(cosinusPushButton,QRect(220, 10, 200, 150), QStringLiteral("cosinusPushButton"),signalsFolder + QStringLiteral("/cosinus.png"));
    setupSignalButton(gaussianPushButton,QRect(430, 10, 200, 150), QStringLiteral("gaussianPushButton"),signalsFolder + QStringLiteral("/gaussian.png"));
    setupSignalButton(impulsePushButton,QRect(640, 10, 200, 150), QStringLiteral("impulsePushButton"),signalsFolder + QStringLiteral("/impulse.png"));
    setupSignalButton(stepPushButton,QRect(10, 210, 200, 150), QStringLiteral("stepPushButton"),signalsFolder + QStringLiteral("/step.png"));
    setupSignalButton(rampPushButton,QRect(220, 210, 200, 150), QStringLiteral("rampPushButton"),signalsFolder + QStringLiteral("/ramp.png"));
    setupSignalButton(constantPushButton,QRect(430, 210, 200, 150), QStringLiteral("constantPushButton"),signalsFolder + QStringLiteral("/constant.png"));
    setupSignalButton(sincPushButton,QRect(640, 210, 200, 150), QStringLiteral("sincPushButton"),signalsFolder + QStringLiteral("/sinc.png"));

    cancelButton = new QPushButton(QStringLiteral("Cancel"),this);
    cancelButton->setGeometry(740,370,100,30);

    connect(sinusPushButton, &QPushButton::clicked, [=](bool) {
        emit signalChosen(signalsFolder + QStringLiteral("/sinus_32.data"));
        accept();
    } );

    connect(cosinusPushButton, &QPushButton::clicked, [=](bool) {
        emit signalChosen(signalsFolder + QStringLiteral("/cosinus_32.data"));
        accept();
    } );

    connect(gaussianPushButton, &QPushButton::clicked, [=](bool) {
        emit signalChosen(signalsFolder + QStringLiteral("/gaussian_32.data"));
        accept();
    } );

    connect(impulsePushButton, &QPushButton::clicked, [=](bool) {
        emit signalChosen(signalsFolder + QStringLiteral("/impulse_15_32.data"));
        accept();
    } );

    connect(stepPushButton, &QPushButton::clicked, [=](bool) {
        emit signalChosen(signalsFolder + QStringLiteral("/step_32.data"));
        accept();
    } );

    connect(rampPushButton, &QPushButton::clicked, [=](bool) {
        emit signalChosen(signalsFolder + QStringLiteral("/ramp_32.data"));
        accept();
    } );

    connect(constantPushButton, &QPushButton::clicked, [=](bool) {
        emit signalChosen(signalsFolder + QStringLiteral("/constant_1_32.data"));
        accept();
    } );

    connect(sincPushButton, &QPushButton::clicked, [=](bool) {
        emit signalChosen(signalsFolder + QStringLiteral("/sinc_32.data"));
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
    delete  sinusPushButton;
    delete cosinusPushButton;
    delete impulsePushButton;
    delete gaussianPushButton;
    delete stepPushButton;
    delete constantPushButton;
    delete rampPushButton;
    delete sincPushButton;
    delete cancelButton;
}

void PredefinedSignalsDialog::setupSignalButton(QPushButton*& signalButton, QRect geometry, QString name, QString imageResouce)
{
    signalButton = new QPushButton(this);
    signalButton->setObjectName(name);
    signalButton->setGeometry(geometry);
    signalButton->setMinimumSize(QSize(100, 100));

    QPixmap pixmap(imageResouce);

    QIcon icon(pixmap);
    signalButton->setIcon(icon);
    signalButton->setIconSize(QSize(200, 150));
    signalButton->setFixedSize(QSize(200, 150));
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


