#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include "localization.h"

#include "glob_includes.h"

namespace Ui {
class HelpDialog;
}

class HelpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HelpDialog(QWidget *parent, const Translation* language);
    ~HelpDialog();

private:
    QLabel *titleLabel;
    QTextBrowser *text;
    QPushButton *openInPDFButton;
    QPushButton *closeButton;

    void setDefaultTexts();

    void setLocalizedTexts(const Translation* language);

    const QString htmlTextHeader = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
                                   "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
                                   "p, li { white-space: pre-wrap; }\n"
                                   "</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n";


    const QString htmlTextFooter = "</body></html>";
};

#endif // HELPDIALOG_H
