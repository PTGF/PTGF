/*!
   \file WindowManager.cpp
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

#include "WindowManagerPrivate.h"

#include <QMenuBar>
#include <QDebug>

#include <CoreWindow/CoreWindow.h>
#include <ActionManager/ActionManager.h>
#include <PluginManager/PluginManager.h>

#include "IMainWindow.h"
#include "AboutDialog.h"

namespace Core {
namespace WindowManager {

/*! \class WindowManager
    \brief Manages the main window interface implementors
    \todo documentation
 */

WindowManager &WindowManager::instance()
{
    static WindowManager m_Instance;
    return m_Instance;
}

WindowManager::WindowManager() :
    QObject(0),
    d(new WindowManagerPrivate)
{
    d->q = this;

#ifndef QT_DEBUG
    AboutDialog::splash();
#endif
}

WindowManager::~WindowManager()
{
}

bool WindowManager::initialize()
{
    if(d->m_Initialized) {
        return false;
    }

    try {

        /*** Register our menu structure ***/

        ActionManager::ActionManager &actionManager = ActionManager::ActionManager::instance();
        ActionManager::MenuPath path("Help");
        d->m_AboutPage = actionManager.createAction(NULL, path);
        d->m_AboutPage->setText(tr("About"));
        d->m_AboutPage->setToolTip(tr("Displays the about dialog"));
        d->m_AboutPage->setIcon(QIcon(":/CoreWindow/app.png"));
        d->m_AboutPage->setIconVisibleInMenu(true);
        connect(d->m_AboutPage, SIGNAL(triggered()), d.data(), SLOT(aboutDialog()));

        /* Check the object pool for anything we should manage */
        PluginManager::PluginManager &pluginManager = PluginManager::PluginManager::instance();
        foreach(QObject *object, pluginManager.allObjects()) { d->pluginObjectRegistered(object); }
        connect(&pluginManager, SIGNAL(objectAdded(QObject*)), d.data(), SLOT(pluginObjectRegistered(QObject*)));
        connect(&pluginManager, SIGNAL(objectRemoving(QObject*)), d.data(), SLOT(pluginObjectDeregistered(QObject*)));

    } catch(...) {
        return false;
    }

    return d->m_Initialized = true;
}

void WindowManager::shutdown()
{
    if(!d->m_Initialized) {
        return;
    }

    // ...
}


/*! \internal
    \returns sorted list of all registered MainWindows
 */
QList<IMainWindow *> WindowManager::mainWindows()
{
    // Resort by priority
    if(d->m_MainWindows.count() > 1) {
        qSort(d->m_MainWindows.begin(), d->m_MainWindows.end(), d->ascending);
    }

    return d->m_MainWindows;
}

/*! \internal
    \returns sorted list of all registered about dialog widgets
 */
QList<QWidget *> WindowManager::aboutWidgets()
{
    QList<QWidget *> aboutWidgets;

    foreach(IMainWindow *mainWindow, mainWindows()) {
        QWidget *aboutWidget = mainWindow->createAboutWidget();
        if(aboutWidget && !aboutWidgets.contains(aboutWidget)) {
            aboutWidgets.append(aboutWidget);
        }
    }

    return aboutWidgets;
}





/***** PRIVATE IMPLEMENTATION *****/

WindowManagerPrivate::WindowManagerPrivate() :
    QObject(NULL),
    q(NULL),
    m_Initialized(false),
    m_AboutPage(NULL)
{
}

void WindowManagerPrivate::aboutDialog()
{
    try {
        AboutDialog aboutDialog;
        aboutDialog.exec();
    } catch(QString err) {
        qCritical() << tr("Failed to open about dialog: %1").arg(err);
    } catch(...) {
        qCritical() << tr("Failed to open about dialog.");
    }
}

void WindowManagerPrivate::pluginObjectRegistered(QObject *object)
{
    IMainWindow *mainWindow = qobject_cast<IMainWindow *>(object);
    if(mainWindow) registerMainWindow(mainWindow);
}

void WindowManagerPrivate::pluginObjectDeregistered(QObject *object)
{
    IMainWindow *mainWindow = qobject_cast<IMainWindow *>(object);
    if(mainWindow) deregisterMainWindow(mainWindow);
}

void WindowManagerPrivate::registerMainWindow(IMainWindow *window)
{
    m_MainWindows.append(window);

    connect(window, SIGNAL(active()), this, SLOT(windowActivated()));

    CoreWindow::CoreWindow &coreWindow = CoreWindow::CoreWindow::instance();

    coreWindow.setWindowTitle(window->mainWindowWidget()->windowTitle());
    coreWindow.setWindowIcon(window->mainWindowWidget()->windowIcon());

    /*** Set our main widget in the main window ***/
    coreWindow.addCentralWidget(window->mainWindowWidget(),
                                window->mainWindowPriority(),
                                window->mainWindowName(),
                                window->mainWindowIcon());


}

void WindowManagerPrivate::deregisterMainWindow(IMainWindow *window)
{
    if(m_MainWindows.contains(window)) {
        m_MainWindows.removeOne(window);
        disconnect(window, SIGNAL(active()), this, SLOT(windowActivated()));

        CoreWindow::CoreWindow &coreWindow = CoreWindow::CoreWindow::instance();
        coreWindow.removeCentralWidget(window->mainWindowWidget());
    }
}

void WindowManagerPrivate::windowActivated()
{
    IMainWindow *window = qobject_cast<IMainWindow *>(sender());

    if(!window) {
        return;
    }


    CoreWindow::CoreWindow &coreWindow = CoreWindow::CoreWindow::instance();
    coreWindow.setCurrentCentralWidget(window->mainWindowWidget());


    //! \todo Hide all windows' actions

    //! \todo Show the activated window's actions
}


/*! \fn WindowManagerPrivate::ascending()
    \internal
    \brief Used by qSort to sort the m_MainWindows QList in order of priority
    \sa qSort
 */
bool WindowManagerPrivate::ascending(IMainWindow *left, IMainWindow *right)
{
    return left->mainWindowPriority() < right->mainWindowPriority();
}


} // namespace WindowManager
} // namespace Core
