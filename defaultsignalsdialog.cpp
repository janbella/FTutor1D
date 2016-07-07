#include "defaultsignalsdialog.h"
#include "ui_defaultsignalsdialog.h"

DefaultSignalsDialog::DefaultSignalsDialog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DefaultSignalsDialog)
{
    ui->setupUi(this);
}

DefaultSignalsDialog::~DefaultSignalsDialog()
{
    delete ui;
}
