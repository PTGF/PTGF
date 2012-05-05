/*!
   \file PluginManager.cpp
   \author Dane Gardner <dane.gardner@gmail.com>
   \version

   \section LICENSE
   This file is part of the Parallel Tools GUI Framework (PTGF)
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

#include "PluginManager.h"

#include <SettingManager/SettingManager.h>
#include <MainWindow/MainWindow.h>

#include "PluginSettingPage.h"
#include "PluginWrapper.h"

#ifdef PLUGINMANAGER_DEBUG
  #include <QtDebug>
#endif

namespace Core {
namespace PluginManager {

/*!
   \class Core::PluginManager::PluginManager
   \brief Part of the core framework libraries, and manages the loading of and interaction between dynamic extensions and
          plugins.  This includes managing an "object pool."

          singleton class
 */

/*! \fn PluginManager::getObjects() const
    \brief Returns a QList populated with the given object type that has been stored in the object pool. */

/*! \fn PluginManager::pluginLoaded()
    \brief Emitted when a plugin has been loaded successfully.
    Before it has been initialized. */

/*! \fn PluginManager::pluginInitialized()
    \brief Emitted when a plugin has been successfully intialized.
    This is not emitted for plugins that fail to initialize. */

/*! \fn PluginManager::pluginShutdown()
    \brief Emitted when a plugin has been shut down. */

/*! \fn PluginManager::objectAdding()
    \brief Emitted immediately before the object is added to the object pool. */

/*! \fn PluginManager::objectAdded()
    \brief Emitted immediately after the object is added to the object pool. */

/*! \fn PluginManager::objectRemoving()
    \brief Emitted immediately before the object is removed from the object pool. */

/*! \fn PluginManager::objectRemoved()
    \brief Emitted immediately after the object is removed from the object pool. */


/*!
   \fn PluginManager::instance()
   \brief Access to the singleton instance of this class.
   \returns A reference to the singleton instance of this class.
 */
PluginManager &PluginManager::instance()
{
    static PluginManager m_Instance;
    return m_Instance;
}

/*!
   \fn PluginManager::PluginManager()
   \brief Constructor
 */
PluginManager::PluginManager() :
    QObject(0),
    m_Objects()
{
    m_Initialized = false;
}

/*!
   \fn PluginManager::PluginManager()
   \brief Destructor
 */
PluginManager::~PluginManager()
{
    qSort(m_Plugins.begin(), m_Plugins.end(), descending);
    qDeleteAll(m_Plugins);

    writeSettings();
}

/*!
   \fn PluginManager::initialize()
   \brief Initializes this class after it has been constructed.
   This pattern allows the class to perform any operations after a class (that this object is dependent upon) has been
   constructed.
   \returns true if successful
   \sa initialized(), shutdown()
 */
bool PluginManager::initialize()
{
    try {

        readSettings();

        Core::PluginManager::PluginManager &pluginManager = Core::PluginManager::PluginManager::instance();
        pluginManager.addObject(this);                         /* Register ourselves as an ISettingPageFactory */

        MainWindow::MainWindow &mainWindow = MainWindow::MainWindow::instance();
        foreach(QAction *action, mainWindow.menuBar()->actions()) {
            if(action->text() == tr("Help")) {
                QAction *pluginDialog = new QAction("Plugins", this);
                pluginDialog->setToolTip(tr("View loaded plugins"));
                connect(pluginDialog, SIGNAL(triggered()), this, SLOT(pluginDialog()));
                action->menu()->addAction(pluginDialog);
            }
        }

    } catch(...) {
        return false;
    }

    return m_Initialized = true;
}

/*!
   \fn PluginManager::initialized()
   \brief Returns a boolean value indicating whether this instance has been initialized or not.
   \sa initialize()
 */
bool PluginManager::initialized()
{
    return m_Initialized;
}

/*!
   \fn PluginManager::shutdown()
   \brief Notifies the instance that it should perform any clean-up operations before destruction.
   This class is called manually, before the application is closed.  It will occur before destruction of the instance.
   \sa initialize()
 */
void PluginManager::shutdown()
{
    qSort(m_Plugins.begin(), m_Plugins.end(), ascending);
    foreach(IPlugin *plugin, m_Plugins) {

#ifdef PLUGINMANAGER_DEBUG
        qDebug() << __FILE__ << __LINE__ << "Shutting down plugin:" << plugin->name();
#endif

        plugin->shutdown();
    }
}

/*!
   \fn PluginManager::readSettings()
   \brief Load settings from the SettingManager.
 */
void PluginManager::readSettings()
{
    SettingManager::SettingManager &settingManager = SettingManager::SettingManager::instance();
    settingManager.beginGroup("PluginManager");

    m_PluginPath = settingManager.value("PluginPath").toString();

    //! \todo Check for environment variable
    //! \todo Maybe this should be a list of paths to check

    if(!QFile::exists(m_PluginPath)) {
        QDir pluginPath(QApplication::applicationDirPath());

#ifdef WIN32
        m_PluginPath = pluginPath.absolutePath();
#else
        if(pluginPath.cd("../lib")) {
            m_PluginPath = pluginPath.absolutePath();
        }
#endif

    }

    settingManager.endGroup();
}

/*!
   \fn PluginManager::writeSettings()
   \brief Stores settings in the SettingManager for later retrieval.
 */
void PluginManager::writeSettings()
{
    SettingManager::SettingManager &settingManager = SettingManager::SettingManager::instance();
    settingManager.beginGroup("PluginManager");

    settingManager.setValue("PluginPath", m_PluginPath);

    settingManager.endGroup();
}

