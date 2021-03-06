/*!
   \file CoreSettingPage.cpp
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

#include "CoreSettingPage.h"
#include "ui_CoreSettingPage.h"

#include <QStyleFactory>
#include <QFileDialog>

#include <SettingManager/SettingManager.h>

namespace Core {
namespace CoreWindow {

/*!
   \fn Core::CoreWindow::CoreSettingPage
   \brief This is the core page for the GUI framework.
   Any general user-settable settings for the framework should be handled here.
 */


/*!
   \fn CoreSettingPage::CoreSettingPage()
   \brief Constructor
 */
CoreSettingPage::CoreSettingPage(QWidget *parent) :
    SettingManager::ISettingPage(parent),
    ui(new Ui::CoreSettingPage)
{
    ui->setupUi(this);

    m_OriginalStyle = QApplication::style()->objectName();
    foreach(QString style, QStyleFactory::keys()) {
        ui->cmbStyle->addItem(style);
    }

    ui->cmbToolbarIcons->insertItem(0, "Large", 32);
    ui->cmbToolbarIcons->insertItem(1, "Medium", 16);
    ui->cmbToolbarIcons->insertItem(2, "Small", 12);
    ui->cmbToolbarIcons->setCurrentIndex(1);

    reset();
}

/*! \reimp */
CoreSettingPage::~CoreSettingPage()
{
    // Reset the style if necessary
    if(QApplication::style()->objectName().compare(m_OriginalStyle, Qt::CaseInsensitive))
        QApplication::setStyle(QStyleFactory::create(m_OriginalStyle));

    delete ui;
}

/*! \reimp */
void CoreSettingPage::apply()
{
    SettingManager::SettingManager &settingManager = SettingManager::SettingManager::instance();
    settingManager.setGroup("CoreWindow");

    m_OriginalStyle = QApplication::style()->objectName();
    settingManager.setValue("style", m_OriginalStyle);

    settingManager.setValue("styleSheet", ui->txtStylesheet->text());

    settingManager.endGroup();
}

/*! \reimp */
void CoreSettingPage::reset()
{
    SettingManager::SettingManager &settingManager = SettingManager::SettingManager::instance();

    settingManager.setGroup("CoreWindow");

    QString style = settingManager.value("style", m_OriginalStyle).toString();
    int index = ui->cmbStyle->findText(style, Qt::MatchFixedString);
    ui->cmbStyle->setCurrentIndex(index);

    ui->txtStylesheet->setText(settingManager.value("styleSheet", QString()).toString());

    settingManager.endGroup();
}

/*!
   \fn CoreSettingPage::on_cmbStyle_currentIndexChanged()
   \brief Slot that handles changes in the style combo box.
 */
void CoreSettingPage::on_cmbStyle_currentIndexChanged(QString style)
{
    // Ensure we're actually changing the style
    if(!QApplication::style()->objectName().compare(ui->cmbStyle->currentText(), Qt::CaseInsensitive))
        return;

    // Set the style to the new, selected one
    QApplication::setStyle(QStyleFactory::create(style));
}

void CoreSettingPage::on_btnStylesheet_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    tr("Open Stylesheet"),
                                                    QApplication::instance()->applicationDirPath(),
                                                    tr("Stylesheets (*.css)"));
    if(!filePath.isEmpty()) {
        ui->txtStylesheet->setText(filePath);
    }
}



} // namespace CoreWindow
} // namespace Core
