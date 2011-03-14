/*!
   \file PluginManager.cpp
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

#ifdef QT_DEBUG
  #include <QtDebug>
#endif

#include <QDir>
#include <QPluginLoader>
#include "PluginManager.h"

namespace Core {
namespace PluginManager {

/*!
   \class PluginManager
   \brief The PluginManager class is part of the core framework libraries, and
          manages the loading of and interaction between dynamic extensions
          and plugins.  This includes managing an "object pool."

          singleton class
 */

/*!
   \fn PluginManager::getObjects() const
   \returns A QList populated with the given object type that has been stored in
            the object pool.
 */

PluginManager *m_Instance;

/*!
   \fn PluginManager::instance()
   \brief Access to the singleton instance of this class
   \returns A pointer to the singleton instance of this class
 */
PluginManager *PluginManager::instance()
{
    return m_Instance ? m_Instance : m_Instance = new PluginManager();
}

/*!
   \fn PluginManager::PluginManager()
   \brief Constructor
   \internal
 */
PluginManager::PluginManager() : QObject(0)
{
    readSettings();
}

/*!
   \fn PluginManager::PluginManager()
   \brief Destructor
   \internal
 */
PluginManager::~PluginManager()
{
    //Should we be doing this in the reverse order?
    foreach (QObject *obj, m_Objects)
        delObject(obj);

    writeSettings();
}

/*!
   \fn PluginManager::readSettings()
   \brief Load settings from the SettingManager.
   \internal
 */
void PluginManager::readSettings()
{
    SettingManager::SettingManager *settings =
            SettingManager::SettingManager::instance();

    settings->beginGroup("PluginManager");

    m_PluginPath = settings->value("PluginPath").toString();

    settings->endGroup();
}

/*!
   \fn PluginManager::writeSettings()
   \brief Stores settings in the SettingManager for later retrieval.
   \internal
 */
void PluginManager::writeSettings()
{
    SettingManager::SettingManager *settings =
            SettingManager::SettingManager::instance();

    settings->beginGroup("PluginManager");

    settings->setValue("PluginPath", m_PluginPath);

    settings->endGroup();
}

/*!
   \fn PluginManager::loadPlugins()
   \brief Loads plugins from a location defined in the SettingManager

   Emits pluginLoaded() signal after loading \b each plugin
 */
void PluginManager::loadPlugins()
{
    //TODO: Get the directory from the SettingManager
    QDir pluginDir("C:/Qt/projects/openspeedshop/build/lib/plugin");

    foreach (QString fileName, pluginDir.entryList(QDir::Files)) {
        QString filePath = pluginDir.absoluteFilePath(fileName);

#ifdef QT_DEBUG
        qDebug() << __FILE__ << __LINE__ << "Attempting to load plugin:" << fileName;
#endif

        QPluginLoader pluginLoader(filePath);
        QObject *object = pluginLoader.instance();

        if (object) {
            IPlugin *plugin = qobject_cast<IPlugin *>(object);

            if (plugin) {
                PluginWrapper *wrapper = new PluginWrapper(plugin, filePath, this);
                m_Plugins.append(wrapper);

#ifdef QT_DEBUG
                qDebug() << __FILE__ << __LINE__ << "Loaded plugin:" << fileName;
#endif

                emit pluginLoaded(plugin);
            }
        }
    }

    initializePlugins();
}

/*!
   \fn PluginManager::initializePlugins()
   \brief Helper function that builds a dependency list, checks the
    dependencies for circular references, and then calls the init function for
    the plugins in the proper order.

    The algorithm used here was inspired from "Algorithms in C++" by Robert
    Sedgewick, section 19.6 --specifically the use of a queue.
 */
void PluginManager::initializePlugins()
{
    // Generate a temporary directed graph of plugins and dependencies
    QMap<QString, QStringList *> dag;
    foreach(PluginWrapper *plugin, m_Plugins) {
        QStringList *names = new QStringList();
        foreach(Dependency dep, plugin->dependencies()) {
            names->append(dep.name);
        }

        dag.insert(plugin->name(), names);
    }

    //TODO: Check plugin versions

    // Create a queue with the proper initialization ordering
    QQueue<PluginWrapper *> queue;
    while(dag.count()) {

        // Iterate through the plugins and see if any have no dependencies
        QString next;
        foreach(QString key, dag.keys()) {
            if(!dag[key]->count()) {
                next = key;
                break;
            }
        }

        // Are we done, or is there a cyclic dependency?
        if(next.isEmpty()) {
            if(dag.count())
                throw QString("Cyclic plugin dependency found");
            else
                break;
        }

        // Add the plugin with no dependencies to the queue
        queue.enqueue(findPlugin(next));

        // Remove it from all of the lists
        dag.remove(next);
        foreach(QStringList *value, dag.values()) {
            value->removeAll(next);
        }
    }

    /* Intialize via the queue */
    QString *err = new QString();
    QStringList args;
    while(queue.count()) {
        PluginWrapper *plugin = queue.dequeue();
        if( plugin->status() == PluginStatus_Loaded && plugin->initialize(args, err) ) {
            emit pluginInitialized(plugin);
        }
    }
}

/*!
   \fn PluginManager::findPlugin()
   \brief Helper function
   \param name Name of the plugin to search for and return
   \returns Pointer to the plugin specified by name
 */
PluginWrapper *PluginManager::findPlugin(QString name)
{
    foreach(PluginWrapper *plugin, m_Plugins) {
        if(plugin->name() == name)
            return plugin;
    }
    return 0;
}

/*!
   \fn PluginManager::addObject()
   \brief Adds an object to the manager for later retrieval. This is typically
          used by plugins as they are initialized to store factory classes.

   Emits objectAdding() signal before adding
   Emits objectAdded() signal after adding
   \param object The object to be stored
 */
void PluginManager::addObject(QObject *object)
{
    emit objectAdding(object);
    m_Objects.append(object);
    emit objectAdded(object);
}

/*!
   \fn PluginManager::delObject()
   \brief Removes a previously stored object from the manager.

   Emits objectRemoving() signal before removal
   Emits objectRemoved() signal after removal
   \param object The object to be removed
 */
bool PluginManager::delObject(QObject *object)
{
    emit objectRemoving(object);
    int RetVal;
    RetVal = m_Objects.removeAll(object);
    emit objectRemoved(object);
    return (RetVal);
}

}}
