/*!
   \file SourceViewPlugin.cpp
   \author Dane Gardner <dane.gardner@gmail.com>

   \section LICENSE
   This file is part of the Parallel Tools GUI Framework (PTGF)
   Copyright (C) 2010-2012 Argo Navis Technologies, LLC

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

#ifndef SOURCEVIEWPLUGINasdf_H
#define SOURCEVIEWPLUGINasdf_H

#include <QtCore>
#include <QtGui>

#include <PluginManager/IPlugin.h>

#include "ISourceViewFactory.h"

namespace Plugins {
namespace SourceView {

class SourceViewPlugin :
        public QObject,
        public Core::PluginManager::IPlugin,
        public Plugins::SourceView::ISourceViewFactory
{
    Q_OBJECT
    Q_INTERFACES(Core::PluginManager::IPlugin)
    Q_INTERFACES(Plugins::SourceView::ISourceViewFactory)

public:
    SourceViewPlugin(QObject *parent = 0);

    /* IPlugin Interface */
    ~SourceViewPlugin();
    bool initialize(QStringList &args, QString *err);
    void shutdown();
    QString name();
    QString version();
    QList<Core::PluginManager::Dependency> dependencies();

    /* ISourceViewFactory Interface */
    SourceView *sourceViewWidget(const QString &text);

protected:
    QString m_Name;
    QString m_Version;
    QList<Core::PluginManager::Dependency> m_Dependencies;

};

} // namespace SourceView
} // namespace Plugins

#endif // SOURCEVIEWPLUGIN_H
