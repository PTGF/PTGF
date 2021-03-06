/*!
   \file SettingDialog.cpp
   \author Dane Gardner <dane.gardner@gmail.com>

   \section LICENSE
   This file is part of the Parallel Tools GUI Framework (PTGF)
   Copyright (C) 2010-2015 Argo Navis Technologies, LLC

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

#include "SettingDialog.h"
#include "ui_SettingDialog.h"

#include <QPushButton>

#include "ISettingPage.h"
#include "SettingManager.h"

namespace Core {
namespace SettingManager {

SettingDialog::SettingDialog(QList<ISettingPageFactory *> pages, QWidget *parent) :
    QDialog(parent),
    m_Pages(pages),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);

    readSettings();

    // Connect the listWidget selections with the displayed page
    connect(ui->listWidget, SIGNAL(currentRowChanged(int)),
            ui->stackedWidget, SLOT(setCurrentIndex(int)));

    // Get the buttons in the button box, and wire them up
    QPushButton *btnOkay = ui->buttonBox->button(QDialogButtonBox::Ok);
    connect(btnOkay, SIGNAL(clicked()), this, SLOT(accept()));
    QPushButton *btnCancel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
    QPushButton *btnApply = ui->buttonBox->button(QDialogButtonBox::Apply);
    QPushButton *btnReset = ui->buttonBox->button(QDialogButtonBox::Reset);

    // Honor priority in the page listing
    qSort(m_Pages.begin(), m_Pages.end(), ascending);

    // Clear previous pages
    while(ui->stackedWidget->count()) {
        ui->stackedWidget->removeWidget(ui->stackedWidget->widget(0));
    }

    // Add the pages to the lists, after initializing them
    for(int i = 0; i < m_Pages.count(); i++) {
        ISettingPageFactory *factory = m_Pages.at(i);

        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
        item->setText(factory->settingPageName());
        item->setIcon(factory->settingPageIcon());

        ISettingPage *page = factory->createSettingPage();
        connect(btnOkay, SIGNAL(clicked()), page, SLOT(apply()));
        connect(btnApply, SIGNAL(clicked()), page, SLOT(apply()));
        connect(btnReset, SIGNAL(clicked()), page, SLOT(reset()));

        ui->stackedWidget->addWidget( qobject_cast<QWidget *>(page) );
    }

    ui->listWidget->setCurrentRow(0);
}

SettingDialog::~SettingDialog()
{
    writeSettings();

    delete ui;
}

void SettingDialog::readSettings()
{
    SettingManager &settingManager = SettingManager::instance();

    settingManager.setGroup("SettingManager/SettingDialog");

    resize( settingManager.value("WindowSize", size()).toSize() );
    move( settingManager.value("WindowPosition", pos()).toPoint() );

    settingManager.endGroup();
}

void SettingDialog::writeSettings()
{
    SettingManager &settingManager = SettingManager::instance();

    settingManager.setGroup("SettingManager/SettingDialog");

    settingManager.setValue("WindowSize", size());
    settingManager.setValue("WindowPosition", pos());

    settingManager.endGroup();
}

bool SettingDialog::ascending(ISettingPageFactory *left, ISettingPageFactory *right)
{
    return left->settingPagePriority() < right->settingPagePriority();
}

} // namespace SettingManager
} // namespace Core
