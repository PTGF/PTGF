/*!
   \file WelcomePlugin.cpp
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

#include "WelcomePlugin.h"

#include <CoreWindow/CoreWindow.h>
#include <PluginManager/PluginManager.h>
#include "WelcomeWidget.h"
#include "IWelcomeData.h"
#include "SettingPage.h"

namespace Plugins {
namespace Welcome {

WelcomePlugin::WelcomePlugin(QObject *parent) :
    QObject(parent)
{
    m_Name = "Welcome";
    m_Version = QString("%1.%2.%3").arg(VER_MAJ).arg(VER_MIN).arg(VER_PAT);
}

/* BEGIN IPlugin */
bool WelcomePlugin::initialize(QStringList &args, QString *err)
{
    Q_UNUSED(args)
    Q_UNUSED(err)

    try {
        using namespace Core;

        WelcomeWidget *welcomeWidget = new WelcomeWidget();

        CoreWindow::CoreWindow &coreWindow = CoreWindow::CoreWindow::instance();
        coreWindow.addCentralWidget(welcomeWidget, 0, tr("Welcome"), QIcon(":/Welcome/welcome.svg"));

        PluginManager::PluginManager &pluginManager = PluginManager::PluginManager::instance();
        pluginManager.addObject(this);
        pluginManager.addObject(welcomeWidget);

    } catch(...) {
        return false;
    }

    return true;
}

void WelcomePlugin::shutdown()
{
}

QString WelcomePlugin::name()
{
    return m_Name;
}

QString WelcomePlugin::version()
{
    return m_Version;
}

QList<Core::PluginManager::Dependency> WelcomePlugin::dependencies()
{
    return m_Dependencies;
}
/* END IPlugin */

/* BEGIN ISettingPageFactory */
QIcon WelcomePlugin::settingPageIcon()
{
    return QIcon(":/Welcome/welcome.svg");
}

QString WelcomePlugin::settingPageName()
{
    return tr("Welcome");
}

int WelcomePlugin::settingPagePriority()
{
    return 32;
}

Core::SettingManager::ISettingPage *WelcomePlugin::createSettingPage()
{
    return new SettingPage();
}
/* END ISettingPageFactory */



} // namespace Welcome
} // namespace Plugins

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN(Plugins::Welcome::WelcomePlugin)
#endif
