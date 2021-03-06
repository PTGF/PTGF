/*!
   \file IPlugin.h
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

#ifndef CORE_PLUGINMANAGER_IPLUGIN_H
#define CORE_PLUGINMANAGER_IPLUGIN_H

#include <QString>
#include <QList>
#include <QtPlugin>

#include "PluginManagerLibrary.h"

namespace Core {
namespace PluginManager {

struct PLUGINMANAGER_EXPORT Dependency {
    QString name;
    QString version;

    Dependency(QString name, QString version) {
        this->name = name;
        this->version = version;
    }
};


class PLUGINMANAGER_EXPORT IPlugin
{
public:
    IPlugin() {}
    virtual ~IPlugin() {}

    virtual bool initialize(QStringList &args, QString *err) = 0;
    virtual void shutdown() = 0;

    virtual QString name() = 0;
    virtual QString version() = 0;
    virtual QList<Dependency> dependencies() = 0;
};

} // namespace PluginManager
} // namespace Core


Q_DECLARE_INTERFACE(Core::PluginManager::IPlugin, "org.krellinst.ptgf.IPlugin")

#endif // CORE_PLUGINMANAGER_IPLUGIN_H
