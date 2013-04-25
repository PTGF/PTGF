/*!
   \file Menu.h
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

#ifndef CORE_ACTIONMANAGER_MENU_H
#define CORE_ACTIONMANAGER_MENU_H

#include <QString>
#include <QList>

#include "ActionManagerLibrary.h"

class QWidget;

namespace Core {
namespace ActionManager {

typedef QWidget MenuWidget;

class ACTIONMANAGER_EXPORT Menu
{
public:
    explicit Menu();
    Menu(const QString &text);
    Menu(const QString &text, const int &priority);

    QString text() const;
    void setText(const QString &text);

    int priority() const;
    void setPriority(const int &priority);

private:
    QString m_Text;
    int m_Priority;
};

typedef QListIterator<Menu> MenuPathIterator;
typedef QMutableListIterator<Menu> MutableMenuPathIterator;

class ACTIONMANAGER_EXPORT MenuPath : public QList<Menu>
{
public:
    inline MenuPath() { }
    inline explicit MenuPath(const Menu &menu) { append(menu); }
    inline MenuPath(const QString &str) { append(Menu(str)); }
    inline MenuPath(const MenuPath &l) : QList<Menu>(l) { }
    inline MenuPath(const QList<Menu> &l) : QList<Menu>(l) { }

    MenuWidget *menu() const;

    inline MenuPath operator+(const MenuPath &other) const { MenuPath n = *this; n += other; return n; }
    inline MenuPath &operator<<(const QString &str) { append(Menu(str)); return *this; }
    inline MenuPath &operator<<(const Menu &menu) { append(menu); return *this; }
    inline MenuPath &operator<<(const MenuPath &l) { *this += l; return *this; }
};

} // namespace ActionManager
} // namespace Core

#endif // CORE_ACTIONMANAGER_MENU_H
