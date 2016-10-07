/**
 * @file aboutdialog.cpp
 * @author Ján Bella
 */

#include "aboutdialog.h"
#include "localization.h"

#include <QGraphicsPixmapItem>

#define APP_VERSION "VERSION 1.0"

using namespace FT1D;

AboutDialog::AboutDialog(QWidget *parent, const Translation* language, QString icon) : QDialog(parent)
{
    setEnabled(true);
    setFixedSize(370,240);
    setModal(true);

    logoGraphicsView = new QGraphicsView(this);
    logoGraphicsView->setGeometry(10, 15, 110, 110);

    QImage img(icon);

    scene = new QGraphicsScene();
    scene->addItem(new QGraphicsPixmapItem(QPixmap::fromImage(img)));
    logoGraphicsView->setScene(scene);
    logoGraphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatioByExpanding);
    logoGraphicsView->show();

    labelAppName = new QLabel(this);
    labelAppName->setGeometry(130,20,230,31);
    labelAppName->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QFont font;
    font.setPointSize(28);
    font.setBold(true);
    font.setWeight(75);
    labelAppName->setFont(font);
    labelAppName->setText("FTutor1D");

    labelVersion = new QLabel(this);
    labelVersion->setGeometry(QRect(240, 50, 120, 17));
    labelVersion->setText(APP_VERSION);

    labelLongName = new QLabel(this);
    labelLongName->setGeometry(QRect(130, 90, 200, 17));
    labelLongName->setText("Fourier Transform Tutor");

    labelOfficialWebsite = new QLabel(this);
    labelOfficialWebsite->setGeometry(130, 110, 200, 17);
    labelOfficialWebsite->setTextFormat(Qt::RichText);
    labelOfficialWebsite->setOpenExternalLinks(true);
    labelOfficialWebsite->setTextInteractionFlags(Qt::TextBrowserInteraction);

    line = new QFrame(this);
    line->setGeometry(QRect(0, 135, 370, 3));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    labelAuthor = new QLabel(this);
    labelAuthor->setGeometry(QRect(10, 145, 60, 17));
    // text is set by localization

    labelAuthorName = new QLabel(this);
    labelAuthorName->setGeometry(QRect(130, 145, 70, 17));
    labelAuthorName->setTextFormat(Qt::RichText);
    labelAuthorName->setOpenExternalLinks(true);
    labelAuthorName->setTextInteractionFlags(Qt::TextBrowserInteraction);
    labelAuthorName->setText("<a href=\"mailto:jan-bella@hotmail.com\">Ján Bella</a>");


    labelCredits = new QLabel(this);
    labelCredits->setGeometry(QRect(10, 170, 350, 17));
    // text is set by localization

    labelInstitution = new QLabel(this);
    labelInstitution->setGeometry(QRect(10, 190, 350, 17));
    // text is set by localization

    labelYear = new QLabel(this);
    labelYear->setGeometry(QRect(10, 210, 70, 20));
    labelYear->setText("© 2016");

    if(language)
    {
        setLocalizedTexts(language);
    }
    else
    {
        setDefaultTexts();
    }
}

AboutDialog::~AboutDialog()
{
    delete scene;
    delete logoGraphicsView;
    delete line;
    delete labelAppName;
    delete labelVersion;
    delete labelLongName;
    delete labelAuthor;
    delete labelAuthorName;
    delete labelOfficialWebsite;
    delete labelCredits;
    delete labelInstitution;
    delete labelYear;
}


void AboutDialog::setDefaultTexts()
{
    setWindowTitle(QStringLiteral("About FTutor1D"));
    labelAuthor->setText(QStringLiteral("Author"));
    labelCredits->setText(QStringLiteral("Inspired by FTutor2D by Petr Sečkář"));
    labelInstitution->setText(QStringLiteral("Faculty of Informatics, Masaryk University in Brno"));
    labelOfficialWebsite->setText("<a href=\"http://www.cbia.fi.muni.cz/projects/ftutor1d.html\">Official website</a>");
}


void AboutDialog::setLocalizedTexts(const Translation* language)
{
    setWindowTitle(language->getTitle());
    if(windowTitle().isEmpty()) setWindowTitle(QStringLiteral("About FTutor1D"));

    labelAuthor->setText(language->getChildElementText(QStringLiteral("labelAuthor")));
    if(labelAuthor->text().isEmpty()) labelAuthor->setText(QStringLiteral("Author"));
    labelCredits->setText(language->getChildElementText("labelCredits"));
    if(labelCredits->text().isEmpty()) labelCredits->setText(QStringLiteral("Based on FTutor2D by Petr Sečkář"));
    labelInstitution->setText(language->getChildElementText("labelInstitution"));
    if(labelInstitution->text().isEmpty()) labelInstitution->setText(QStringLiteral("Faculty of Informatics, Masaryk University in Brno"));

    QString s = language->getChildElementText(QStringLiteral("labelOfficialWebsite"));
    if(s.isEmpty()) s = QStringLiteral("Official website");
    labelOfficialWebsite->setText(QStringLiteral("<a href=\"http://www.cbia.fi.muni.cz/projects/ftutor1d.html\">") + s + QStringLiteral("</a>"));
}


void AboutDialog::showEvent(QShowEvent *)
{
    logoGraphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
}
