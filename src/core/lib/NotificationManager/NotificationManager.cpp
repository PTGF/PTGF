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

#if QT_VERSION >= 0x050000
#  include <QStandardPaths>
#else
#  include <QDesktopServices>
#endif

#include <QDockWidget>
#include <QMenuBar>
#include <QDateTime>
#include <QDebug>
#include <QDir>

#include <CoreWindow/CoreWindow.h>
#include <PluginManager/PluginManager.h>
#include <PrettyWidgets/ConsoleWidget.h>
#include <ActionManager/ActionManager.h>

namespace Core {
namespace NotificationManager {


/***** PUBLIC IMPLEMENTATION *****/

/*!
   \class NotificationManager
   \brief Manages notifications displayed in the CoreWindow and logged events
   \todo better documentation
 */


/*!
   \fn NotificationManager::instance()
   \brief Accessor to singleton instance
   \todo better documentation
 */
NotificationManager &NotificationManager::instance()
{
    static NotificationManager m_Instance;
    return m_Instance;
}


/*!
   \internal
   \brief NotificationManager::NotificationManager
 */
NotificationManager::NotificationManager() :
    d(new NotificationManagerPrivate)
{
    d->q = this;
}

/*!
   \internal
   \brief NotificationManager::~NotificationManager
 */
NotificationManager::~NotificationManager()
{
}


/*!
   \internal
   \brief NotificationManager::initialize
   \return
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

        ActionManager::ActionManager &actionManager = ActionManager::ActionManager::instance();
        ActionManager::MenuPath menuPath("Window");
        actionManager.registerAction(menuPath, dockWidget->toggleViewAction());



        //
        // Create log file and register handler for qDebug() messages
        //

        QDir path;
#if QT_VERSION >= 0x050000
        path.setPath(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
#else
        path.setPath(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
#endif

        // Create the logfile path if need be
        if(!path.exists()) {
            if(!path.mkpath(path.path())) {
                qCritical() << tr("Unable to create path for log files at: \"%1\"").arg(path.path());
                return false;
            }
        }

        // Set the logfile name
        d->m_LogFile.setFileName(QString("%1/%2.txt")
                                 .arg(path.path())
                                 .arg(QDateTime::currentDateTime().toUTC().toString(QString("yyyyMMddhhmmsszzz"))));


        // Register with the debugging system
#if QT_VERSION >= 0x050000
        qInstallMessageHandler(d->qMessageHandler);
#else
        qInstallMsgHandler(d->qMessageHandler);
#endif

        Core::PluginManager::PluginManager::instance().addObject(this);

    } catch(...) {
        return false;
    }

    return d->m_Initialized = true;
}

/*!
   \internal
   \brief NotificationManager::shutdown
 */
void NotificationManager::shutdown()
{
    if(!d->m_Initialized) {
        return;
    }

#if QT_VERSION >= 0x050000
    qInstallMessageHandler(0);
#else
    qInstallMsgHandler(0);
#endif

    if(d->m_LogFile.isOpen()) {
        d->m_LogFile.close();
    }
}


/*!
   \fn NotificationManager::writeToLogFile()
   \brief Writes message to log file
   \todo better documentation
 */
void NotificationManager::writeToLogFile(const int &level, QString message)
{
    d->writeToLogFile(level, message);
}

/*!
   \fn NotificationManager::notify()
   \brief Notification of event displayed in CoreWindow

   If an identical notification already exists, it will be returned. This means that the same
   NotificationWidget can be returned under different calls to this function.  Do not take ownership,
   or delete the widget manually, it will screw things up!

   If the same widget is returned, and a timeout has been set, the timer will be restarted.

   \returns NotificationWidget, which is owned and destroyed by CoreWindow
   \todo better documentation
 */
NotificationWidget *NotificationManager::notify(const QString &text,
                                                NotificationWidget::Icon icon,
                                                NotificationWidget::StandardButtons buttons,
                                                const QObject *receiver, const char *member)
{
    return d->notify(text, icon, buttons, receiver, member);
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
        if(!m_LogFile.open(QIODevice::Append | QIODevice::Text)) {
            QString logFileFailMsg = tr("Failed to open log file: %1").arg(m_LogFile.fileName());
            q->notify(logFileFailMsg, NotificationWidget::Critical);
            m_StdError << Q_FUNC_INFO << logFileFailMsg;
        }

        m_LogFileStream.setDevice(&m_LogFile);
    }

    QDateTime currentTime = QDateTime::currentDateTime().toUTC();

    QString outputString = QString("%1 - %2")
            .arg(currentTime.toString("yyyy-MM-dd hh:mm:ss.zzz"))
            .arg(message);

    if(m_LogFile.isOpen()) {
        m_LogFileStream << outputString << endl;
    }

    m_ConsoleWidget->messageEvent(level, outputString);
    m_StdError << outputString << endl;

}

#if QT_VERSION >= 0x050000
void NotificationManagerPrivate::qMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &message)
#else
void NotificationManagerPrivate::qMessageHandler(QtMsgType type, const char *message)
#endif
{
#if QT_VERSION >= 0x050000
    Q_UNUSED(context)
#endif

    QString msg = QString(message);
    int level = 0;

    // Static member function requires that we get the singleton pointer directly
    NotificationManager *q = &NotificationManager::instance();

    switch (type) {
    case QtDebugMsg:
        level = (int)QtDebugMsg;
        msg.prepend(q->tr("Debug: "));
        break;
    case QtWarningMsg:
        q->notify(msg, NotificationWidget::Warning)->setTimeoutInterval(10 * 1000);
        level = (int)QtWarningMsg;
        msg.prepend(q->tr("Warning: "));
        break;
    case QtCriticalMsg:
        q->notify(msg, NotificationWidget::Critical);
        level = (int)QtCriticalMsg;
        msg.prepend(q->tr("Critical: "));
        break;
    case QtFatalMsg:
        level = (int)QtFatalMsg;
        msg.prepend(q->tr("Fatal: "));
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

void NotificationManagerPrivate::removeNotificationWidget()
{
    if(NotificationWidget *nw = qobject_cast<NotificationWidget *>(sender())) {
        m_NotificationWidgets.removeAll(nw);
    }
}

NotificationWidget *NotificationManagerPrivate::notify(const QString &text,
                                                       NotificationWidget::Icon icon,
                                                       NotificationWidget::StandardButtons buttons,
                                                       const QObject *receiver, const char *member)
{
    CoreWindow::CoreWindow &coreWindow = CoreWindow::CoreWindow::instance();

    foreach(NotificationWidget *nw, m_NotificationWidgets) {
        if( nw->text().compare(text, Qt::CaseInsensitive) == 0
                && nw->icon() == icon
                && nw->standardButtons() == buttons ) {

            if(nw->timeoutInterval() > 0) {    // Reset the timer if necessary
                nw->setTimeoutInterval(nw->timeoutInterval());
            }

            if(receiver && member) {
                connect(nw, SIGNAL(buttonClicked(NotificationWidget::StandardButton)), receiver, member);
            }

            return nw;
        }
    }


    NotificationWidget *notificationWidget =
            new NotificationWidget(text, icon, buttons, receiver, member, &coreWindow);

    m_NotificationWidgets.append(notificationWidget);
    connect(notificationWidget, SIGNAL(closing()), this, SLOT(removeNotificationWidget()));
    connect(notificationWidget, SIGNAL(destroyed()), this, SLOT(removeNotificationWidget()));

    coreWindow.addNotificationWidget(notificationWidget);
    notificationWidget->setFocus();

    return notificationWidget;
}


} // namespace NotificationManager
} // namespace Core
