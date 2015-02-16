/*!
   \file Menu.cpp
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

#include "Menu.h"

#include "ActionManagerPrivate.h"

namespace Core {
namespace ActionManager {

static const int defaultPriority = 128;

/*!
   \class Menu
   \todo Better documentation
   \brief Core::ActionManager::Menu
 */


/*!
   \brief Menu::Menu
   \todo Better documentation
 */
Menu::Menu()
{
    m_Priority = defaultPriority;
}

/*!
   \brief Menu::Menu
   \todo Better documentation
   \param text
 */
Menu::Menu(const QString &text)
{
    m_Text = text;
    m_Priority = defaultPriority;
}

/*!
   \brief Menu::Menu
   \todo Better documentation
   \param text
   \param priority
 */
Menu::Menu(const QString &text, const int &priority)
{
    m_Text = text;
    m_Priority = priority;
}

/*!
   \brief Menu::text
   \todo Better documentation
   \return
 */
QString Menu::text() const
{
    return m_Text;
}

/*!
   \brief Menu::setText
   \todo Better documentation
   \param text
 */
void Menu::setText(const QString &text)
{
    m_Text = text;
}

/*!
   \brief Menu::priority
   \todo Better documentation
   \return
 */
int Menu::priority() const
{
    return m_Priority;
}

/*!
   \brief Menu::setPriority
   \todo Better documentation
   \param priority
 */
void Menu::setPriority(const int &priority)
{
    m_Priority = priority;
}

/*!
   \class Core::ActionManager::MenuPath
   \brief Subclass of QList<Menu> with some helper functions to assist rapid development.
 *
   Please see the ActionManager documentation for detailed usage examples.
 *
   \sa Core::ActionManager::ActionManager
 */

/*!
   \fn MenuPath &MenuPath::operator<<(const QString &str)
   \brief Append a menu object (defined by a string) to the path
 *
   Example: \code
     path << "Edit" << "Find and Replace" << "Find";
   \endcode
 *
   Is equivalent to: \code
     path << Menu("Edit") << Menu("Find and Replace") << Menu("Find");
   \endcode
 *
   \param str A QString parameter, or equivalent object, to initialize a new Menu object
   \sa Menu, Menu::Menu(const QString &str), QString::QString(const char *ch)
 */

/*!
   \fn MenuPath &MenuPath::operator<<(const Menu &menu)
   \brief Append a menu object to the path
 *
   Example: \code
     MenuPath path;
     path << Menu(16, "Edit") << Menu("Find and Replace") << "Find";
   \endcode
 *
   \param menu A Menu object to be appended to the path
   \sa Menu
 */

/*!
   \fn MenuPath::MenuPath(const QString &str)
   \brief Allows the creation of a menu path object from a string
 *
   The most common use-case is to have a menu item that is only one level deep.  This helps the user achieve this with a single line.
 *
   Example: \code
   MenuPath path(tr("File"));
   \endcode
 *
   Is equivalent to: \code
   MenuPath path;
   path << Menu(tr("File"));
   \endcode
 *
   \param str A QString parameter, or equivalent object, to initialize a new Menu object
 */

/*!
   \internal
   \fn MenuWidget *MenuPath::menu()
   \brief MenuPath::menu
 *
   Example: \code
   MenuPath file("File");
   MenuWidget *menuWidget = file.menu();
   if(menuWidget == menuBar()) {
     qDebug() << "Widget is MenuBar()";
   }
   \endcode
 *
   \return Extreme branch QMenu item associated with this path
 */
MenuWidget *MenuPath::menu() const
{
    return (MenuWidget *)(ActionManager::instance().createMenuPath(*this));
}


} // namespace ActionManager
} // namespace Core
