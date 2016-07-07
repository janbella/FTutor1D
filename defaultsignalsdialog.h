#ifndef DEFAULTSIGNALSDIALOG_H
#define DEFAULTSIGNALSDIALOG_H

#include <QMainWindow>

namespace Ui {
class DefaultSignalsDialog;
}

class DefaultSignalsDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit DefaultSignalsDialog(QWidget *parent = 0);
    ~DefaultSignalsDialog();

private:
    Ui::DefaultSignalsDialog *ui;
};

#endif // DEFAULTSIGNALSDIALOG_H
