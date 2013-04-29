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
 */

#ifndef CORE_NOTIFICATIONMANAGER_NOTIFICATIONMANAGERPRIVATE_H
#define CORE_NOTIFICATIONMANAGER_NOTIFICATIONMANAGERPRIVATE_H


//
//  W A R N I N G
//  -------------
//
// This file is not part of the public PTGF API.  This header file may change
// from version to version without notice, or even be removed.
//


#include <QObject>
#include <QString>
#include <QFile>
#include <QTextStream>

#include "NotificationManager.h"
#include "NotificationWidget.h"

class ConsoleWidget;

namespace Core {
namespace NotificationManager {

class NotificationManagerPrivate : public QObject
{
    Q_OBJECT
    DECLARE_PUBLIC(NotificationManager)

public:
    NotificationManagerPrivate();
    ~NotificationManagerPrivate();

    void writeToLogFile(const int &level, QString message);

    NotificationWidget *notify(const QString &text,
                               NotificationWidget::Icon icon,
                               NotificationWidget::StandardButtons buttons = NotificationWidget::NoButton,
                               const QObject *receiver = 0, const char *member = 0);

protected:
#if QT_VERSION >= 0x050000
    static void qMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &message);
#else
    static void qMessageHandler(QtMsgType type, const char *message);
#endif

protected slots:
    void removeNotificationWidget();

private:
    bool m_Initialized;
    QFile m_LogFile;
    QTextStream m_LogFileStream;

    QTextStream m_StdOut;
    QTextStream m_StdError;

    ConsoleWidget *m_ConsoleWidget;

    QList<NotificationWidget *> m_NotificationWidgets;
};

} // namespace NotificationManager
} // namespace Core

#endif // CORE_NOTIFICATIONMANAGER_NOTIFICATIONMANAGERPRIVATE_H
