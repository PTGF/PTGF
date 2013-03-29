/*!
   \file SettingManager.cpp
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

#include "SettingManager.h"
#include "SettingManagerPrivate.h"

#include <QMenuBar>
#include <QAction>

#include <CoreWindow/CoreWindow.h>
#include <PluginManager/PluginManager.h>

#include "SettingDialog.h"
#include "ISettingPageFactory.h"

#ifdef QT_DEBUG
    #include <QtDebug>
#endif

namespace Core {
namespace SettingManager {

/*!
   \class SettingManager
   \brief Manages persisted settings throughout the framework.
   singleton class

   \note Right now, this is just a wrapper for the QSettings object that comes
         with Qt4. In the future, I'd like to see something that is a bit more
         robust backing the settings in this project.
 */

/*!
   \fn SettingManager::instance()
   \brief Access to the singleton instance of this class
   \returns A pointer to the singleton instance of this class
 */
SettingManager &SettingManager::instance()
{
    static SettingManager m_Instance;
    return m_Instance;
}

/*!
   \fn SettingManager::SettingManager()
   \brief Constructor
   \internal
 */
SettingManager::SettingManager(QObject *parent) :
    QObject(parent),
    d(new SettingManagerPrivate(this))
{
}

/*!
   \fn SettingManager::~SettingManager()
   \brief Destructor
   \internal
 */
SettingManager::~SettingManager()
{
    d->m_Settings.sync();
}

bool SettingManager::initialize()
{
    if(d->m_Initialized) {
        return false;
    }

    try {

        CoreWindow::CoreWindow &coreWindow = CoreWindow::CoreWindow::instance();
        foreach(QAction *action, coreWindow.menuBar()->actions()) {
            if(action->text() == tr("Tools")) {
                QAction *settingDialog = new QAction(tr("Settings"), this);
                settingDialog->setToolTip(tr("Change application and plugin settings"));
                settingDialog->setShortcut(QKeySequence::Preferences);
                connect(settingDialog, SIGNAL(triggered()), this, SLOT(settingDialog()));
                action->menu()->addAction(settingDialog);
            }
        }

        /* Check the object pool for anything we should manage */
        PluginManager::PluginManager &pluginManager = PluginManager::PluginManager::instance();
        foreach(QObject *object, pluginManager.allObjects()) { d->pluginObjectRegistered(object); }
        connect(&pluginManager, SIGNAL(objectAdded(QObject*)), d, SLOT(pluginObjectRegistered(QObject*)));
        connect(&pluginManager, SIGNAL(objectRemoving(QObject*)), d, SLOT(pluginObjectDeregistered(QObject*)));

    } catch(...) {
        return false;
    }

    return d->m_Initialized = true;
}

void SettingManager::shutdown()
{
    if(!d->m_Initialized) {
        return;
    }

    // ...
}

/*!
   \fn SettingManager::setValue()
   \brief Sets the value of setting key to value. If the key already exists, the previous value is overwritten.
   \param key
   \param value
 */
void SettingManager::setValue(const QString &key, const QVariant &value)
{
    d->m_Settings.setValue(key, value);
}

/*!
   \fn SettingManager::value()
   \brief Returns the value for setting key. If the setting doesn't exist, returns defaultValue.
   \param key
   \param defaultValue
 */
QVariant SettingManager::value(const QString &key, const QVariant &defaultValue) const
{
    return d->m_Settings.value(key, defaultValue);
}

/*!
   \fn SettingManager::remove()
   \brief Removes the setting key and any sub-settings of key.
 */
void SettingManager::remove(const QString &key)
{
    d->m_Settings.remove(key);
}

/*!
   \fn SettingManager::contains()
   \brief Returns true if there exists a setting called key; returns false otherwise.
 */
bool SettingManager::contains(const QString &key) const
{
    return d->m_Settings.contains(key);
}

/*!
   \fn SettingManager::beginGroup()
   \brief Appends prefix to the current group.
   \param prefix
   \sa endGroup
   \sa group
 */
void SettingManager::beginGroup(const QString &prefix)
{
    d->m_Settings.beginGroup(prefix);
}

/*!
   \fn SettingManager::endGroup()
   \brief Resets the group to what it was before the corresponding beginGroup() call.
   \sa beginGroup
   \sa group
 */
void SettingManager::endGroup()
{
    d->m_Settings.endGroup();
}

/*!
   \fn SettingManager::group()
   \brief Returns the current group.
   \returns The current group
   \sa beginGroup
   \sa endGroup
 */
QString SettingManager::group() const
{
    return d->m_Settings.group();
}


void SettingManager::settingDialog()
{
    SettingDialog dialog(d->m_Pages, &CoreWindow::CoreWindow::instance());
    dialog.exec();
}







SettingManagerPrivate::SettingManagerPrivate(SettingManager *parent) :
    QObject(parent),
    q(parent),
    m_Initialized(false)
{
}

void SettingManagerPrivate::registerPageFactory(ISettingPageFactory *page)
{
    m_Pages.append(page);
}

void SettingManagerPrivate::deregisterPageFactory(ISettingPageFactory *page)
{
    if(m_Pages.contains(page)) {
        m_Pages.removeAll(page);
    }
}

void SettingManagerPrivate::pluginObjectRegistered(QObject *object)
{
    ISettingPageFactory *settingPageFactory = qobject_cast<ISettingPageFactory *>(object);
    if(settingPageFactory) registerPageFactory(settingPageFactory);
}

void SettingManagerPrivate::pluginObjectDeregistered(QObject *object)
{
    ISettingPageFactory *settingPageFactory = qobject_cast<ISettingPageFactory *>(object);
    if(settingPageFactory) deregisterPageFactory(settingPageFactory);
}


} // namespace SettingManager
} // namespace Core
