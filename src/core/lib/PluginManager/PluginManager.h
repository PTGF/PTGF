/*!
   \file PluginManager.h
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

#ifndef CORE_PLUGINMANAGER_PLUGINMANAGER_H
#define CORE_PLUGINMANAGER_PLUGINMANAGER_H

#include <QObject>

#include "IPlugin.h"
#include "PluginManagerLibrary.h"

namespace Core {
namespace PluginManager {

class PluginManagerPrivate;
class PluginSettingPage;

class PLUGINMANAGER_EXPORT PluginManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(PluginManager)
    DECLARE_PRIVATE(PluginManager)

public:
    static PluginManager &instance();
    ~PluginManager();

    bool initialize();
    void shutdown();

    void loadPlugins();

    QObjectList allObjects() const;
    void addObject(QObject *object);
    bool delObject(QObject *object);

    template <typename Type> QList<Type *> getObjects() const {
        QList<Type *> list;
        Type *type = 0;
        foreach (QObject *object, allObjects()) {
            if( (type = qobject_cast<Type *>(object)) )
                list.append(type);
        }
        return list;
    }

signals:
    void pluginLoaded(IPlugin *);
    void pluginInitialized(IPlugin *);
    void pluginShutdown(IPlugin *);
    void objectAdding(QObject *);
    void objectAdded(QObject *);
    void objectRemoving(QObject *);
    void objectRemoved(QObject *);

public slots:
    void pluginDialog();

protected:
    explicit PluginManager();

private:
    friend class PluginSettingPage;
};

} // namespace PluginManager
} // namespace Core



#endif // CORE_PLUGINMANAGER_PLUGINMANAGER_H
