/*!
   \file NodeListViewPlugin.cpp
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


#include "NodeListViewPlugin.h"

#include <QTime>
#include <QtGlobal>
#include <QDebug>

#include <PluginManager/PluginManager.h>

#include "NodeListView.h"
#include "NodeRange.h"

namespace Plugins {
namespace NodeListView {

/*! \namespace Plugins::NodeListView
    \brief Contains the NodeListViewPlugin
 */

/*! \class NodeListViewPlugin
    \version 0.4.dev
    \brief NodeListViewPlugin

    \todo Document this more explicitly
 */

NodeListViewPlugin::NodeListViewPlugin()
{
    m_Name = "NodeListView";
    m_Version = QString("%1.%2.%3").arg(VER_MAJ).arg(VER_MIN).arg(VER_PAT);
}


NodeListViewPlugin::~NodeListViewPlugin()
{
}

bool NodeListViewPlugin::initialize(QStringList &args, QString *err)
{
    Q_UNUSED(args)
    Q_UNUSED(err)

    try {

        Core::PluginManager::PluginManager &pluginManager = Core::PluginManager::PluginManager::instance();
        pluginManager.addObject(this);

    } catch(...) {
        return false;
    }

    return true;
}

void NodeListViewPlugin::shutdown()
{
}

QString NodeListViewPlugin::name()
{
    return m_Name;
}

QString NodeListViewPlugin::version()
{
    return m_Version;
}

QList<Core::PluginManager::Dependency> NodeListViewPlugin::dependencies()
{
    return m_Dependencies;
}

} // namespace NodeListView
} // namespace Plugins

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN(Plugins::NodeListView::NodeListViewPlugin)
#endif
