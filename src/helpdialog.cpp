#include "helpdialog.h"

HelpDialog::HelpDialog(QWidget *parent, const Translation* language) :
    QDialog(parent)
{
    setFixedSize(640,640);

    titleLabel = new QLabel(this);
    titleLabel->setGeometry(QRect(10, 10, 621, 31));

    QFont font;
    font.setPointSize(16);
    titleLabel->setFont(font);
    titleLabel->setAlignment(Qt::AlignCenter);
    text = new QTextBrowser(this);
    text->setGeometry(QRect(10, 50, 621, 551));

    openInPDFButton = new QPushButton(this);
    openInPDFButton->setGeometry(QRect(10, 605, 110, 30));


//    connect(openInPDFButton, &QAction::triggered, this, [=](bool)
//    {
////        QString link = "./resources/FTutod1D.pdf";
////        QDesktopServices::openUrl(QUrl(link));
//    });
    openInPDFButton->setEnabled(false);

    closeButton = new QPushButton(this);
    closeButton->setGeometry(QRect(520, 605, 110, 30));
    connect(closeButton, &QPushButton::clicked,this,&QDialog::reject);

    setLocalizedTexts(language);
}


HelpDialog::~HelpDialog()
{
    delete titleLabel;
    delete text;
    delete openInPDFButton;
    delete closeButton;
}

void HelpDialog::setDefaultTexts()
{
    setWindowTitle(QStringLiteral("FTutor1D Help"));
    titleLabel->setText(QStringLiteral("FTutor1D Help"));
    openInPDFButton->setText(QStringLiteral("Open in PDF"));
    closeButton->setText(QStringLiteral("Close"));

    text->setHtml(htmlTextHeader + "<p>" + "There should be some text, provided by translation file." + "</p>" + htmlTextFooter);

}

void HelpDialog::setLocalizedTexts(const Translation* language)
{
    if(language == nullptr)
    {
        setDefaultTexts();
    }
    setWindowTitle(language->getTitle());
    if(windowTitle().isEmpty()) setWindowTitle(QStringLiteral("FTutor1D Help"));

    titleLabel->setText(language->getChildElementText("titleLabel"));
    if(titleLabel->text().isEmpty()) titleLabel->setText(QStringLiteral("FTutor1D Help"));

    openInPDFButton->setText(language->getChildElementText("openInPDFButton"));
    if(openInPDFButton->text().isEmpty()) openInPDFButton->setText(QStringLiteral("Open in PDF"));

    closeButton->setText(language->getChildElementText("closeButton"));
    if(closeButton->text().isEmpty()) closeButton->setText(QStringLiteral("Close"));


    closeButton->setText(language->getChildElementText("closeButton"));
    openInPDFButton->setText(language->getChildElementText("openInPDFButton"));

    Translation* tr = language->getTranslationForElement(QStringLiteral("text"));

    QString firstParagraph = tr->getChildElementText(QStringLiteral("firstParagraph"));
    QString secondParagraph = tr->getChildElementText(QStringLiteral("secondParagraph"));

    text->setHtml(htmlTextHeader + "<p>" + firstParagraph + "</p>\n<p><br /></p>\n<p>" + secondParagraph + "</p>" + htmlTextFooter);
}
