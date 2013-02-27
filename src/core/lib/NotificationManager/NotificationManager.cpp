#include "NotificationManager.h"

#include <CoreWindow/CoreWindow.h>
#include <PrettyWidgets/ConsoleWidget.h>

#include <CoreWindow/CoreWindow.h>
#include <PluginManager/PluginManager.h>

namespace Core {
namespace NotificationManager {

NotificationManager &NotificationManager::instance()
{
    static NotificationManager m_Instance;
    return m_Instance;
}

NotificationManager::NotificationManager() :
    QObject(0),
    m_Initialized(false),
    m_StdOut(stdout, QIODevice::WriteOnly),
    m_StdError(stderr, QIODevice::WriteOnly),
    m_ConsoleWidget(NULL)
{
}

NotificationManager::~NotificationManager()
{
    if(m_LogFile.isOpen()) {
        m_LogFile.close();
    }
}

bool NotificationManager::initialize()
{
    Q_ASSERT(!m_Initialized);

    try {

        CoreWindow::CoreWindow &coreWindow = CoreWindow::CoreWindow::instance();
        PluginManager::PluginManager &pluginManager = PluginManager::PluginManager::instance();


        // Setup dock widget for message console
        QDockWidget *dockWidget = new QDockWidget(tr("Message Console"), &coreWindow);

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


        pluginManager.addObject(this);

    } catch(...) {
        return false;
    }

    return m_Initialized = true;
}

bool NotificationManager::initialized()
{
    return m_Initialized;
}

void NotificationManager::shutdown()
{
    Q_ASSERT(m_Initialized);

    qInstallMsgHandler(0);

    if(m_LogFile.isOpen()) {
        m_LogFile.close();
    }
}


void NotificationManager::writeToLogFile(const int &level, QString message)
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

void NotificationManager::qMessageHandler(QtMsgType type, const char *message)
{
    QString msg;
    int level = 0;

    switch (type) {
    case QtDebugMsg:
        msg = tr("Debug: %1\n").arg(message);
        level = (int)QtDebugMsg;
        break;
    case QtWarningMsg:
        msg = tr("Warning: %1\n").arg(message);
        level = (int)QtWarningMsg;
        break;
    case QtCriticalMsg:
        msg = tr("Critical: %1\n").arg(message);
        level = (int)QtCriticalMsg;
        break;
    case QtFatalMsg:
        msg = tr("Fatal: %1\n").arg(message);
        level = (int)QtFatalMsg;
        break;
    default:
        msg = tr("Unknown: %1\n").arg(message);
        break;
    }

    NotificationManager::instance().writeToLogFile(level, msg);

    if(type == QtFatalMsg) {
        abort();
    }
}


} // namespace NotificationManager
} // namespace Core
