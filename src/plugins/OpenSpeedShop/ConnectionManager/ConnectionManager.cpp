/*!
   \file 
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

#include "ConnectionManager.h"

namespace Plugins {
namespace OpenSpeedShop {


ConnectionManager *m_Instance;
ConnectionManager *ConnectionManager::instance()
{
    return m_Instance ? m_Instance : m_Instance = new ConnectionManager();
}

ConnectionManager::ConnectionManager(QObject *parent) :
    QObject(parent)
{
}

ConnectionManager::~ConnectionManager()
{
    // We're responsible for disposing of the connections
    while(!m_Connections.isEmpty())
        delete(m_Connections.takeFirst());
}

bool ConnectionManager::initialize()
{
    Core::MainWindow::MainWindow *mainWindow =
            Core::MainWindow::MainWindow::instance();

    QDockWidget *dockWidget = new QDockWidget(mainWindow);
    dockWidget->setWidget(new ConnectionWidget(dockWidget));
    mainWindow->addDockWidget(Qt::LeftDockWidgetArea, dockWidget, Qt::Vertical);

    registerConnection(new DirectConnection());

    return true;
}

void ConnectionManager::shutdown()
{
}

void ConnectionManager::registerConnection(IConnection *connection)
{
    connection->setParent(this);
    m_Connections.append(connection);
    emit connectionRegistered(connection);
}


} // namespace OpenSpeedShop
} // namespace Plugins
