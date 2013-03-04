/*!
   \file CoreWindow.h
   \author Dane Gardner <dane.gardner@gmail.com>
   \version

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


#ifndef COREWINDOW_H
#define COREWINDOW_H

#include <QtCore>
#include <QtGui>

#include <SettingManager/ISettingPageFactory.h>

#include "CoreWindowLibrary.h"

namespace Core {

namespace WindowManager {
    class WindowManager;
}
namespace NotificationManager {
    class NotificationManager;
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

    QList<QAction*> allActions();

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

    QList<QAction*> allActions(QList<QAction *> actions);

    void addNotificationWidget(QWidget *notificationWidget);


protected slots:
    void setCurrentCentralWidget();
    void on_actionExit_triggered();


private:
    Ui::CoreWindow *ui;
    bool m_Initialized;
    QString m_StylesheetFilePath;

    friend class CoreSettingPage;
    friend class NotificationManager::NotificationManager;
    friend class WindowManager::WindowManager;
};


} // namespace CoreWindow
} // namespace Core
#endif // COREWINDOW_H
