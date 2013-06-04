/*!
   \file TableViewPlugin.cpp
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

#include "TableViewPlugin.h"

#include <PluginManager/PluginManager.h>
#include <ViewManager/ViewManager.h>

#include "TableView.h"

namespace Plugins {
namespace TableView {

/*! \namespace Plugins::TableView
    \brief Contains the TableViewPlugin.
 */

/*! \class TableViewPlugin
    \version 0.3.dev
    \brief

    \par Depends on Plugins:

    \todo Document this more explicitly.
 */

TableViewPlugin::TableViewPlugin(QObject *parent) : QObject(parent)
{
    m_Name = "TableView";
    m_Version = QString("%1.%2.%3").arg(VER_MAJ).arg(VER_MIN).arg(VER_PAT);
    m_Dependencies.append( Core::PluginManager::Dependency("OpenSpeedShop", QString("^%1\\.%2.*$").arg(VER_MAJ).arg(VER_MIN)) );
}

TableViewPlugin::~TableViewPlugin()
{
}

bool TableViewPlugin::initialize(QStringList &args, QString *err)
{
    Q_UNUSED(args)
    Q_UNUSED(err)

    try {

        Core::PluginManager::PluginManager &pluginManager = Core::PluginManager::PluginManager::instance();
        pluginManager.addObject(this);                         /* Register ourselves as an IViewFactory */

    } catch(...) {
        return false;
    }

    return true;
}

void TableViewPlugin::shutdown()
{
}

QString TableViewPlugin::name()
{
    return m_Name;
}

QString TableViewPlugin::version()
{
    return m_Version;
}

QList<Core::PluginManager::Dependency> TableViewPlugin::dependencies()
{
    return m_Dependencies;
}

QString TableViewPlugin::viewName()
{
    return "Table View";
}

bool TableViewPlugin::viewHandles(QAbstractItemModel *model)
{
    Q_UNUSED(model)

    /* We can handle pretty much anything, so there's no checking needed */
    return true;
}

QAbstractItemView *TableViewPlugin::viewWidget(QAbstractItemModel *model)
{
    /* Check to see if we should even try to handle this model */
    if(!viewHandles(model)) {
        return NULL;
    }

    TableView *tableView = new TableView();
    tableView->setModel(model);
    return tableView;
}

} // namespace TableView
} // namespace Plugins

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN(Plugins::TableView::TableViewPlugin)
#endif
