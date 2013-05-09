/*!
   \file SettingPage.cpp
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

#include "PluginSettingPage.h"
#include "ui_PluginSettingPage.h"

#include <QDebug>
#include <QFileDialog>

#include <SettingManager/SettingManager.h>
#include "PluginManagerPrivate.h"

#include "PluginWrapper.h"

namespace Core {
namespace PluginManager {

#ifdef WIN32
    static const QString m_PathSep = ";";
#else
    static const QString m_PathSep = ":";
#endif

PluginSettingPage::PluginSettingPage(QList<PluginWrapper *> plugins, QWidget *parent) :
    SettingManager::ISettingPage(parent),
    ui(new Ui::PluginSettingPage)
{
    ui->setupUi(this);

    readSettings();

    /* This constructor is called from the PluginManager as a dialog. We need
       to disable editing of the settings from here */
    ui->txtPluginPath->setEnabled(false);
    ui->lblPluginPath->setEnabled(false);
    ui->btnPluginPath->setVisible(false);

    buildTree(plugins);
    reset();
}

PluginSettingPage::PluginSettingPage(QWidget *parent) :
    SettingManager::ISettingPage(parent),
    ui(new Ui::PluginSettingPage)
{
    ui->setupUi(this);

    readSettings();

    buildTree( PluginManager::instance().d->m_Plugins );
    reset();
}

PluginSettingPage::~PluginSettingPage()
{
    writeSettings();

    delete ui;
}

void PluginSettingPage::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
void PluginSettingPage::apply()
{
    SettingManager::SettingManager &settingManager = SettingManager::SettingManager::instance();

    settingManager.beginGroup("PluginManager");

    // Store user settable settings
    if(!PluginManager::instance().d->m_PluginPathsOverride) {
        PluginManager::instance().d->m_PluginPaths = ui->txtPluginPath->text().split(m_PathSep);
        settingManager.setValue("PluginPath", ui->txtPluginPath->text());
    }

    settingManager.endGroup();
}

void PluginSettingPage::reset()
{
    SettingManager::SettingManager &settingManager = SettingManager::SettingManager::instance();
    settingManager.beginGroup("PluginManager");

    // Restore user settable settings
    ui->txtPluginPath->setText( PluginManager::instance().d->m_PluginPaths.join(m_PathSep) );

    if(PluginManager::instance().d->m_PluginPathsOverride) {
        ui->txtPluginPath->setEnabled(false);
        ui->txtPluginPath->setToolTip(tr("Environment variable or commandline argument was used, and cannot be modified here"));
    }

    settingManager.endGroup();
}

void PluginSettingPage::readSettings()
{
    SettingManager::SettingManager &settingManager = SettingManager::SettingManager::instance();
    settingManager.beginGroup("PluginManager");
    settingManager.beginGroup("SettingPage");

    //! todo Restore tree state
    resize( settingManager.value("WindowSize", size()).toSize() );
    move( settingManager.value("WindowPosition", pos()).toPoint() );

    settingManager.endGroup();
    settingManager.endGroup();
}

void PluginSettingPage::writeSettings()
{
    SettingManager::SettingManager &settingManager = SettingManager::SettingManager::instance();

    settingManager.beginGroup("PluginManager");
    settingManager.beginGroup("SettingPage");

    //! \todo Store tree state
    settingManager.setValue("WindowSize", size());
    settingManager.setValue("WindowPosition", pos());

    settingManager.endGroup();
    settingManager.endGroup();
}

void PluginSettingPage::buildTree(QList<PluginWrapper *> plugins)
{
    QStringList headers;
    headers.append("Name");
    ui->treeWidget->setColumnWidth(0, 250);
    headers.append("Version");
    ui->treeWidget->setColumnWidth(1, 50);
    ui->treeWidget->setHeaderLabels(headers);
    ui->treeWidget->setHeaderHidden(false);

    // Plugins should already be in the proper order after initialization

    foreach(PluginWrapper *plugin, plugins) {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
        item->setText(0, plugin->name());
        item->setText(1, plugin->version());

        switch(plugin->state()) {
            case PluginWrapper::State_Loaded:
                item->setToolTip(0, tr("Plugin loaded"));
                item->setIcon(0, QIcon(":/PluginManager/check.svg"));
                break;
            case PluginWrapper::State_Initialized:
                item->setToolTip(0, tr("Plugin initialized"));
                item->setIcon(0, QIcon(":/PluginManager/check.svg"));
                break;
            case PluginWrapper::State_Error:
                item->setToolTip(0, tr("Plugin failed"));
                item->setIcon(0, QIcon(":/PluginManager/fail.svg"));
                break;
            case PluginWrapper::State_Shutdown:
                item->setToolTip(0, tr("Plugin shutdown"));
                item->setIcon(0, QIcon(":/PluginManager/fail.svg"));
                break;
            default:
                item->setToolTip(0, tr("Unknown status"));
                item->setIcon(0, QIcon(":/PluginManager/fail.svg"));
                break;
        }

        m_Plugins.append(item);
    }
}


void PluginSettingPage::on_btnPluginPath_clicked()
{
    QString filePath = QFileDialog::getExistingDirectory(this, tr("Use Plugin Path"), qApp->applicationDirPath());
    if(!filePath.isEmpty()) {
        ui->txtPluginPath->setText(filePath);
    }
}


} // namespace PluginManager
} // namespace Core
