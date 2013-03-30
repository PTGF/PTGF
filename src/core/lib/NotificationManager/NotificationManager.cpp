/*!
   \file NotificationManager.cpp
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

#include "NotificationManagerPrivate.h"

#include <QDesktopServices>
#include <QDockWidget>
#include <QMenuBar>
#include <QAction>
#include <QDateTime>
#include <QDebug>

#include <CoreWindow/CoreWindow.h>
#include <PluginManager/PluginManager.h>
#include <PrettyWidgets/ConsoleWidget.h>

namespace Core {
namespace NotificationManager {


/***** PUBLIC IMPLEMENTATION *****/

/*! \class NotificationManager
    \brief Manages notifications displayed in the CoreWindow and logged events
    \todo better documentation
 */


/*! \fn NotificationManager::instance()
    \brief Accessor to singleton instance
    \todo better documentation
 */
NotificationManager &NotificationManager::instance()
{
    static NotificationManager m_Instance;
    return m_Instance;
}


/*! \internal
 */
NotificationManager::NotificationManager() :
    d(new NotificationManagerPrivate)
{
    d->q = this;
}

/*! \internal
 */
NotificationManager::~NotificationManager()
{
}


/*! \internal
 */
bool NotificationManager::initialize()
{
    if(d->m_Initialized) {
        return false;
    }

    try {

        CoreWindow::CoreWindow &coreWindow = CoreWindow::CoreWindow::instance();


        // Setup dock widget for message console
        QDockWidget *dockWidget = new QDockWidget(tr("Message Console"), &coreWindow);
        dockWidget->setObjectName("MessageConsoleDockWidget");
        dockWidget->hide();

        d->m_ConsoleWidget = new ConsoleWidget(dockWidget);
        d->m_ConsoleWidget->setEventLevelColor((int)QtDebugMsg, Qt::darkGreen);
        d->m_ConsoleWidget->setEventLevelColor((int)QtWarningMsg, Qt::darkYellow);
        d->m_ConsoleWidget->setEventLevelColor((int)QtCriticalMsg, Qt::darkRed);
        d->m_ConsoleWidget->setEventLevelColor((int)QtFatalMsg, Qt::darkRed);
        dockWidget->setWidget(d->m_ConsoleWidget);

        coreWindow.addDockWidget(Qt::BottomDockWidgetArea, dockWidget);

        foreach(QAction *action, coreWindow.menuBar()->actions()) {
            if(action->text() == tr("Window")) {
                action->menu()->addAction(dockWidget->toggleViewAction());
            }
        }


        // Create log file and register handler for qDebug() messages
        d->m_LogFile.setFileName(QString("%1/%2.txt")
                                 .arg(QDesktopServices::storageLocation(QDesktopServices::DataLocation))
                                 .arg(QDateTime::currentDateTime().toUTC().toString(QString("yyyyMMddhhmmsszzz"))));

        qInstallMsgHandler(d->qMessageHandler);


        Core::PluginManager::PluginManager::instance().addObject(this);

    } catch(...) {
        return false;
    }

    return d->m_Initialized = true;
}

/*! \internal
 */
void NotificationManager::shutdown()
{
    if(!d->m_Initialized) {
        return;
    }

    qInstallMsgHandler(0);

    if(d->m_LogFile.isOpen()) {
        d->m_LogFile.close();
    }
}


/*! \fn NotificationManager::writeToLogFile()
    \brief Writes message to log file
    \todo better documentation
 */
void NotificationManager::writeToLogFile(const int &level, QString message)
{
    d->writeToLogFile(level, message);
}

/*! \fn NotificationManager::notify()
    \brief Notification of event displayed in CoreWindow
    \returns NotificationWidget, which is owned and destroyed by CoreWindow
    \todo better documentation
 */
NotificationWidget *NotificationManager::notify(const QString &text,
                                                NotificationWidget::Icon icon,
                                                NotificationWidget::StandardButtons buttons,
                                                const QObject *reciever, const char *member)
{
    return d->notify(text, icon, buttons, reciever, member);
}



/***** PRIVATE IMPLEMENTATION *****/

NotificationManagerPrivate::NotificationManagerPrivate() :
    q(NULL),
    m_Initialized(false),
    m_StdOut(stdout, QIODevice::WriteOnly),
    m_StdError(stderr, QIODevice::WriteOnly),
    m_ConsoleWidget(NULL)
{
}

NotificationManagerPrivate::~NotificationManagerPrivate()
{
    if(m_LogFile.isOpen()) {
        m_LogFile.close();
    }
}

void NotificationManagerPrivate::writeToLogFile(const int &level, QString message)
{
    if(!m_LogFile.isOpen()) {
        m_LogFile.open(QIODevice::Append | QIODevice::Text);
        m_LogFileStream.setDevice(&m_LogFile);
    }

    QDateTime currentTime = QDateTime::currentDateTime().toUTC();

    if(m_LogFile.isOpen()) {
        QString outputString = QString("%1 - %2")
                .arg(currentTime.toString("yyyy-MM-dd hh:mm:ss.zzz"))
                .arg(message);

        m_LogFileStream << outputString << endl;

        m_StdError << outputString << endl;

        m_ConsoleWidget->messageEvent(level, outputString);
    }
}

void NotificationManagerPrivate::qMessageHandler(QtMsgType type, const char *message)
{
    QString msg;
    int level = 0;

    // Static member function requires that we get the singleton pointer directly
    NotificationManager *q = &NotificationManager::instance();

    switch (type) {
    case QtDebugMsg:

        msg = q->tr("Debug: %1").arg(message);
        level = (int)QtDebugMsg;
        break;
    case QtWarningMsg:
        msg = q->tr("Warning: %1").arg(message);
        level = (int)QtWarningMsg;
        q->notify(msg, NotificationWidget::Warning);
        break;
    case QtCriticalMsg:
        msg = q->tr("Critical: %1").arg(message);
        level = (int)QtCriticalMsg;
        q->notify(msg, NotificationWidget::Critical);
        break;
    case QtFatalMsg:
        msg = q->tr("Fatal: %1").arg(message);
        level = (int)QtFatalMsg;
        break;
    default:
        msg = q->tr("Unknown: %1").arg(message);
        break;
    }

    q->writeToLogFile(level, msg);

    if(type == QtFatalMsg) {
        abort();
    }
}

NotificationWidget *NotificationManagerPrivate::notify(const QString &text,
                                       NotificationWidget::Icon icon,
                                       NotificationWidget::StandardButtons buttons,
                                       const QObject *reciever, const char *member)
{
#ifdef COREWINDOW_DEBUG
    qDebug() << __FILE__ << __LINE__ << "\tCoreWindow::notify";
#endif

    CoreWindow::CoreWindow &coreWindow = CoreWindow::CoreWindow::instance();

    NotificationWidget *notificationWidget =
            new NotificationWidget(text, icon, buttons, reciever, member, &coreWindow);

    coreWindow.addNotificationWidget(notificationWidget);
    notificationWidget->setFocus();

    return notificationWidget;
}


} // namespace NotificationManager
} // namespace Core
