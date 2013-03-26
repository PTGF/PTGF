/*!
   \file IPlugin.cpp
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

#include "IPlugin.h"

namespace Core {
namespace PluginManager {

/*! \struct Dependency
    \brief Object that describes a dependent object
    \param name A QString that contains the name of the dependent plugin.
    \param version A QString that contains a Regular Expression pattern that matches desired versions of the dependent plugin.
    \sa IPlugin
 */

/*! \fn Dependency::Dependency()
    \param name A QString that contains the name of the dependent plugin.
    \param version A QString that contains a Regular Expression pattern that matches desired versions of the dependent plugin.
 */


/*! \interface IPlugin
    \brief To be overriden as an interface for plugins

    pure virtual class
 */

/*! \fn IPlugin::IPlugin()
    \brief A plugin must have a default constructor.
 */

/*! \fn IPlugin::~IPlugin()
    \brief A plugin must have a destructor.
 */

/*! \fn IPlugin::initialize()
    \brief This method is called after the plugin has been instantiated and after the plugin's dependencies have been initialized.
    \param args QStringList containing required arguments
    \param err Returned errors in human-readable form
    \returns true if successful; false otherwise
 */

/*! \fn IPlugin::shutdown()
    \brief This method is called in the reverse order of the initialize
           method and notifies the plugin that we are about to shutdown
           --before it is destroyed.
 */

/*! \fn IPlugin::name()
    \returns QString containing the name of the plugin
 */

/*! \fn IPlugin::version()
    \returns QString containing the version of the plugin
 */

/*! \fn IPlugin::dependencies()
    \returns QList of Dependencies where each Dependency in the list contains the name
             and version of a parent plugin which must be initialized before this one
    \sa Core::PluginManager::Dependency
 */

} // namespace PluginManager
} // namespace Core

