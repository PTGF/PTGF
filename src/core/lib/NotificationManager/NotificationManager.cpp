#include "NotificationManager.h"

#include <CoreWindow/CoreWindow.h>
#include <PrettyWidgets/ConsoleWidget.h>

#include <CoreWindow/CoreWindow.h>

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
    m_ConsoleWidget(new ConsoleWidget())
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
    try {


        //! \todo Register actions for hiding and showing the message console


        CoreWindow::CoreWindow &coreWindow = CoreWindow::CoreWindow::instance();

        QDockWidget *dockWidget = new QDockWidget(tr("Notification Message Console"));
        dockWidget->setWidget(m_ConsoleWidget);
        coreWindow.addDockWidget(Qt::BottomDockWidgetArea, dockWidget);

        connect(dockWidget, SIGNAL(visibilityChanged(bool))

        m_ConsoleWidget->setEventLevelColor((int)QtDebugMsg, Qt::darkGreen);
        m_ConsoleWidget->setEventLevelColor((int)QtWarningMsg, Qt::darkYellow);
        m_ConsoleWidget->setEventLevelColor((int)QtCriticalMsg, Qt::darkRed);
        m_ConsoleWidget->setEventLevelColor((int)QtFatalMsg, Qt::darkRed);


        m_LogFile.setFileName(QString("%1/%2.txt")
                              .arg(QDesktopServices::storageLocation(QDesktopServices::DataLocation))
                              .arg(QDateTime::currentDateTime().toUTC().toString(QString("yyyyMMddhhmmsszzz"))));

        qInstallMsgHandler(qMessageHandler);

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
