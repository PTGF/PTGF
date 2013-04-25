/*!
   \file ActionManager.h
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

#ifndef CORE_ACTIONMANAGER_ACTIONMANAGER_H
#define CORE_ACTIONMANAGER_ACTIONMANAGER_H

#include <QObject>
#include <QAction>

#include "ActionManagerLibrary.h"
#include "Menu.h"
#include "Context.h"

class QAction;
class QShortcut;

namespace Core {
namespace ActionManager {

class ActionManagerPrivate;

class ACTIONMANAGER_EXPORT ActionManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ActionManager)
    DECLARE_PRIVATE(ActionManager)

public:
    static ActionManager &instance();
    explicit ActionManager();
    ~ActionManager();

    bool initialize();
    void shutdown();

    Context *createContext(Context *parent = 0);

    MenuWidget *createMenuPath(const MenuPath &path);

    QAction *createAction(const MenuPath &path);
    QAction *createAction(Context *context, const MenuPath &path);

    void registerAction(const MenuPath &path, QAction *action);
    void registerAction(Context *context, const MenuPath &path, QAction *action);

    QShortcut *createShortcut(Context *context);

signals:

protected:


};

} // namespace ActionManager
} // namespace Core

#endif // CORE_ACTIONMANAGER_ACTIONMANAGER_H
