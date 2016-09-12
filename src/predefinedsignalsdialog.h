#ifndef PREDEFINEDSIGNALSDIALOG_H
#define PREDEFINEDSIGNALSDIALOG_H

#include "glob_includes.h"
#include "localization.h"

class PredefinedSignalsDialog : public QDialog
{
    Q_OBJECT


public:

    PredefinedSignalsDialog(QWidget *parent, QString signalsFolder, const Translation* translation);

    virtual ~PredefinedSignalsDialog();

signals:
    void signalChosen(QString resourcePath);

private:

    void setupSignalButton(QPushButton*& signalButton, QRect geometry, QString imageResouce);

    void setupRadioButtons(QGroupBox*& groupBox, QRadioButton*& radio16, QRadioButton*& radio32, QRect geometry, QPushButton* pushButton, const QString imageResouce16, const QString imageResouce32);

    QPushButton *sinusPushButton;
    QPushButton *cosinusPushButton;
    QPushButton *impulsePushButton;
    QPushButton *gaussianPushButton;
    QPushButton *stepPushButton;
    QPushButton *constantPushButton;
    QPushButton *rampPushButton;
    QPushButton *sincPushButton;
    QPushButton *cancelButton;

    QGroupBox* sinusGroupBox;
    QGroupBox* cosinusGroupBox;
    QGroupBox* impulseGroupBox;
    QGroupBox* gaussianGroupBox;
    QGroupBox* stepGroupBox;
    QGroupBox* constantGroupBox;
    QGroupBox* rampGroupBox;
    QGroupBox* sincGroupBox;

    QRadioButton* sinusRadioButton16;
    QRadioButton* sinusRadioButton32;

    QRadioButton* cosinusRadioButton16;
    QRadioButton* cosinusRadioButton32;

    QRadioButton* impulseRadioButton16;
    QRadioButton* impulseRadioButton32;

    QRadioButton* gaussianRadioButton16;
    QRadioButton* gaussianRadioButton32;

    QRadioButton* stepRadioButton16;
    QRadioButton* stepRadioButton32;

    QRadioButton* constantRadioButton16;
    QRadioButton* constantRadioButton32;

    QRadioButton* rampRadioButton16;
    QRadioButton* rampRadioButton32;

    QRadioButton* sincRadioButton16;
    QRadioButton* sincRadioButton32;


    /**
     * @brief setDefaultTexts sets defaults values to each text or title or label in the window.
     */
    void setDefaultTexts();

    /**
     * @brief setLocalizedTexts sets text, title or label values according to given Translation object`
     * @param language Translation object used to set texts
     */
    void setLocalizedTexts(const Translation* language);
};

#endif // PREDEFINEDSIGNALSDIALOG_H
