/*!
   \file ActionManagerPrivate.h
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

#ifndef CORE_ACTIONMANAGER_ACTIONMANAGERPRIVATE_H
#define CORE_ACTIONMANAGER_ACTIONMANAGERPRIVATE_H

#include <QMultiMap>
#include "ActionManager.h"

namespace Core {
namespace ActionManager {

class ActionManagerPrivate : QObject
{
    Q_OBJECT
    DECLARE_PUBLIC(ActionManager)

public:
    explicit ActionManagerPrivate();

protected slots:
    void contextChanged();
    void actionDestroyed(QObject *object);
    void shortcutDestroyed(QObject *object);

private:
    bool m_Initialized;

    QMultiMap<Context *, QAction *> m_Actions;
    QMultiMap<Context *, QShortcut *> m_Shortcuts;

};

} // namespace ActionManager
} // namespace Core

#endif // CORE_ACTIONMANAGER_ACTIONMANAGERPRIVATE_H
