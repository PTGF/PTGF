/*!
   \file NodeListViewPlugin.h
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

#ifndef NODELISTVIEWPLUGIN_H
#define NODELISTVIEWPLUGIN_H

#include <QObject>

#include <PluginManager/IPlugin.h>
#include <ViewManager/IViewFactory.h>

namespace Plugins {
namespace NodeListView {

class NodeListViewPlugin :
        public QObject,
        public Core::PluginManager::IPlugin,
        public Core::ViewManager::IViewFactory
{
    Q_OBJECT

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.krellinst.ptgf.NodeListViewPlugin")
#endif

    Q_INTERFACES(Core::PluginManager::IPlugin)
    Q_INTERFACES(Core::ViewManager::IViewFactory)

public:
    NodeListViewPlugin();

    /* IViewFactory Interface */
    QString viewName();
    bool viewHandles(QAbstractItemModel *model);
    QAbstractItemView *viewWidget(QAbstractItemModel *model);

    /* IPlugin Interface */
    ~NodeListViewPlugin();
    bool initialize(QStringList &args, QString *err);
    void shutdown();
    QString name();
    QString version();
    QList<Core::PluginManager::Dependency> dependencies();

protected:
    QString m_Name;
    QString m_Version;
    QList<Core::PluginManager::Dependency> m_Dependencies;

};

} // namespace NodeListView
} // namespace Plugins
#endif // NODELISTVIEWPLUGIN_H
