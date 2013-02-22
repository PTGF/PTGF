#include "WindowManager.h"

#include <CoreWindow/CoreWindow.h>
#include <PluginManager/PluginManager.h>

#include "IMainWindow.h"
#include "AboutDialog.h"

namespace Core {
namespace WindowManager {


WindowManager &WindowManager::instance()
{
    static WindowManager m_Instance;
    return m_Instance;
}

WindowManager::WindowManager() :
    QObject(0),
    m_Initialized(false),
    m_AboutPage(new QAction(this))
{
    AboutDialog::splash();
}

WindowManager::~WindowManager()
{
    if(m_AboutPage) {
        delete m_AboutPage;
    }
}


bool WindowManager::initialize()
{
    try {

        /*** Register our menu structure ***/
        CoreWindow::CoreWindow &coreWindow = CoreWindow::CoreWindow::instance();
        foreach(QAction *action, coreWindow.menuBar()->actions()) {
            if(action->text() == tr("Help")) {
                Q_ASSERT(m_AboutPage);
                m_AboutPage->setText(tr("About"));
                m_AboutPage->setToolTip(tr("Displays the about dialog"));
                m_AboutPage->setIcon(QIcon(":/CoreWindow/app.png"));
                m_AboutPage->setIconVisibleInMenu(true);
                connect(m_AboutPage, SIGNAL(triggered()), this, SLOT(aboutDialog()));
                action->menu()->addAction(m_AboutPage);
            }
        }

        /* Check the object pool for anything we should manage */
        PluginManager::PluginManager &pluginManager = PluginManager::PluginManager::instance();
        foreach(QObject *object, pluginManager.allObjects()) { pluginObjectRegistered(object); }
        connect(&pluginManager, SIGNAL(objectAdded(QObject*)), this, SLOT(pluginObjectRegistered(QObject*)));
        connect(&pluginManager, SIGNAL(objectRemoving(QObject*)), this, SLOT(pluginObjectDeregistered(QObject*)));

    } catch(...) {
        return false;
    }

    return m_Initialized = true;
}

bool WindowManager::initialized()
{
    return m_Initialized;
}

void WindowManager::shutdown()
{
    Q_ASSERT(m_Initialized);
}


//! \returns sorted list of all registered MainWindows
QList<IMainWindow *> WindowManager::mainWindows()
{
    // Resort by priority
    if(m_MainWindows.count() > 1) {
        qSort(m_MainWindows.begin(), m_MainWindows.end(), ascending);
    }

    return m_MainWindows;
}

//! \returns sorted list of all registered about dialog widgets
QList<QWidget *> WindowManager::aboutWidgets()
{
    QList<QWidget *> aboutWidgets;

    foreach(IMainWindow *mainWindow, mainWindows()) {
        QWidget *aboutWidget = mainWindow->createAboutWidget();
        if(aboutWidget && !aboutWidgets.contains(aboutWidget)) {
            aboutWidgets.append(aboutWidget);
        }
    }

    return aboutWidgets;
}


void WindowManager::aboutDialog()
{
//    try {
        AboutDialog aboutDialog;
        aboutDialog.exec();
//    } catch(QString err) {
//        Core::CoreWindow::CoreWindow::instance().notify(tr("Failed to open about dialog: %1").arg(err), NotificationWidget::Critical);
//    } catch(...) {
//        using namespace Core::CoreWindow;
//        Core::CoreWindow::CoreWindow::instance().notify(tr("Failed to open about dialog."), NotificationWidget::Critical);
//    }
}

void WindowManager::pluginObjectRegistered(QObject *object)
{
    IMainWindow *mainWindow = qobject_cast<IMainWindow *>(object);
    if(mainWindow) registerMainWindow(mainWindow);
}

void WindowManager::pluginObjectDeregistered(QObject *object)
{
    IMainWindow *mainWindow = qobject_cast<IMainWindow *>(object);
    if(mainWindow) deregisterMainWindow(mainWindow);
}

void WindowManager::registerMainWindow(IMainWindow *window)
{
    m_MainWindows.append(window);

    connect(window, SIGNAL(active()), this, SLOT(windowActivated()));

    CoreWindow::CoreWindow &coreWindow = CoreWindow::CoreWindow::instance();

    coreWindow.setWindowTitle(window->mainWindowWidget()->windowTitle());
    coreWindow.setWindowIcon(window->mainWindowWidget()->windowIcon());

    /*** Set our main widget in the main window ***/
    coreWindow.addCentralWidget(window->mainWindowWidget(),
                                window->mainWindowPriority(),
                                window->mainWindowName(),
                                window->mainWindowIcon());


}

void WindowManager::deregisterMainWindow(IMainWindow *window)
{
    if(m_MainWindows.contains(window)) {
        m_MainWindows.removeOne(window);
        disconnect(window, SIGNAL(active()), this, SLOT(windowActivated()));

        CoreWindow::CoreWindow &coreWindow = CoreWindow::CoreWindow::instance();
        coreWindow.removeCentralWidget(window->mainWindowWidget());
    }
}

void WindowManager::windowActivated()
{
    IMainWindow *mainWindow = qobject_cast<IMainWindow *>(sender());

    if(!mainWindow) {
        return;
    }

    //! \todo Hide all windows' actions

    //! \todo Show the activated window's actions
}


/*! Used by qSort to sort the m_MainWindows QList in order of priority
 *  \sa qSort
 */
bool WindowManager::ascending(IMainWindow *left, IMainWindow *right)
{
    return left->mainWindowPriority() < right->mainWindowPriority();
}


} // namespace WindowManager
} // namespace Core
