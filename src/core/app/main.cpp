/*!
   \file main.cpp
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

/*! \mainpage Parallel Tool GUI Framework (PTGF)
    \author Dane Gardner <dane.gardner@gmail.com>
    \version 0.4.dev

    \section intro Introduction
    \par
    The Parallel Tools GUI Framework project is a multi-platform graphical-user-interface
    originally designed for the Open|SpeedShop framework using the QT4 toolkit, and is
    currently under development by Argo Navis Technologies LLC.
    \par
    For those who are using this documentation to learn the project's development
    process, you might want to start with the CoreWindow object.

    \section releases Releases
    \par
    None yet.

    \section copyright Copyright
    \par
    This file is part of the Parallel Tools GUI Framework (PTGF)
    Copyright (C) 2010-2013 Argo Navis Technologies, LLC
    \par
    This library is free software; you can redistribute it and/or modify it
    under the terms of the GNU Lesser General Public License as published by the
    Free Software Foundation; either version 2.1 of the License, or (at your
    option) any later version.
    \par
    This library is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
    for more details.
    \par
    You should have received a copy of the GNU Lesser General Public License
    along with this library; if not, write to the Free Software Foundation,
    Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

 */


#include <QApplication>

#include <NotificationManager/NotificationManager.h>
#include <SettingManager/SettingManager.h>
#include <PluginManager/PluginManager.h>
#include <CoreWindow/CoreWindow.h>
#include <ActionManager/ActionManager.h>
#include <WindowManager/WindowManager.h>

#ifdef MAIN_DEBUG
#  include <QDebug>
#endif

#include <QDebug>

int main(int argc, char *argv[])
{
    int retval = 0;

#ifdef MAIN_DEBUG
    try {
#endif

    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Krell Institute");
    QCoreApplication::setOrganizationDomain("krellinst.org");
    QCoreApplication::setApplicationName("Parallel Tools GUI Framework");
    QCoreApplication::setApplicationVersion(QString("%1.%2.%3").arg(VER_MAJ).arg(VER_MIN).arg(VER_PAT));

#ifdef MAIN_DEBUG
    qDebug() << __FILE__ << __LINE__ << "\tInstantiating the singleton classes";
#endif

    using namespace Core;

    Core::NotificationManager::NotificationManager &notificationManager = Core::NotificationManager::NotificationManager::instance();
    SettingManager::SettingManager &settingManager = SettingManager::SettingManager::instance();
    CoreWindow::CoreWindow &coreWindow = CoreWindow::CoreWindow::instance();
    PluginManager::PluginManager &pluginManager = PluginManager::PluginManager::instance();
    WindowManager::WindowManager &windowManager = WindowManager::WindowManager::instance();
    ActionManager::ActionManager &actionManager = ActionManager::ActionManager::instance();

#ifdef MAIN_DEBUG
    qDebug() << __FILE__ << __LINE__ << "\tInitializing the singleton classes";
#endif
    notificationManager.initialize();
    settingManager.initialize();

    if(!coreWindow.initialized())
        coreWindow.initialize();

    pluginManager.initialize();
    actionManager.initialize();
    windowManager.initialize();


#ifdef MAIN_DEBUG
    qDebug() << __FILE__ << __LINE__ << "\tLoading plugins";
#endif

    pluginManager.loadPlugins();

#ifdef MAIN_DEBUG
    qDebug() << __FILE__ << __LINE__ << "\tShowing the CoreWindow";
#endif

    coreWindow.show();
    retval = a.exec();

#ifdef MAIN_DEBUG
    qDebug() << __FILE__ << __LINE__ << "\tShutting down singleton classes";
#endif
    actionManager.shutdown();
    windowManager.shutdown();
    pluginManager.shutdown();

    if(coreWindow.initialized())
        coreWindow.shutdown();

    settingManager.shutdown();
    notificationManager.shutdown();


#ifdef MAIN_DEBUG
    qDebug() << __FILE__ << __LINE__ << "\tDone";
#endif

#ifdef MAIN_DEBUG
    } catch(QString err) {
        qDebug() << __FILE__ << __LINE__ << "\tCaught error:" << err;
        retval = 1;
    } catch(...) {
        qDebug() << __FILE__ << __LINE__ << "\tCaught error";
        retval = 1;
    }
#endif

    return retval;
}
