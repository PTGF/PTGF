#include "NotificationManager.h"
#include "NotificationManagerPrivate.h"

#include <CoreWindow/CoreWindow.h>
#include <PrettyWidgets/ConsoleWidget.h>

#include <CoreWindow/CoreWindow.h>
#include <PluginManager/PluginManager.h>

namespace Core {
namespace NotificationManager {


/***** PUBLIC IMPLEMENTATION *****/

NotificationManagerPrivate *NotificationManager::d = &NotificationManagerPrivate::instance();

NotificationManager::NotificationManager()
{
}

bool NotificationManager::initialize()
{
    if(!d->initialized()) {
        return d->initialize();
    }
    return false;
}

void NotificationManager::shutdown()
{
    if(d->initialized()) {
        d->shutdown();
    }
}


void NotificationManager::writeToLogFile(const int &level, QString message)
{
    d->writeToLogFile(level, message);
}

/*!
   \fn NotificationWidget::notify()
   \returns NotificationWidget, which is owned and destroyed by CoreWindow
 */
NotificationWidget *NotificationManager::notify(const QString &text,
                                                NotificationWidget::Icon icon,
                                                NotificationWidget::StandardButtons buttons,
                                                const QObject *reciever, const char *member)
{
    return d->notify(text, icon, buttons, reciever, member);
}



/***** PRIVATE IMPLEMENTATION *****/

NotificationManagerPrivate &NotificationManagerPrivate::instance()
{
    static NotificationManagerPrivate m_Instance;
    return m_Instance;
}

NotificationManagerPrivate::NotificationManagerPrivate() :
    QObject(0),
    m_Initialized(false),
    m_StdOut(stdout, QIODevice::WriteOnly),
    m_StdError(stderr, QIODevice::WriteOnly),
    m_ConsoleWidget(NULL)
{
}

NotificationManagerPrivate::~NotificationManagerPrivate()
{
    if(m_LogFile.isOpen()) {
        m_LogFile.close();
    }
}

bool NotificationManagerPrivate::initialize()
{
    Q_ASSERT(!m_Initialized);

    try {

        CoreWindow::CoreWindow &coreWindow = CoreWindow::CoreWindow::instance();


        // Setup dock widget for message console
        QDockWidget *dockWidget = new QDockWidget(tr("Message Console"), &coreWindow);
        dockWidget->setObjectName("MessageConsoleDockWidget");
        dockWidget->hide();

        m_ConsoleWidget = new ConsoleWidget(dockWidget);
        m_ConsoleWidget->setEventLevelColor((int)QtDebugMsg, Qt::darkGreen);
        m_ConsoleWidget->setEventLevelColor((int)QtWarningMsg, Qt::darkYellow);
        m_ConsoleWidget->setEventLevelColor((int)QtCriticalMsg, Qt::darkRed);
        m_ConsoleWidget->setEventLevelColor((int)QtFatalMsg, Qt::darkRed);
        dockWidget->setWidget(m_ConsoleWidget);

        coreWindow.addDockWidget(Qt::BottomDockWidgetArea, dockWidget);

        foreach(QAction *action, coreWindow.menuBar()->actions()) {
            if(action->text() == tr("Window")) {
                action->menu()->addAction(dockWidget->toggleViewAction());
            }
        }


        // Create log file and register handler for qDebug() messages
        m_LogFile.setFileName(QString("%1/%2.txt")
                              .arg(QDesktopServices::storageLocation(QDesktopServices::DataLocation))
                              .arg(QDateTime::currentDateTime().toUTC().toString(QString("yyyyMMddhhmmsszzz"))));

        qInstallMsgHandler(qMessageHandler);


        Core::PluginManager::PluginManager::instance().addObject(this);

    } catch(...) {
        return false;
    }

    return m_Initialized = true;
}

bool NotificationManagerPrivate::initialized()
{
    return m_Initialized;
}

void NotificationManagerPrivate::shutdown()
{
    Q_ASSERT(m_Initialized);

    qInstallMsgHandler(0);

    if(m_LogFile.isOpen()) {
        m_LogFile.close();
    }
}


void NotificationManagerPrivate::writeToLogFile(const int &level, QString message)
{
    if(!m_LogFile.isOpen()) {
        m_LogFile.open(QIODevice::Append | QIODevice::Text);
        m_LogFileStream.setDevice(&m_LogFile);
    }

    QDateTime currentTime = QDateTime::currentDateTime().toUTC();

    if(m_LogFile.isOpen()) {
        QString outputString = QString("%1 - %2")
                .arg(currentTime.toString("yyyy-MM-dd hh:mm:ss.zzz"))
                .arg(message);

        m_LogFileStream << outputString << endl;

        m_StdError << outputString << endl;

        m_ConsoleWidget->messageEvent(level, outputString);
    }
}

void NotificationManagerPrivate::qMessageHandler(QtMsgType type, const char *message)
{
    QString msg;
    int level = 0;

    switch (type) {
    case QtDebugMsg:
        msg = tr("Debug: %1").arg(message);
        level = (int)QtDebugMsg;
        break;
    case QtWarningMsg:
        msg = tr("Warning: %1").arg(message);
        level = (int)QtWarningMsg;
        NotificationManagerPrivate::instance().notify(msg, NotificationWidget::Warning);
        break;
    case QtCriticalMsg:
        msg = tr("Critical: %1").arg(message);
        level = (int)QtCriticalMsg;
        NotificationManagerPrivate::instance().notify(msg, NotificationWidget::Critical);
        break;
    case QtFatalMsg:
        msg = tr("Fatal: %1").arg(message);
        level = (int)QtFatalMsg;
        break;
    default:
        msg = tr("Unknown: %1").arg(message);
        break;
    }

    NotificationManagerPrivate::instance().writeToLogFile(level, msg);

    if(type == QtFatalMsg) {
        abort();
    }
}

NotificationWidget *NotificationManagerPrivate::notify(const QString &text,
                                       NotificationWidget::Icon icon,
                                       NotificationWidget::StandardButtons buttons,
                                       const QObject *reciever, const char *member)
{
#ifdef COREWINDOW_DEBUG
    qDebug() << __FILE__ << __LINE__ << "\tCoreWindow::notify";
#endif

    CoreWindow::CoreWindow &coreWindow = CoreWindow::CoreWindow::instance();

    NotificationWidget *notificationWidget =
            new NotificationWidget(text, icon, buttons, reciever, member, &coreWindow);

    coreWindow.addNotificationWidget(notificationWidget);
    notificationWidget->setFocus();

    return notificationWidget;
}


} // namespace NotificationManager
} // namespace Core
