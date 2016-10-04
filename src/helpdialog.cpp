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

    if(language)
    {
        setLocalizedTexts(language);
    }
    else
    {
        setDefaultTexts();
    }
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

    QString firstParagraph = QStringLiteral("Welcome to FTutor1D, a free tool for visualizing 1D Fourier transform. This text is to provide you with some basic overview on how to use the application.");
    QString secondParagraph = QStringLiteral("The main window is split into four parts by four graphs. The upper two graphs display the signal in frequency domain (magnitude and phase graph on the left) and selected frequency (on the right). The lower two graphs display the original signal (on the left) and the modified signal (on the right).");
    QString thirdParagraph = QStringLiteral("First, you should load a signal using File menu, and clicking Open or Open predefined signal. Whereas the second option opens a window with several prepared signals, the first one requires you to select a file to load. File format is any gnuplot text file (x and y coordinate of one point on one line).");
    QString fourthParagraph = QStringLiteral("There's also an option to define your own signal. Choose File and New in the menu, or right click the original graph and choose open edit mode. Left clicking the graph adds points. Because the points must be equally spaced, predefined spacing is set to 1 and if you skip a position, the signal is supplemented with zero points. In the edit mode, you can delete the points by clicking the right button. Use the middle button and wheel to change the scale.");
    QString fifthParagraph = QStringLiteral("Once you have a signal created, you see its representation in frequency domain in terms of magnitude and phase. Change of the visible area can be done by using mouse wheen and left button. The left button can be also used to move one of the points, the effect is immediately displayed in the filtered graph in the lower right corner. The frequency of the point you are currently pointing at with the mouse is displayed in the upper right corner.");
    QString sixthParagraph = QStringLiteral("Besides editing single points, you can apply a filter on the graph using Filters menu. Several fitlers are available, namely ideal, Gaussian and Butterworth, in both low-pass and high-pass variants.");
    QString seventhParagraph = QStringLiteral("Everytime you edit the signal, you can go one step back using the Edit -> undo option, or revert all changes to go back to the signal which is currently displayed in the original signal graph.");
    QString eightParagraph = QStringLiteral("The application provides some further displaying options and possibility to choose different languages.");
    QString ninthParagraph = QStringLiteral("I hope you will enjoy the time spent with FTutor1D");
    QString tenthParagraph = QStringLiteral("J.B.");

    text->setHtml(htmlTextHeader + "<p>" + firstParagraph +
                  "</p>\n<p></p>\n<p>" + secondParagraph +
                  "</p>\n<p></p>\n<p>" + thirdParagraph +
                  "</p>\n<p></p>\n<p>" + fourthParagraph +
                  "</p>\n<p></p>\n<p>" + fifthParagraph +
                  "</p>\n<p></p>\n<p>" + sixthParagraph +
                  "</p>\n<p></p>\n<p>" + seventhParagraph +
                  "</p>\n<p></p>\n<p>" + eightParagraph +
                  "</p>\n<p></p>\n<p>" + ninthParagraph +
                  "</p>\n<p></p>\n<p>" + tenthParagraph +
                  "</p>" + htmlTextFooter);

}

void HelpDialog::setLocalizedTexts(const Translation* language)
{
    if(language == nullptr)
    {
        setDefaultTexts();
        return;
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
    QString thirdParagraph = tr->getChildElementText(QStringLiteral("thirdParagraph"));
    QString fourthParagraph = tr->getChildElementText(QStringLiteral("fourthParagraph"));
    QString fifthParagraph = tr->getChildElementText(QStringLiteral("fifthParagraph"));
    QString sixthParagraph = tr->getChildElementText(QStringLiteral("sixthParagraph"));
    QString seventhParagraph = tr->getChildElementText(QStringLiteral("seventhParagraph"));
    QString eightParagraph = tr->getChildElementText(QStringLiteral("eightParagraph"));
    QString ninthParagraph = tr->getChildElementText(QStringLiteral("ninthParagraph"));
    QString tenthParagraph = tr->getChildElementText(QStringLiteral("tenthParagraph"));

// <br />
    text->setHtml(htmlTextHeader + "<p>" + firstParagraph +
                  "</p>\n<p></p>\n<p>" + secondParagraph +
                  "</p>\n<p></p>\n<p>" + thirdParagraph +
                  "</p>\n<p></p>\n<p>" + fourthParagraph +
                  "</p>\n<p></p>\n<p>" + fifthParagraph +
                  "</p>\n<p></p>\n<p>" + sixthParagraph +
                  "</p>\n<p></p>\n<p>" + seventhParagraph +
                  "</p>\n<p></p>\n<p>" + eightParagraph +
                  "</p>\n<p></p>\n<p>" + ninthParagraph +
                  "</p>\n<p></p>\n<p>" + tenthParagraph +
                  "</p>" + htmlTextFooter);
}
