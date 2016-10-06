#ifndef HELPDIALOG_H
#define HELPDIALOG_H

/**
 * @file helpdialog.h
 * @author Ján Bella <xbella1@fi.muni.cz>
 */


#include "localization.h"

#include <QDialog>
#include <QLabel>
#include <QTextBrowser>
#include <QPushButton>

/**
 * @brief The HelpDialog class is a dialog window with information about usage
 */
class HelpDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief HelpDialog constructor
     * @param parent parent widget
     * @param language translation for this object
     */
    explicit HelpDialog(QWidget *parent, const Translation* language);

    /**
     * Destructor
     */
    ~HelpDialog();

private:

    // the following are QtWidgets in the window
    QLabel *titleLabel;
    QTextBrowser *text;
    QPushButton *openInPDFButton;
    QPushButton *closeButton;

    /**
     * @brief setDefaultTexts sets the defaut texts to the class widgets
     */
    void setDefaultTexts();

    /**
     * @brief setLocalizedTexts obtains the translated texts from @a language and displays it in the widgets
     * @param language translation for this window
     */
    void setLocalizedTexts(const Translation* language);

    // default html header
    const QString htmlTextHeader = QStringLiteral("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
                                   "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
                                   "p, li { white-space: pre-wrap; }\n"
                                   "</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n");


    // default html footer
    const QString htmlTextFooter = QStringLiteral("</body></html>");
};

#endif // HELPDIALOG_H
