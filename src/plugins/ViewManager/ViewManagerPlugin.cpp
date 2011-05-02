/*!
   \file ViewManagerPlugin.cpp
   \author Dane Gardner <dane.gardner@gmail.com>
   \version 

   \section LICENSE
   This file is part of the Open|SpeedShop Graphical User Interface
   Copyright (C) 2010-2011 Argo Navis Technologies, LLC

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

   \section DESCRIPTION

 */

#include "ViewManagerPlugin.h"

namespace Plugins {
namespace ViewManager {

/*! \namespace Plugins::ViewManager
    \brief Implements the basis for the model-view pattern.

    The ViewManager namespace contains all of the base classes necessary to
    implement the model-view design pattern that we are following in this
    project. It is loaded as a plugin, and is dependent upon the
    Open|SpeedShop plugin.
 */

/*! \class ViewManagerPlugin
    \version 0.1.dev
    \brief Class which allows for the dynamic loading of the ViewManager
           namespace.

    \par Depends on Plugins:
         OpenSpeedShop

    \sa DataItem, DataModel
 */

ViewManagerPlugin::ViewManagerPlugin(QObject *parent) :
    QObject(parent)
{
    m_Name = "ViewManager";
    m_Version = "0.1.dev";
    m_Dependencies.append( Dependency("OpenSpeedShop", "^0\\.1.*$") );
}

ViewManagerPlugin::~ViewManagerPlugin()
{
}

bool ViewManagerPlugin::initialize(QStringList &args, QString *err)
{
    return true;
}

void ViewManagerPlugin::shutdown()
{
}

QString ViewManagerPlugin::name() { return m_Name; }

QString ViewManagerPlugin::version() { return m_Version; }

QList<Dependency> ViewManagerPlugin::dependencies() { return m_Dependencies; }

Q_EXPORT_PLUGIN(Plugins::ViewManager::ViewManagerPlugin)

} // namespace ViewManager
} // namespace Plugins