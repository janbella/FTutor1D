/**
 * @file aboutdialog.cpp
 * @author Ján Bella
 */

#include "aboutdialog.h"
#include "localization.h"

#define APP_VERSION "VERSION 0.1"

AboutDialog::AboutDialog(QWidget *parent, const Translation* language, QString icon) : QDialog(parent)
{
    setEnabled(true);
    setFixedSize(360,240);
    setModal(true);

    logoGraphicsView = new QGraphicsView(this);
    logoGraphicsView->setGeometry(QRect(10, 10, 100, 100));
    logoGraphicsView->setMinimumSize(QSize(100, 100));

    QImage img(icon);

    QGraphicsScene* scene = new QGraphicsScene();
    scene->addPixmap(QPixmap::fromImage(img));
    logoGraphicsView->setScene(scene);
    logoGraphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatioByExpanding);

    line = new QFrame(this);
    line->setGeometry(QRect(0, 120, 360, 3));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    labelAppName = new QLabel(this);
    labelAppName->setGeometry(130,20,170,31);
    labelAppName->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QFont font;
    font.setPointSize(28);
    font.setBold(true);
    font.setWeight(75);
    labelAppName->setFont(font);
    labelAppName->setText("FTutor1D");

    labelVersion = new QLabel(this);
    labelVersion->setGeometry(QRect(240, 50, 100, 17));
    labelVersion->setText(APP_VERSION);

    labelLongName = new QLabel(this);
    labelLongName->setGeometry(QRect(130, 90, 200, 17));
    labelLongName->setText("Fourier Transform Tutor");


    labelAuthor = new QLabel(this);
    labelAuthor->setGeometry(QRect(10, 130, 60, 17));
    // text is set by localization

    labelAuthorName = new QLabel(this);
    labelAuthorName->setGeometry(QRect(100, 130, 70, 17));
    labelAuthorName->setTextFormat(Qt::RichText);
    labelAuthorName->setOpenExternalLinks(true);
    labelAuthorName->setTextInteractionFlags(Qt::TextBrowserInteraction);
    labelAuthorName->setText("<a href=\"mailto:jan-bella@hotmail.com\">Ján Bella</a>");

    labelCredits = new QLabel(this);
    labelCredits->setGeometry(QRect(10, 160, 340, 17));
    // text is set by localization

    labelInstitution = new QLabel(this);
    labelInstitution->setGeometry(QRect(10, 180, 340, 17));
    // text is set by localization

    labelYear = new QLabel(this);
    labelYear->setGeometry(QRect(10, 200, 70, 20));
    labelYear->setText("© 2016");


    line2 = new QFrame(this);
    line2->setGeometry(QRect(0, 230, 360, 3));
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Sunken);

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
    delete logoGraphicsView;
    delete line;
    delete labelAppName;
    delete labelVersion;
    delete labelLongName;
    delete labelAuthor;
    delete labelAuthorName;
    delete labelCredits;
    delete labelInstitution;
    delete labelYear;
    delete line2;
}


void AboutDialog::setDefaultTexts()
{
    setWindowTitle(QStringLiteral("About FTutor1D"));
    labelAuthor->setText(QStringLiteral("Author"));
    labelCredits->setText(QStringLiteral("Based on FTutor2D by Petr Sečkář"));
    labelInstitution->setText(QStringLiteral("Faculty of Informatics, Masaryk University in Brno"));
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
}

