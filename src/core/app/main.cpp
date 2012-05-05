/*!
   \file main.cpp
   \author Dane Gardner <dane.gardner@gmail.com>
   \version 0.1.dev

   \section LICENSE
   This file is part of the Parallel Tools GUI Framework (PTGF)
   Copyright (C) 2010-2011 Argo Navis Technologies, LLC

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

   \section DESCRIPTION

 */

/*! \mainpage Parallel Tool GUI Framework (PTGF)
    \author Dane Gardner <dane.gardner@gmail.com>
    \version 0.1.dev

    \section intro Introduction
    \par
    The Parallel Tools GUI Framework project is a multi-platform graphical-user-interface
    originally designed for the Open|SpeedShop framework using the QT4 toolkit, and is
    currently under development by Argo Navis Technologies LLC.
    \par
    For those who are using this documentation to learn the project's development
    process, you might want to start with the MainWindow object.

    \section releases Releases
    \par
    None yet.

    \section copyright Copyright
    \par
    This file is part of the Parallel Tools GUI Framework (PTGF)
    Copyright (C) 2010-2011 Argo Navis Technologies, LLC
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


#include <QtGui/QApplication>

#include <SettingManager/SettingManager.h>
#include <PluginManager/PluginManager.h>
#include <MainWindow/MainWindow.h>

#ifdef MAIN_DEBUG
#  include <QDebug>
#endif



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
    QCoreApplication::setApplicationVersion("0.1");

#ifdef MAIN_DEBUG
    qDebug() << __FILE__ << __LINE__ << "\tInstantiating the singleton classes";
#endif

    using namespace Core;
    SettingManager::SettingManager &settingManager = SettingManager::SettingManager::instance();
    MainWindow::MainWindow &mainWindow = MainWindow::MainWindow::instance();
    PluginManager::PluginManager &pluginManager = PluginManager::PluginManager::instance();

#ifdef MAIN_DEBUG
    qDebug() << __FILE__ << __LINE__ << "\tInitializing the singleton classes";
#endif

    if(!settingManager.initialized())
        settingManager.initialize();
    if(!mainWindow.initialized())
        mainWindow.initialize();
    if(!pluginManager.initialized())
        pluginManager.initialize();

#ifdef MAIN_DEBUG
    qDebug() << __FILE__ << __LINE__ << "\tLoading plugins";
#endif

    pluginManager.loadPlugins();

#ifdef MAIN_DEBUG
    qDebug() << __FILE__ << __LINE__ << "\tShowing the MainWindow";
#endif

    mainWindow.show();
    retval = a.exec();

#ifdef MAIN_DEBUG
    qDebug() << __FILE__ << __LINE__ << "\tShutting down singleton classes";
#endif

    if(pluginManager.initialized())
        pluginManager.shutdown();
    if(mainWindow.initialized())
        mainWindow.shutdown();
    if(settingManager.initialized())
        settingManager.shutdown();

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
