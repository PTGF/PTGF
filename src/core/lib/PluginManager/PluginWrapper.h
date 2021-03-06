/*!
   \file PluginWrapper.h
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

#ifndef CORE_PLUGINMANAGER_PLUGINWRAPPER_H
#define CORE_PLUGINMANAGER_PLUGINWRAPPER_H


//
//  W A R N I N G
//  -------------
//
// This file is not part of the public PTGF API.  This header file may change
// from version to version without notice, or even be removed.
//


#include <QObject>
#include <QString>
#include <QList>
#include <QStringList>

#include "IPlugin.h"

namespace Core {
namespace PluginManager {

class PluginWrapper : public QObject, public IPlugin {
    Q_OBJECT
    Q_INTERFACES(Core::PluginManager::IPlugin)

    Q_PROPERTY(int priority READ priority WRITE setPriority STORED true)

public:

    enum States {
        State_Error,
        State_Loaded,
        State_Initialized,
        State_Shutdown
    };

    PluginWrapper(IPlugin *plugin, QString filePath, QObject *parent = 0);
    ~PluginWrapper();

    States state();

    int priority();
    void setPriority(int priority);

    /* IPlugin interface */
    bool initialize(QStringList &args, QString *err);
    void shutdown();
    QString name();
    QString version();
    QList<Dependency> dependencies();

protected:
    QString m_FilePath;
    IPlugin *m_Plugin;
    States m_State;
    int m_Priority;
};


} // namespace PluginManager
} // namespace Core
#endif // CORE_PLUGINMANAGER_PLUGINWRAPPER_H
