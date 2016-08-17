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

    void setupSignalButton(QPushButton*& signalButton, QRect geometry, QString name, QString imageResouce);

    QPushButton *sinusPushButton;
    QPushButton *cosinusPushButton;
    QPushButton *impulsePushButton;
    QPushButton *gaussianPushButton;
    QPushButton *stepPushButton;
    QPushButton *constantPushButton;
    QPushButton *rampPushButton;
    QPushButton *sincPushButton;
    QPushButton *cancelButton;


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
