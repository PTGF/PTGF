/*!
   \file NotificationWidget.h
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

#ifndef CORE_NOTIFICATIONMANAGER_NOTIFICATIONWIDGET_H
#define CORE_NOTIFICATIONMANAGER_NOTIFICATIONWIDGET_H

#include <QFrame>
#include "NotificationManagerLibrary.h"

class QPushButton;
class QAbstractButton;
class QLabel;
class QToolButton;
class QDialogButtonBox;

namespace Core {
namespace NotificationManager {

class NotificationWidgetPrivate;

class NOTIFICATIONMANAGER_EXPORT NotificationWidget : public QFrame
{
    Q_OBJECT

    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(Icon icon READ icon WRITE setIcon)
    Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap)
    Q_PROPERTY(StandardButtons standardButtons READ standardButtons WRITE setStandardButtons)
    Q_PROPERTY(int timeoutInterval READ timeoutInterval WRITE setTimeoutInterval)
    Q_PROPERTY(int progress READ progress WRITE setProgress)

    DECLARE_PRIVATE(NotificationWidget)

public:
    enum Icon {
        // keep this in sync with QMessageBox::Icon
        NoIcon = 0,
        Information = 1,
        Warning = 2,
        Critical = 3,
        Question = 4,
        Loading = 32
    };

    enum ButtonRole {
        // keep this in sync with QMessageBox::ButtonRole
        InvalidRole = -1,
        AcceptRole,
        RejectRole,
        DestructiveRole,
        ActionRole,
        HelpRole,
        YesRole,
        NoRole,
        ResetRole,
        ApplyRole,

        NRoles
    };

    enum StandardButton {
        // keep this in sync with QMessageBox::StandardButton
        NoButton           = 0x00000000,
        Ok                 = 0x00000400,
        Save               = 0x00000800,
        SaveAll            = 0x00001000,
        Open               = 0x00002000,
        Yes                = 0x00004000,
        YesToAll           = 0x00008000,
        No                 = 0x00010000,
        NoToAll            = 0x00020000,
        Abort              = 0x00040000,
        Retry              = 0x00080000,
        Ignore             = 0x00100000,
        Close              = 0x00200000,
        Cancel             = 0x00400000,
        Discard            = 0x00800000,
        Help               = 0x01000000,
        Apply              = 0x02000000,
        Reset              = 0x04000000,
        RestoreDefaults    = 0x08000000
    };
    Q_DECLARE_FLAGS(StandardButtons, StandardButton)

    explicit NotificationWidget(QWidget *parent = 0);
    NotificationWidget(const QString &text, Icon icon = NoIcon, StandardButtons buttons = NoButton, QWidget *parent = 0);
    NotificationWidget(const QString &text, Icon icon = NoIcon, StandardButtons buttons = NoButton,
                       const QObject *receiver = NULL, const char *member = NULL, QWidget *parent = 0);
    ~NotificationWidget();

    QString text() const;
    void setText(const QString &text);

    Icon icon() const;
    void setIcon(const Icon &icon);

    QPixmap pixmap() const;
    void setPixmap(const QPixmap &pixmap);

    StandardButtons standardButtons() const;
    void setStandardButtons(StandardButtons standardButtons);

    void addButton(StandardButton button);
    QPushButton *button(StandardButton button) const;
    QList<QAbstractButton *> buttons() const;

    int timeoutInterval() const;
    void setTimeoutInterval(const int &msec);

    int progress() const;
    void setProgress(const int &progress);

    bool close();

signals:
    void buttonClicked(StandardButton standardButton);
    void closing();

protected:
    virtual void keyReleaseEvent(QKeyEvent *event);
    virtual void timerEvent(QTimerEvent *event);

protected slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_closeButton_clicked();

};

Q_DECLARE_OPERATORS_FOR_FLAGS(NotificationWidget::StandardButtons)

} // namespace CoreWindow
} // namespace Core

#endif // CORE_NOTIFICATIONMANAGER_NOTIFICATIONWIDGET_H
