/*!
   \file ActionManager.h
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

#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMenu>
#include <QToolBar>
#include <QStatusBar>

namespace Core {
namespace ActionManager {

class ActionManager : public QObject
{
    Q_OBJECT
public:
    static ActionManager *instance();

    void registerMenuItem(QString menuName, QAction *action, int priority=0);
//    void registerToolBarItem(QString toolBarName, QAction action, int priority=0);
//    void registerStatusBarItem(QString statusBarName, QAction action, int priority=0);

//    QList<QToolBar *> m_ToolBars;

signals:
    void menuAdded(QMenu *);
    void toolBarAdded(QToolBar *);
    void statusBarAdded(QStatusBar *);

public slots:

protected:
    ActionManager();
    ~ActionManager();

    QList<QMenu *> m_Menus;

};

}}
#endif // ACTIONMANAGER_H
