#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include <QImage>
#include <iostream>

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    QImage img(":/resources/app_icon.png");

    QGraphicsScene* scene = new QGraphicsScene();
    scene->addPixmap(QPixmap::fromImage(img));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatioByExpanding);

}

AboutDialog::~AboutDialog()
{
    delete ui;
}

