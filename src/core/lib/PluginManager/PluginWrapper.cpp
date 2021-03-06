/*!
   \file PluginWrapper.cpp
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

#include "PluginWrapper.h"

#ifdef QT_DEBUG
  #include <QDebug>
#endif

namespace Core {
namespace PluginManager {

PluginWrapper::PluginWrapper(IPlugin *plugin, QString filePath, QObject *parent) :
        QObject(parent)
{
    if(!plugin)
        throw QString(tr("Cannot wrap null plugin pointer"));
    if(filePath.isEmpty())
        throw QString(tr("Cannot wrap empty filePath"));

    m_Plugin = plugin;
    m_FilePath = filePath;
    m_State = State_Loaded;
}

PluginWrapper::~PluginWrapper()
{
    delete m_Plugin;
}

PluginWrapper::States PluginWrapper::state()
{
    return m_State;
}

bool PluginWrapper::initialize(QStringList &args, QString *err)
{
    bool retVal = m_Plugin->initialize(args, err);
    m_State = retVal ? State_Initialized : State_Error;
    //! \todo We should save the error string for later retrieval (i.e. in the PluginDialog)
    return retVal;
}

void PluginWrapper::shutdown()
{
    m_Plugin->shutdown();
    m_State = State_Shutdown;
}

QString PluginWrapper::name()
{
    return m_Plugin->name();
}

QString PluginWrapper::version()
{
    return m_Plugin->version();
}

QList<Dependency> PluginWrapper::dependencies()
{
    return m_Plugin->dependencies();
}

int PluginWrapper::priority()
{
    return m_Priority;
}

void PluginWrapper::setPriority(int priority)
{
    m_Priority = priority;
}

} // namespace PluginManager
} // namespace Core
