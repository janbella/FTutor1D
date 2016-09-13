#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

/**
 * @file aboutdialog.h
 * @author Ján Bella <xbella1@fi.muni.cz>
 */

#include "glob_includes.h"
#include "localization.h"

/**
 * @brief The AboutDialog class is a simple dialog window with information about the application and its creator
 */
class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief AboutDialog costructor
     * @param parent the parent object, should be MainWindow
     * @param localization the instance of Localization class, which provides translated labels
     */
    explicit AboutDialog(QWidget *parent, const Translation* language, QString icon);

    /**
     * Desctructor
     */
    virtual ~AboutDialog();

private:

    // the following are QtWidgets in the window
    QGraphicsView *logoGraphicsView;
    QGraphicsScene* scene;
    QFrame *line;
    QLabel *labelAppName;
    QLabel *labelVersion;
    QLabel *labelLongName;
    QLabel *labelAuthor;
    QLabel *labelAuthorName;
    QLabel *labelOfficialWebsite;
    QLabel *labelCredits;
    QLabel *labelInstitution;
    QLabel *labelYear;

    /**
     * @brief setDefaultTexts sets defaults values to each text or title or label in the window.
     */
    void setDefaultTexts();

    /**
     * @brief setLocalizedTexts sets text, title or label values according to given Translation object`
     * @param language Translation object used to set texts
     */
    void setLocalizedTexts(const Translation* language);

    /**
     * @brief showEvent reimplementing QDialog showEvent to make possible rescaling the logo to fit logoGraphicsView area.
     */
    void showEvent(QShowEvent *);
};

#endif // ABOUTDIALOG_H
