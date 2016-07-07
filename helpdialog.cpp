#include "helpdialog.h"
#include "ui_helpdialog.h"

HelpDialog::HelpDialog(QWidget *parent, const Localization& localization) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);
    setLanguage(localization);
}

HelpDialog::~HelpDialog()
{
    delete ui;
}

void HelpDialog::setLanguage(const Localization& localization)
{
    ui->helpDialogCloseButton->setText(localization("helpDialogCloseButton","Close"));
    ui->helpDialogOpenInPDFButton->setText(localization("helpDialogOpenInPDFButton","Open in PDF"));
    this->setWindowTitle(localization("HelpDialogTitle","Help"));

}
