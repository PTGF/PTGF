/*!
   \file AboutWidget.cpp
   \author Dane Gardner <dane.gardner@gmail.com>

   \section LICENSE
   This file is part of the Parallel Tools GUI Framework (PTGF)
   Copyright (C) 2010-2013 Argo Navis Technologies, LLC

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU Lesser General Public License as published by the
   Free Software Foundation; either version 2.1 of the License, or (at your
   option) any later version.

   This library is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
   for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this library; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "AboutWidget.h"
#include "ui_AboutWidget.h"

#include <QDate>

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
