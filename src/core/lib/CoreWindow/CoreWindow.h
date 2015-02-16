/*!
   \file CoreWindow.h
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

#ifndef CORE_COREWINDOW_COREWINDOW_H
#define CORE_COREWINDOW_COREWINDOW_H


//
//  W A R N I N G
//  -------------
//
// This file is not part of the public PTGF API.  This header file may change
// from version to version without notice, or even be removed.
//


#include <QMainWindow>
#include <SettingManager/ISettingPageFactory.h>
#include "CoreWindowLibrary.h"

class QProgressBar;


namespace Core {

namespace WindowManager {
    class WindowManager;
}
namespace NotificationManager {
    class NotificationManagerPrivate;
}

namespace CoreWindow {

namespace Ui {
    class CoreWindow;
}

class COREWINDOW_EXPORT CoreWindow : public QMainWindow, public SettingManager::ISettingPageFactory
{
    Q_OBJECT
    Q_INTERFACES(Core::SettingManager::ISettingPageFactory)

public:
    static CoreWindow &instance();

    bool initialize();
    bool initialized();
    void shutdown();

    QProgressBar *addProgressBar();
    void removeProgressBar(QProgressBar *progressBar);

    void addCentralWidget(QWidget *widget, int priority = 128, QString title = QString(), QIcon icon = QIcon());
    void removeCentralWidget(QWidget *widget);

    /* BEGIN ISettingPageFactory */
    QIcon settingPageIcon();
    QString settingPageName();
    int settingPagePriority();
    SettingManager::ISettingPage *createSettingPage();
    /* END ISettingPageFactory */

public slots:
    void setCurrentCentralWidget(QWidget *current);

protected:
    explicit CoreWindow(QWidget *parent = 0);
    ~CoreWindow();

    void readSettings();
    void writeSettings();
    void initActions();

    void updateWindowTitle();

    void addNotificationWidget(QWidget *notificationWidget);


protected slots:
    void setCurrentCentralWidget();
    void on_actionExit_triggered();


private:
    Ui::CoreWindow *ui;
    bool m_Initialized;
    QString m_StylesheetFilePath;

    friend class CoreSettingPage;
    friend class NotificationManager::NotificationManagerPrivate;
    friend class WindowManager::WindowManager;
};


} // namespace CoreWindow
} // namespace Core
#endif // CORE_COREWINDOW_COREWINDOW_H
