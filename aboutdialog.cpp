#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "localization.h"

#include <QImage>
#include <iostream>

AboutDialog::AboutDialog(QWidget *parent, const Localization& localization ) : QDialog(parent), ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    QImage img(":/resources/app_icon.png");

    QGraphicsScene* scene = new QGraphicsScene();
    scene->addPixmap(QPixmap::fromImage(img));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatioByExpanding);
    setLanguage(localization);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}


void AboutDialog::setLanguage(const Localization&  localization)
{
    ui->label_author->setText(localization("label_author","Author"));
    ui->label_credits->setText(localization("label_credits","Based on FTutor2D by Petr Sečkář"));
    ui->label_institution->setText(localization("label_institution","Faculty of Informatics, Masaryk University"));
}
