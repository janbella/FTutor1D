#ifndef PREDEFINEDSIGNALSDIALOG_H
#define PREDEFINEDSIGNALSDIALOG_H

/**
 * @file predefinedsignalsdialog.h
 * @author Ján Bella <xbella1@fi.muni.cz>
 */

#include "localization.h"

#include <QDialog>
#include <QPushButton>
#include <QGroupBox>
#include <QRadioButton>

/**
 * @brief The PredefinedSignalsDialog class is a Dialog in which the user can choose to load one of 8 predefined signals
 */
class PredefinedSignalsDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief PredefinedSignalsDialog constructor
     * @param parent parent object
     * @param signalsFolder a resource folder, in which to look for files to load and images to display
     * @param translation provides localized names for this dialog`
     */
    PredefinedSignalsDialog(QWidget *parent, QString signalsFolder, const Translation* translation);

    /**
     * @brief ~PredefinedSignalsDialog desctructor
     */
    virtual ~PredefinedSignalsDialog();

signals:
    /**
     * @brief signalChosen a signal to notify that a signal was selected
     * @param resourcePath a path to the file which should be loaded
     */
    void signalChosen(QString resourcePath);

private:

    /**
     * @brief setupSignalButton allocates @a signalButton, at position @a geometry, and sets its layout to @imageResource file content
     * @param signalButton a button to set up
     * @param geometry position of the newly created button
     * @param imageResouce an image to display on the top of the button
     */
    void setupSignalButton(QPushButton*& signalButton, QRect geometry, QString imageResouce);

    /**
     * @brief setupRadioButtons allocates and creates callbacks for radio buttons, to distinguish between 16 and 32 samples signals
     * @param groupBox groupbox to contain the two radio buttons
     * @param radio16 radio button to enable the 16 samples signal
     * @param radio32 radio button to enable the 32 samples signal
     * @param geometry position of the groupbox
     * @param pushButton already created instance of pushButton, passed here because of the callback (need to change pushButton layout)
     * @param imageResouce16 a path to imageFile that should be used as layout for pushButton when radio16 is checked
     * @param imageResouce32 a path to imageFile that should be used as layout for pushButton when radio32 is checked
     */
    void setupRadioButtons(QGroupBox*& groupBox, QRadioButton*& radio16, QRadioButton*& radio32, QRect geometry, QPushButton* pushButton, const QString imageResouce16, const QString imageResouce32);

    /**
     * @brief setDefaultTexts sets defaults values to each text or title or label in the window.
     */
    void setDefaultTexts();

    /**
     * @brief setLocalizedTexts sets text, title or label values according to given Translation object`
     * @param language Translation object used to set texts
     */
    void setLocalizedTexts(const Translation* language);


    // Attributes

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
};

#endif // PREDEFINEDSIGNALSDIALOG_H
