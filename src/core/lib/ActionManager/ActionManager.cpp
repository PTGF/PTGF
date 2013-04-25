/*!
   \file ActionManager.cpp
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

#include "ActionManagerPrivate.h"

#include <QAction>
#include <QShortcut>
#include <QMenu>

#include <private/qshortcutmap_p.h>
#include <private/qapplication_p.h>

#include <CoreWindow/CoreWindow.h>

#ifdef ACTIONMANAGER_DEBUG
#  include <QDebug>
#endif


namespace Core {
namespace ActionManager {

/*!
   \class ActionManager
   \brief The ActionManager class is part of the core framework libraries, and
          manages the menus and toolbars in the CoreWindow.
 *
          singleton class
 *
   \todo Better documentation
 */

/*!
   \fn ActionManager::instance()
   \brief Access to the singleton instance of this class
   \returns A reference to the singleton instance of this class
 */
ActionManager &ActionManager::instance()
{
    static ActionManager m_Instance;
    return m_Instance;
}

/*!
   \fn ActionManager::ActionManager()
   \brief Constructor
 */
ActionManager::ActionManager() :
    QObject(NULL),
    d(new ActionManagerPrivate)
{
    d->q = this;
}

/*!
   \internal
   \brief ActionManager::~ActionManager()
 */
ActionManager::~ActionManager()
{
}

/*!
   \fn ActionManager::initialize()
   \brief Initializes this class after it has been constructed.
   This design pattern allows the class to perform any operations after a class that this object is dependent upon has been
   constructed.
   \returns true if successful.
 */
bool ActionManager::initialize()
{
    if(d->m_Initialized) {
        return false;
    }

    return d->m_Initialized = true;
}

/*!
   \fn ActionManager::shutdown()
   \brief Notifies the instance that it should perform any clean-up operations before destruction.
   This method is called manually, before the application is closed.  It will occur before destruction of the instance.
 */
void ActionManager::shutdown()
{
    if(!d->m_Initialized) {
        return;
    }

    // ...

}

/*!
   \brief ActionManager::createContext
   \param parent
   \return
 */
Context *ActionManager::createContext(Context *parent)
{
    Context *context = new Context(parent);
    connect(context, SIGNAL(changed()), d.data(), SLOT(contextChanged()));
    return context;
}

/*!
   \internal
   \brief widgetInsertAction Inserts an action, by priority, into a parent widget
   \param action
   \param parent
 */
inline void widgetInsertAction(QAction *action, MenuWidget *parent) {
    Q_ASSERT(action);
    Q_ASSERT(parent);

    QList<QAction *> actions = parent->actions();
    bool actionInserted = false;

    for(int i=0; i < actions.count(); ++i) {
        if(actions[i]->priority() > action->priority()) {
            parent->insertAction(actions[i], action);
            actionInserted = true;
            break;
        }
    }

    if(!actionInserted) {
        parent->addAction(action);
    }
}

/*!
   \internal
   \brief createMenu finds or creates a menu widget based on given parameters.
   This is an internal use only helper function.
   \param menu
   \param parent
   \return
 */
inline MenuWidget *createMenu(const Menu &menu, MenuWidget *parent)
{
    Q_ASSERT(parent);
    Q_ASSERT(!menu.text().isEmpty());

    // Locate the action
    QAction *action = NULL;
    foreach(QAction *childAction, parent->actions()) {
        if(childAction->text() == menu.text()) {
            action = childAction;
        }
    }

    if(action) {
        // If it's not a menu item, force it to be one
        if(!action->menu()) {
            action->setMenu(new QMenu(action->text(), parent));
        }

    } else {
        // Doesn't already exist, create one and insert it by priority
        action = (new QMenu(parent))->menuAction();
        action->setText(menu.text());
        action->setPriority((QAction::Priority)menu.priority());
        widgetInsertAction(action, parent);

    }

    return action->menu();
}


/*!
   \brief ActionManager::createMenuPath
   \param path
   \return
 */
MenuWidget *ActionManager::createMenuPath(const MenuPath &path)
{
    // Top widget is the menu bar
    MenuWidget *parentWidget = (MenuWidget *)(CoreWindow::CoreWindow::instance().menuBar());

    // Traverse or generate the path
    foreach(Menu pathMenu, path) {
        parentWidget = createMenu(pathMenu, parentWidget);
    }

    return parentWidget;
}

/*!
   \brief ActionManager::createAction
   \param path
   \return
 */
QAction *ActionManager::createAction(const MenuPath &path)
{
    return createAction(NULL, path);
}

/*!
   \brief ActionManager::createAction
   \param context
   \param path
   \return
 */
QAction *ActionManager::createAction(Context *context, const MenuPath &path)
{
    QAction *action = new QAction(this);
    registerAction(context, path, action);
    return action;
}

/*!
   \brief ActionManager::registerAction()
   \param path
   \param action
 */
void ActionManager::registerAction(const MenuPath &path, QAction *action)
{
    registerAction(NULL, path, action);
}

/*!
   \brief ActionManager::registerAction
   \param context
   \param path
   \param action
 */
void ActionManager::registerAction(Context *context, const MenuPath &path, QAction *action)
{
    Q_ASSERT(action);

    MenuWidget *menuWidget = path.menu();
    if(!menuWidget) {
        menuWidget = (MenuWidget *)(CoreWindow::CoreWindow::instance().menuBar());
    }

    widgetInsertAction((QAction*)action, menuWidget);
    d->m_Actions.insert(context, action);
    connect(action, SIGNAL(destroyed(QObject*)), d.data(), SLOT(actionDestroyed(QObject*)));
}

/*!
   \brief ActionManager::createShortcut
   \param context
   \return
 */
QShortcut *ActionManager::createShortcut(Context *context)
{
    QShortcut *shortcut = new QShortcut(NULL);
    d->m_Shortcuts.insert(context, shortcut);
    connect(shortcut, SIGNAL(destroyed(QObject*)), d.data(), SLOT(shortcutDestroyed(QObject*)));
    return shortcut;
}





/***** PRIVATE IMPLEMENTATION *****/

/*!
   \internal
   \brief ActionManagerPrivate::ActionManagerPrivate
 */
ActionManagerPrivate::ActionManagerPrivate() :
    q(NULL),
    m_Initialized(false)
{
}

/*!
   \internal
   \brief ActionManagerPrivate::contextChanged
 */
void ActionManagerPrivate::contextChanged()
{
    Context *context = qobject_cast<Context *>(QObject::sender());
    if(!context) {
        return;
    }

    const bool enabled = context->isEnabled();
    const bool visible = context->isVisible();

    foreach(QAction *action, m_Actions.values(context)) {
        action->setEnabled(enabled);
        action->setVisible(visible);
    }

    foreach(QShortcut *shortcut, m_Shortcuts.values(context)) {
        shortcut->setEnabled(enabled);
    }

}

/*!
   \internal
   \brief ActionManagerPrivate::actionDestroyed
   \param object
 */
void ActionManagerPrivate::actionDestroyed(QObject *object)
{
    Q_ASSERT(object);
    if(QAction *action = qobject_cast<QAction *>(object)) {
        m_Actions.remove(NULL, action);
    }
}

/*!
   \internal
   \brief ActionManagerPrivate::shortcutDestroyed
   \param object
 */
void ActionManagerPrivate::shortcutDestroyed(QObject *object)
{
    Q_ASSERT(object);
    if(QShortcut *shortcut = qobject_cast<QShortcut *>(object)) {
        m_Shortcuts.remove(NULL, shortcut);
    }
}

} // namespace ActionManager
} // namespace Core
