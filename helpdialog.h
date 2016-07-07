#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include "localization.h"

#include <QDialog>

namespace Ui {
class HelpDialog;
}

class HelpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HelpDialog(QWidget *parent, const Localization& localization);
    ~HelpDialog();

private:
    Ui::HelpDialog *ui;
    void setLanguage(const Localization& localization);
};

#endif // HELPDIALOG_H
