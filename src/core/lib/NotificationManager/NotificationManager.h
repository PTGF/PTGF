#ifndef CORE_NOTIFICATIONMANAGER_NOTIFICATIONMANAGER_H
#define CORE_NOTIFICATIONMANAGER_NOTIFICATIONMANAGER_H

#include <QtCore>
#include <QtGui>

#include <NotificationManager/NotificationManagerLibrary.h>
#include <NotificationManager/NotificationWidget.h>

class ConsoleWidget;

namespace Core {
namespace NotificationManager {

class NOTIFICATIONMANAGER_EXPORT NotificationManager : public QObject
{
    Q_OBJECT
public:
    static NotificationManager &instance();
    ~NotificationManager();

    bool initialize();
    bool initialized();
    void shutdown();

    void writeToLogFile(const int &level, QString message);

    NotificationWidget *notify(const QString &text,
                               NotificationWidget::Icon icon = NotificationWidget::NoIcon,
                               NotificationWidget::StandardButtons buttons = NotificationWidget::NoButton,
                               const QObject *reciever = 0, const char *member = 0);


public slots:


protected:
    NotificationManager();

    static void qMessageHandler(QtMsgType type, const char *message);

private:
    bool m_Initialized;
    QFile m_LogFile;
    QTextStream m_LogFileStream;

    QTextStream m_StdOut;
    QTextStream m_StdError;

    ConsoleWidget *m_ConsoleWidget;
};

} // namespace NotificationManager
} // namespace Core

#endif // CORE_NOTIFICATIONMANAGER_NOTIFICATIONMANAGER_H
