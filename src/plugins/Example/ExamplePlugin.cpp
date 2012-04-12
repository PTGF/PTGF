/*!
   \file ExamplePlugin.cpp
   \author Dane Gardner <dane.gardner@gmail.com>
   \version

   \section LICENSE
   This file is part of the Parallel Tools GUI Framework (PTGF)
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


#include "ExamplePlugin.h"

namespace Plugins {
namespace Example {

/*! \namespace Plugins::Example
    \brief Contains the ExamplePlugin, which helps demonstrate the
           methodologies for creating a GUI plugin.
 */

/*! \class ExamplePlugin
    \version 0.1.dev
    \brief This is an example of the methods for producing a GUI plugin.

    \todo Document this more explicitly as a developer example.
 */

ExamplePlugin::ExamplePlugin()
{
    m_Name = "Example";
    m_Version = "0.1.dev";
}

ExamplePlugin::~ExamplePlugin()
{
}

bool ExamplePlugin::initialize(QStringList &args, QString *err)
{
    Q_UNUSED(args)
    Q_UNUSED(err)

    return true;
}

void ExamplePlugin::shutdown()
{
}

QString ExamplePlugin::name()
{
    return m_Name;
}

QString ExamplePlugin::version()
{
    return m_Version;
}

QList<Core::PluginManager::Dependency> ExamplePlugin::dependencies()
{
    return m_Dependencies;
}

} // namespace Example
} // namespace Plugins

Q_EXPORT_PLUGIN(Plugins::Example::ExamplePlugin)
