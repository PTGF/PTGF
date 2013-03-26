#ifndef CORE_NOTIFICATIONMANAGER_NOTIFICATIONMANAGER_H
#define CORE_NOTIFICATIONMANAGER_NOTIFICATIONMANAGER_H

#include <NotificationManager/NotificationManagerLibrary.h>
#include <NotificationManager/NotificationWidget.h>

namespace Core {
namespace NotificationManager {

class NotificationManagerPrivate;

class NOTIFICATIONMANAGER_EXPORT NotificationManager
{
public:
    explicit NotificationManager();

    bool initialize();
    void shutdown();

    void writeToLogFile(const int &level, QString message);

    NotificationWidget *notify(const QString &text,
                                      NotificationWidget::Icon icon = NotificationWidget::NoIcon,
                                      NotificationWidget::StandardButtons buttons = NotificationWidget::NoButton,
                                      const QObject *reciever = 0, const char *member = 0);

private:
    Q_DISABLE_COPY(NotificationManager)

    static NotificationManagerPrivate *d;

};

} // namespace NotificationManager
} // namespace Core

#endif // CORE_NOTIFICATIONMANAGER_NOTIFICATIONMANAGER_H
