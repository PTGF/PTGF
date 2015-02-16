/*!
   \file NotificationWidgetPrivate.h
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

#ifndef CORE_NOTIFICATIONMANAGER_NOTIFICATIONWIDGETPRIVATE_H
#define CORE_NOTIFICATIONMANAGER_NOTIFICATIONWIDGETPRIVATE_H


//
//  W A R N I N G
//  -------------
//
// This file is not part of the public PTGF API.  This header file may change
// from version to version without notice, or even be removed.
//


#include "Global.h"
#include "NotificationWidget.h"

class QProgressBar;
class QHBoxLayout;

namespace Core {
namespace NotificationManager {

class NotificationWidgetPrivate
{
    DECLARE_PUBLIC(NotificationWidget)

public:
    explicit NotificationWidgetPrivate();

protected:
    void setupUi();

private:
    QHBoxLayout *m_Layout;
    QLabel *m_Label;
    QLabel *m_IconLabel;
    NotificationWidget::Icon m_Icon;
    QDialogButtonBox *m_ButtonBox;
    QToolButton *m_CloseButton;
    QProgressBar *m_ProgressBar;

    int m_TimeoutInterval;
    int m_TimeoutTimerId;

    int m_FadeoutTimerId;
};

} // namespace CoreWindow
} // namespace Core

#endif // CORE_NOTIFICATIONMANAGER_NOTIFICATIONWIDGETPRIVATE_H
