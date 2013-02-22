#include "AboutWidget.h"
#include "ui_AboutWidget.h"

#include <QtCore>

AboutWidget::AboutWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutWidget)
{
    ui->setupUi(this);

    QString aboutText;
    aboutText.append(tr("%1 v%2\n").
                     arg(QCoreApplication::applicationName()).
                     arg(QCoreApplication::applicationVersion()));
    aboutText.append(tr("Built on %1 at %2\n").arg(__DATE__).arg(__TIME__));
    aboutText.append(tr("Copyright 2010-%1 Argo Navis Technologies LLC.  All rights reserved.\n").arg(QDate::currentDate().year()));
    ui->lblAbout->setText(aboutText);
}

AboutWidget::~AboutWidget()
{
    delete ui;
}
