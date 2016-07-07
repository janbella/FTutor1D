#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include "localization.h"

#include <QDialog>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent, const Localization& localization);
    ~AboutDialog();

public slots:
    // nothing so far

private:
    Ui::AboutDialog *ui;

    void setLanguage(const Localization&  localization);

};

#endif // ABOUTDIALOG_H
