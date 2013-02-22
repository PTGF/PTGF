#ifndef IMAINWINDOW_H
#define IMAINWINDOW_H

#include <QtCore>
#include <QtGui>
#include <QtPlugin>

#include <WindowManager/WindowManagerLibrary.h>

namespace Core {
namespace WindowManager {

class WINDOWMANAGER_EXPORT IMainWindow : public QObject
{
    Q_OBJECT
public:
    IMainWindow(QObject *parent = 0) : QObject(parent) {}
    virtual ~IMainWindow() {}

    virtual QWidget *mainWindowWidget() = 0;
    virtual QString mainWindowName() = 0;
    virtual int mainWindowPriority() = 0;
    virtual QIcon mainWindowIcon() = 0;

    virtual QWidget *createAboutWidget() = 0;

signals:
    void active();
    void notify(const int &level, const QString &message);

};

} // namespace WindowManager
} // namespace Core

#ifndef STRINGIFY
#define STRINGIFY(X) # X
#endif

#define IMAINWINDOW_VERSION "org.krellinst.ptgf.IMainWindow/" STRINGIFY(VER_MAJ) "." STRINGIFY(VER_MIN)
Q_DECLARE_INTERFACE(Core::WindowManager::IMainWindow, IMAINWINDOW_VERSION)

#endif // IMAINWINDOW_H
