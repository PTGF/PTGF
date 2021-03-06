/*!
   \file ViewManager.cpp
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

#include "ViewManagerPrivate.h"

#include <QAbstractItemView>
#include <QDebug>

#include <PluginManager/PluginManager.h>

#include "IViewFactory.h"

namespace Core {
namespace ViewManager {

ViewManager &ViewManager::instance()
{
    static ViewManager m_Instance;
    return m_Instance;
}

/*!
   \internal
   \brief ViewManager::ViewManager
 */
ViewManager::ViewManager() :
    QObject(0),
    d(new ViewManagerPrivate)
{
    d->q = this;
}

/*!
   \internal
   \brief ViewManager::~ViewManager
 */
ViewManager::~ViewManager()
{
}

/*!
   \internal
   \brief ViewManager::initialize
   \return true if successful
 */
bool ViewManager::initialize()
{
    if(d->m_Initialized) {
        return false;
    }

    try {

        PluginManager::PluginManager &pluginManager = PluginManager::PluginManager::instance();
        pluginManager.addObject(this);

        /* Check the object pool for anything we should manage */
        foreach(QObject *object, pluginManager.allObjects()) { d->pluginObjectRegistered(object); }
        connect(&pluginManager, SIGNAL(objectAdded(QObject*)), d.data(), SLOT(pluginObjectRegistered(QObject*)));
        connect(&pluginManager, SIGNAL(objectRemoving(QObject*)), d.data(), SLOT(pluginObjectDeregistered(QObject*)));

    } catch(...) {
        return false;
    }

    return d->m_Initialized = true;
}

/*!
   \internal
   \brief ViewManager::shutdown
 */
void ViewManager::shutdown()
{
    if(!d->m_Initialized) {
        return;
    }

    // ...
}

/*!
   \fn ViewManager::viewNames

   \brief Returns a list of names of views that can handle the supplied model.

   A NULL model will return all names.

   \param model
   \return
   \sa viewWidget
 */
QStringList ViewManager::viewNames(QAbstractItemModel *model)
{
    QStringList nameList;
    foreach(IViewFactory *viewPlugin, d->m_viewFactories.values()) {
        if(model) {
            if(viewPlugin->viewHandles(model)) {
                nameList.append(viewPlugin->viewName());
            }
        } else {
            nameList.append(viewPlugin->viewName());
        }
    }
    return nameList;
}

/*!
   \fn ViewManager::viewWidget

   \brief Creates the specified view using the supplied model, and returns it.

   You should not create a view from it's constructor directly, but rather from its associated IViewFactory
   through the ViewManager::viewWidget method.  For example:

   \code
   // Grab the instance of the ViewManager
   Core::ViewManager::ViewManager &viewManager = Core::ViewManager::ViewManager::instance();

   // Create your own data model
   QAbstractItemModel *model = new DataModel(data);

   // Check to see if this view can handle your model
   if(viewManager.viewNames(model).contains("A View Name")) {

       // Create the view using the model, and display it to the user
       QAbstractItemView *view = viewManager.viewWidget("A View Name", model);
       this->layout()->insertWidget(view);

   }
   \endcode

   If the creation should fail for some reason, a NULL pointer will be returned.  Failure can occur when: a name was
   not specified; a named view doesn't exist; a named view cannot handle the specified model.  A NULL model will not
   be checked, and will always return a view if it exists.

   The returned object does not have a parent, and should be immediately reparented, or deconstruction should be
   handled manually.

   \param name
   \param model
   \return
   \sa viewNames IViewFactory
 */
QAbstractItemView *ViewManager::viewWidget(QString name, QAbstractItemModel *model)
{
    if(name.isEmpty()) {
        return NULL;
    }

    IViewFactory *viewPlugin = d->m_viewFactories.value(name, NULL);

    if(model != NULL && !viewPlugin->viewHandles(model)) {
        return NULL;
    }

    return viewPlugin->viewWidget(model);
}




/***** PRIVATE IMPLEMENTATION *****/
ViewManagerPrivate::ViewManagerPrivate() :
    QObject(NULL),
    m_Initialized(false)
{
}

void ViewManagerPrivate::registerViewFactory(IViewFactory *viewFactory)
{
    if(!m_viewFactories.contains(viewFactory->viewName())) {
        m_viewFactories.insert(viewFactory->viewName(), viewFactory);
    }
}

void ViewManagerPrivate::deregisterViewFactory(IViewFactory *viewFactory)
{
    if(m_viewFactories.contains(viewFactory->viewName())) {
        m_viewFactories.remove(viewFactory->viewName());
    }
}

void ViewManagerPrivate::pluginObjectRegistered(QObject *object)
{
    if(IViewFactory *viewFactory = qobject_cast<IViewFactory *>(object)) {
        registerViewFactory(viewFactory);
    }
}

void ViewManagerPrivate::pluginObjectDeregistered(QObject *object)
{
    if(IViewFactory *viewFactory = qobject_cast<IViewFactory *>(object)) {
        deregisterViewFactory(viewFactory);
    }
}



} // namespace ViewManager
} // namespace Core
