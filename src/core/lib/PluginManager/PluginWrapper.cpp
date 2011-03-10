/*!
   \file PluginWrapper.cpp
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

#include "PluginWrapper.h"

#ifdef QT_DEBUG
  #include <QtDebug>
#endif

Core::PluginWrapper::PluginWrapper(IPlugin *plugin, QString filePath, QObject *parent) :
        QObject(parent)
{
    if(!plugin)
        throw QString(tr("Cannot wrap null plugin pointer"));
    if(filePath.isEmpty())
        throw QString(tr("Cannot wrap empty filePath"));

    m_Plugin = plugin;
    m_FilePath = filePath;
    m_Status = PluginStatus_Loaded;

    loadSpecs();
}

void Core::PluginWrapper::loadSpecs()
{
    QString specFilePath = m_FilePath;
    int index = specFilePath.lastIndexOf('.');
    specFilePath.truncate(index);
    specFilePath.append(".spec");

#ifdef QT_DEBUG
    qDebug() << __FILE__ << __LINE__ << "Loading plugin spec file:" << specFilePath;
#endif

}


