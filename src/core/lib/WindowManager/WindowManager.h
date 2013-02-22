#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QtCore>
#include <QtGui>

#include "WindowManagerLibrary.h"

namespace Core {
namespace WindowManager {

class IMainWindow;

class WINDOWMANAGER_EXPORT WindowManager : public QObject
{
    Q_OBJECT
public:
    static WindowManager &instance();
    ~WindowManager();

    bool initialize();
    bool initialized();
    void shutdown();

    QList<IMainWindow *> mainWindows();
    QList<QWidget *> aboutWidgets();

protected:
    WindowManager();

    void registerMainWindow(IMainWindow *window);
    void deregisterMainWindow(IMainWindow *window);

    static bool ascending(IMainWindow *left, IMainWindow *right);

protected slots:
    void aboutDialog();
    void pluginObjectRegistered(QObject *object);
    void pluginObjectDeregistered(QObject *object);
    void windowActivated();

private:
    bool m_Initialized;
    QList<IMainWindow *> m_MainWindows;
    QAction *m_AboutPage;

};

} // namespace WindowManager
} // namespace Core
#endif // WINDOWMANAGER_H
