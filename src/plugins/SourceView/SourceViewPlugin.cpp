/*!
   \file SourceViewPlugin.cpp
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

#include "SourceViewPlugin.h"

#include <PluginManager/PluginManager.h>
#include "SourceView.h"

namespace Plugins {
namespace SourceView {

/*! \namespace Plugins::SourceView
    \brief Contains the SourceViewPlugin.
 */

/*! \class SourceViewPlugin
    \version 0.4.dev
    \brief

    \todo Document this more explicitly.
 */

/*!
   \fn SourceViewPlugin::SourceViewPlugin()
   \brief Constructor.
 */
SourceViewPlugin::SourceViewPlugin(QObject *parent) :
    QObject(parent)
{
    m_Name = "SourceView";
    m_Version = QString("%1.%2.%3").arg(VER_MAJ).arg(VER_MIN).arg(VER_PAT);
}

/*!
   \fn SourceViewPlugin::~SourceViewPlugin()
   \brief Destructor.
 */
SourceViewPlugin::~SourceViewPlugin()
{
}

/*!
   \fn SourceViewPlugin::initialize()
   \brief
   \returns true if successful
 */
bool SourceViewPlugin::initialize(QStringList &args, QString *err)
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

/*!
   \fn SourceViewPlugin::shutdown()
   \brief
 */
void SourceViewPlugin::shutdown()
{
}

/*!
   \fn SourceViewPlugin::name()
   \brief
   \returns name of plugin
 */
QString SourceViewPlugin::name()
{
    return m_Name;
}

/*!
   \fn SourceViewPlugin::version()
   \brief
   \returns version of plugin
 */
QString SourceViewPlugin::version()
{
    return m_Version;
}

/*!
   \fn SourceViewPlugin::dependencies()
   \brief
   \returns list of dependecies
 */
QList<Core::PluginManager::Dependency> SourceViewPlugin::dependencies()
{
    return m_Dependencies;
}

SourceView *SourceViewPlugin::sourceViewWidget(const QString &text)
{
    SourceView *view = new SourceView();
    view->setPlainText(text);
    return view;
}

} // namespace SourceView
} // namespace Plugins

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN(Plugins::SourceView::SourceViewPlugin)
#endif
