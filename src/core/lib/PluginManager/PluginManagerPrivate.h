/*!
   \file PluginManagerPrivate.h
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

#ifndef CORE_PLUGINMANAGER_PLUGINMANAGERPRIVATE_H
#define CORE_PLUGINMANAGER_PLUGINMANAGER_H

#include <QtCore>

#include <Global.h>
#include <SettingManager/ISettingPageFactory.h>

namespace Core {
namespace PluginManager {

class PluginWrapper;
class PluginManager;
class IPlugin;

class PluginManagerPrivate :
        public QObject,
        public SettingManager::ISettingPageFactory
{
    Q_OBJECT
    Q_INTERFACES(Core::SettingManager::ISettingPageFactory)
    Q_DISABLE_COPY(PluginManagerPrivate)
    DECLARE_PUBLIC(PluginManager)

public:
    static PluginManagerPrivate *instance();
    ~PluginManagerPrivate();

    bool initialize();
    bool initialized();
    void shutdown();

    void loadPlugins();

    QObjectList allObjects() const;
    void addObject(QObject *object);
    bool delObject(QObject *object);

    template <typename Type> QList<Type *> getObjects() const {
        QList<Type *> list;
        Type *type = 0;
        foreach (QObject *object, m_Objects) {
            if( (type = qobject_cast<Type *>(object)) )
                list.append(type);
        }
        return list;
    }

    /* BEGIN ISettingPageFactory */
    QIcon settingPageIcon();
    QString settingPageName();
    int settingPagePriority();
    SettingManager::ISettingPage *createSettingPage();
    /* END ISettingPageFactory */

public slots:
    void pluginDialog();

protected:
    PluginManagerPrivate(PluginManager *parent);

    void readSettings();
    void writeSettings();

    void initializePlugins();

    void loadPlugins(QString pluginPath);
    void loadPlugin(QString fileName);

    PluginWrapper *findPlugin(QString name);
    static bool ascending(PluginWrapper *left, PluginWrapper *right);
    static bool descending(PluginWrapper *left, PluginWrapper *right);

private:

    bool m_Initialized;
    QList<PluginWrapper *> m_Plugins;
    QObjectList m_Objects;

    QStringList m_PluginPaths;
    bool m_PluginPathsOverride;

    static PluginManagerPrivate *m_Instance;

    friend class PluginSettingPage;
};

} // namespace PluginManager
} // namespace Core
#endif // CORE_PLUGINMANAGER_PLUGINMANAGER_H
