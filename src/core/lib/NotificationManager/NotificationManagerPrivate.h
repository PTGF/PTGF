/*!
   \file NotificationManagerPrivate.h
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
   
   \section DESCRIPTION
   
 */

#ifndef CORE_NOTIFICATIONMANAGER_NOTIFICATIONMANAGERPRIVATE_H
#define CORE_NOTIFICATIONMANAGER_NOTIFICATIONMANAGERPRIVATE_H

#include <QtCore>
#include <QtGui>

#include <NotificationManager/NotificationWidget.h>
#include <PrettyWidgets/ConsoleWidget.h>

namespace Core {
namespace NotificationManager {

class NotificationManagerPrivate : public QObject
{
    Q_OBJECT
public:
    static NotificationManagerPrivate &instance();
    ~NotificationManagerPrivate();

    bool initialize();
    bool initialized();
    void shutdown();

    void writeToLogFile(const int &level, QString message);

    NotificationWidget *notify(const QString &text,
                               NotificationWidget::Icon icon = NotificationWidget::NoIcon,
                               NotificationWidget::StandardButtons buttons = NotificationWidget::NoButton,
                               const QObject *reciever = 0, const char *member = 0);

protected:
    NotificationManagerPrivate();

    static void qMessageHandler(QtMsgType type, const char *message);

private:
    bool m_Initialized;
    QFile m_LogFile;
    QTextStream m_LogFileStream;

    QTextStream m_StdOut;
    QTextStream m_StdError;

    ConsoleWidget *m_ConsoleWidget;
};

} // namespace NotificationManager
} // namespace Core

#endif // CORE_NOTIFICATIONMANAGER_NOTIFICATIONMANAGERPRIVATE_H