/*!
   \fn PluginManager::loadPlugins()
   \brief Loads plugins from a location defined in the SettingManager

   \par Emits
        pluginLoaded() signal after loading \b each plugin
 */
void PluginManager::loadPlugins()
{
    QDir pluginDir(m_PluginPath);
    foreach (QString fileName, pluginDir.entryList(QDir::Files)) {
        QString filePath = pluginDir.absoluteFilePath(fileName);

#ifdef PLUGINMANAGER_DEBUG
        qDebug() << __FILE__ << __LINE__ << "Attempting to load plugin:" << fileName;
#endif

        QPluginLoader pluginLoader(filePath);
        QObject *object = pluginLoader.instance();

        if (object) {
#ifdef PLUGINMANAGER_DEBUG
                qDebug() << __FILE__ << __LINE__ << "Plugin was object:" << fileName;
#endif
            IPlugin *plugin = qobject_cast<IPlugin *>(object);
            if (plugin) {
                PluginWrapper *wrapper = new PluginWrapper(plugin, filePath, this);
                m_Plugins.append(wrapper);

#ifdef PLUGINMANAGER_DEBUG
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
   \brief Helper function that builds a dependency list.

    It checks the dependencies for circular references, and then calls the init function for
    the plugins in the proper order.

    \note The algorithm used here was inspired from "Algorithms in C++" by Robert Sedgewick,
          section 19.6 --specifically the use of a queue.
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

    //! \todo Check plugin versions

    // Create a queue with the proper initialization ordering
    int priority = 0;
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
        PluginWrapper *plugin = findPlugin(next);
        plugin->setPriority(priority++);

        // Remove it from all of the lists
        dag.remove(next);
        foreach(QStringList *value, dag.values()) {
            value->removeAll(next);
        }
    }

    qSort(m_Plugins.begin(), m_Plugins.end(), descending);

    /* Intialize via the queue */
    QString *err = NULL;
    QStringList args;
    foreach(PluginWrapper *plugin, m_Plugins) {

#ifdef PLUGINMANAGER_DEBUG
        qDebug() << __FILE__ << __LINE__ << "Initializing plugin:" << plugin->name();
#endif

        if( plugin->state() == PluginWrapper::State_Loaded && plugin->initialize(args, err) ) {
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

   \par Emits
        objectAdding() signal before adding
        objectAdded() signal after adding
   \param object The object to be stored
 */
void PluginManager::addObject(QObject *object)
{
    emit objectAdding(object);
    m_Objects.append(object);
    emit objectAdded(object);
}

QObjectList PluginManager::allObjects() const
{
    return m_Objects;
}


/*!
   \fn PluginManager::delObject()
   \brief Removes a previously stored object from the manager.

   \par Emits
       objectRemoving() signal before removal
       objectRemoved() signal after removal
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

/*!
   \fn PluginManager::pluginDialog()
   \brief Slot connected to menu item.

   When this slot is called, a QDialog is created as a wrapper to the plugin widget that is
   registered as a settings page.

   \note This only allows viewing of the plugins that are loaded; user changes should only
         be performed through the settings dialog.
 */
void PluginManager::pluginDialog()
{
    // Wrapped in a QDialog because this is also registered as a setting page

    QDialog *dialog = new QDialog(&MainWindow::MainWindow::instance());
    QGridLayout *layout = new QGridLayout(dialog);
    layout->addWidget(new PluginSettingPage(m_Plugins, dialog));
    dialog->setLayout(layout);
    dialog->resize(640, 480);
    dialog->setWindowIcon(QIcon(":/PluginManager/plugin.svg"));
    dialog->exec();

    delete dialog;
}

/*!
   \fn PluginManager::ascending()
   \brief Works with qSort to sort an iterable of plugin wrapper pointers
   \returns A boolean value for comparison of the two parameters that sorts in ascending
            order.
 */
bool PluginManager::ascending(PluginWrapper *left, PluginWrapper *right)
{
    return left->priority() > right->priority();
}

/*!
   \fn PluginManager::descending()
   \brief Works with qSort to sort an iterable of plugin wrapper pointers
   \returns A boolean value for comparison of the two parameters that sorts in descending
            order.
 */
bool PluginManager::descending(PluginWrapper *left, PluginWrapper *right)
{
    return left->priority() < right->priority();
}

/* BEGIN ISettingPageFactory */
/*!
   \fn MainWPluginManagerindow::settingPageIcon()
   \brief Reimplemented from ISettingPageFactory.
   \sa Core::SettingManager::ISettingPageFactory::settingPageIcon()
 */
QIcon PluginManager::settingPageIcon()
{
    return QIcon(":/PluginManager/plugin.svg");
}

/*!
   \fn PluginManager::settingPageName()
   \brief Reimplemented from ISettingPageFactory.
   \sa Core::SettingManager::ISettingPageFactory::settingPageName()
 */
QString PluginManager::settingPageName()
{
    return tr("Plugins");
}

/*!
   \fn PluginManager::settingPagePriority()
   \brief Reimplemented from ISettingPageFactory.
   \sa Core::SettingManager::ISettingPageFactory::settingPagePriority()
 */
int PluginManager::settingPagePriority()
{
    return 50;
}

/*!
   \fn PluginManager::createSettingPage()
   \brief Reimplemented from ISettingPageFactory.
   \sa Core::SettingManager::ISettingPageFactory::createSettingPage()
 */
Core::SettingManager::ISettingPage *PluginManager::createSettingPage()
{
    return new PluginSettingPage();
}
/* END ISettingPageFactory */

} // namespace PluginManager
} // namespace Core
