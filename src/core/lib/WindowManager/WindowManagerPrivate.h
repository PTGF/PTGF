/*!
   \file WindowManagerPrivate.h
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

#ifndef CORE_WINDOWMANAGER_WINDOWMANAGERPRIVATE_H
#define CORE_WINDOWMANAGER_WINDOWMANAGERPRIVATE_H


//
//  W A R N I N G
//  -------------
//
// This file is not part of the public PTGF API.  This header file may change
// from version to version without notice, or even be removed.
//


#include "WindowManager.h"

class QAction;

namespace Core {
namespace WindowManager {

class IMainWindow;

class WindowManagerPrivate : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(WindowManagerPrivate)
    DECLARE_PUBLIC(WindowManager)

public:
    explicit WindowManagerPrivate();

    void registerMainWindow(IMainWindow *window);
    void deregisterMainWindow(IMainWindow *window);

    static bool ascending(IMainWindow *left, IMainWindow *right);

public slots:
    void aboutDialog();
    void pluginObjectRegistered(QObject *object);
    void pluginObjectDeregistered(QObject *object);
    void windowActivated();

private:
    bool m_Initialized;
    QList<IMainWindow *> m_MainWindows;
    QAction *m_AboutPage;

};

} // namespace WindowManager
} // namespace Core

#endif // CORE_WINDOWMANAGER_WINDOWMANAGERPRIVATE_H
