/*!
   \file ProcessListPlugin.cpp
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

#include "ProcessListPlugin.h"

#include <PluginManager/PluginManager.h>

namespace Plugins {
namespace ProcessList {

ProcessListPlugin::ProcessListPlugin(QObject *parent) :
    QObject(parent)
{
    m_Name = "ProcessList";
    m_Version = QString("%1.%2.%3").arg(VER_MAJ).arg(VER_MIN).arg(VER_PAT);
}

ProcessListPlugin::~ProcessListPlugin()
{
}

bool ProcessListPlugin::initialize(QStringList &args, QString *err)
{
    Q_UNUSED(args)
    Q_UNUSED(err)

    try {

        using namespace Core;
        PluginManager::PluginManager &pluginManager = PluginManager::PluginManager::instance();
        pluginManager.addObject(this);

    } catch(...) {
        return false;
    }

    return true;
}

void ProcessListPlugin::shutdown()
{
}

QString ProcessListPlugin::name()
{
    return m_Name;
}

QString ProcessListPlugin::version()
{
    return m_Version;
}

QList<Core::PluginManager::Dependency> ProcessListPlugin::dependencies()
{
    return m_Dependencies;
}

} // namespace ProcessList
} // namespace Plugins

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN(Plugins::ProcessList::ProcessListPlugin)
#endif
