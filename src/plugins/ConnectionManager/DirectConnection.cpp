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

#include "DirectConnection.h"

#include "DirectConnectionPage.h"
#include <SettingManager/SettingManager.h>

namespace Plugins {
namespace ConnectionManager {

/*! \class DirectConnection
    \brief Handles the interaction with a standard, remote (or local) TCP
           connected server.
    \sa DirectConnectionPage, ConnectionManager
 */


DirectConnection::DirectConnection(QObject *parent) :
    IConnection(parent)
{
    m_State = ConnectionState_Disconnected;

    readSettings();

    m_TcpSocket = new QTcpSocket(this);
    connect(m_TcpSocket, SIGNAL(readyRead()), this, SLOT(readReady()));
    connect(m_TcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(m_TcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(m_TcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(error(QAbstractSocket::SocketError)));
}

DirectConnection::~DirectConnection()
{
}

QWidget *DirectConnection::page()
{
    return new DirectConnectionPage(this);
}

void DirectConnection::connectToServer()
{
    setState(ConnectionState_Connecting);
    m_TcpSocket->connectToHost(m_HostName, m_Port);
}

void DirectConnection::disconnectFromServer()
{
    setState(ConnectionState_Disconnecting);
    m_TcpSocket->disconnectFromHost();
}

void DirectConnection::abort()
{
    setState(ConnectionState_Disconnected);
    m_TcpSocket->abort();
}

void DirectConnection::readReady()
{
    emit readyReceive();
}

void DirectConnection::error(QAbstractSocket::SocketError error)
{
    //TODO: These error messages were copied from Qt's docs. Already found one gramatical error. Needs more checking.

    switch(error) {
    case QAbstractSocket::ConnectionRefusedError:
        m_ErrorMessage = tr("The connection was refused by the peer (or timed out).");
        break;
    case QAbstractSocket::RemoteHostClosedError:
        m_ErrorMessage = tr("The remote host closed the connection. Note that the client socket (i.e., this socket) will be closed after the remote close notification has been sent.");
    case QAbstractSocket::HostNotFoundError:
        m_ErrorMessage = tr("The host address was not found.");
        break;
    case QAbstractSocket::SocketAccessError:
        m_ErrorMessage = tr("The socket operation failed because the application lacked the required privileges.");
        break;
    case QAbstractSocket::SocketResourceError:
        m_ErrorMessage = tr("The local system ran out of resources (e.g., too many sockets).");
        break;
    case QAbstractSocket::SocketTimeoutError:
        m_ErrorMessage = tr("The socket operation timed out.");
        break;
    case QAbstractSocket::DatagramTooLargeError:
        m_ErrorMessage = tr("The datagram was larger than the operating system's limit (which can be as low as 8192 bytes).");
        break;
    case QAbstractSocket::NetworkError:
        m_ErrorMessage = tr("An error occurred with the network (e.g., the network cable was accidentally unplugged).");
        break;
    case QAbstractSocket::AddressInUseError:
        m_ErrorMessage = tr("The address specified to QUdpSocket::bind() is already in use and was set to be exclusive.");
        break;
    case QAbstractSocket::SocketAddressNotAvailableError:
        m_ErrorMessage = tr("The address specified to QUdpSocket::bind() does not belong to the host.");
        break;
    case QAbstractSocket::UnsupportedSocketOperationError:
        m_ErrorMessage = tr("The requested socket operation is not supported by the local operating system (e.g., lack of IPv6 support).");
        break;
    case QAbstractSocket::ProxyAuthenticationRequiredError:
        m_ErrorMessage = tr("The socket is using a proxy, and the proxy requires authentication.");
        break;
    case QAbstractSocket::SslHandshakeFailedError:
        m_ErrorMessage = tr("The SSL/TLS handshake failed, so the connection was closed (only used in QSslSocket)");
        break;
    case QAbstractSocket::UnfinishedSocketOperationError:
        m_ErrorMessage = tr("Used by QAbstractSocketEngine only, The last operation attempted has not finished yet (still in progress in the background).");
        break;
    case QAbstractSocket::ProxyConnectionRefusedError:
        m_ErrorMessage = tr("Could not contact the proxy server because the connection to that server was denied");
        break;
    case QAbstractSocket::ProxyConnectionClosedError:
        m_ErrorMessage = tr("The connection to the proxy server was closed unexpectedly (before the connection to the final peer was established)");
        break;
    case QAbstractSocket::ProxyConnectionTimeoutError:
        m_ErrorMessage = tr("The connection to the proxy server timed out or the proxy server stopped responding in the authentication phase.");
        break;
    case QAbstractSocket::ProxyNotFoundError:
        m_ErrorMessage = tr("The proxy address set with setProxy() (or the application proxy) was not found.");
        break;
    case QAbstractSocket::ProxyProtocolError:
        m_ErrorMessage = tr("The connection negotiation with the proxy server because the response from the proxy server could not be understood.");
        break;
    case QAbstractSocket::UnknownSocketError:
    default:
        m_ErrorMessage = tr("Unknown socket error");
        break;
    }

    setState(ConnectionState_Error);
}

QString DirectConnection::errorMessage()
{
    return m_ErrorMessage;
}

void DirectConnection::connected()
{
    setState(ConnectionState_Connected);
}

void DirectConnection::disconnected()
{
    setState(ConnectionState_Disconnected);
}

void DirectConnection::writeSettings()
{
    Core::SettingManager::SettingManager *settingManager =
            Core::SettingManager::SettingManager::instance();

    settingManager->beginGroup("Plugins");
    settingManager->beginGroup("ConnectionManager");

    settingManager->setValue("DirectConnection/HostName", m_HostName);
    settingManager->setValue("DirectConnection/Port", m_Port);

    settingManager->endGroup();
    settingManager->endGroup();
}

void DirectConnection::readSettings()
{
    Core::SettingManager::SettingManager *settingManager =
            Core::SettingManager::SettingManager::instance();

    settingManager->beginGroup("Plugins");
    settingManager->beginGroup("ConnectionManager");

    m_HostName = settingManager->value("DirectConnection/Hostname", "localhost").toString();

    bool okay;
    m_Port = settingManager->value("DirectConnection/Port").toInt(&okay);
    if(!okay) m_Port = 2048;

    settingManager->endGroup();
    settingManager->endGroup();
}

ConnectionStates DirectConnection::state()
{
    return m_State;
}

void DirectConnection::setState(ConnectionStates state)
{
    m_State = state;
    emit stateChanged();
}

void DirectConnection::send(QString command)
{
    if(!command.endsWith('\n'))
        command.append('\n');
    m_TcpSocket->write(command.toLatin1());
}

QString DirectConnection::receive()
{
    return m_TcpSocket->readAll();
}


} // namespace OpenSpeedShop
} // namespace Plugins

