#include "AboutDialog.h"
#include "ui_AboutDialog.h"

#include <PrettyWidgets/TabWidget.h>
#include <WindowManager/WindowManager.h>
#include "AboutWidget.h"

namespace Core {
namespace WindowManager {

AboutDialog::AboutDialog(bool splash, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog),
    m_TabWidget(new TabWidget(this))
{
    ui->setupUi(this);

    AboutWidget *aboutWidget = new AboutWidget(this);

    if(splash) {
        layout()->addWidget(aboutWidget);
    } else {
        m_TabWidget->setHideBarOnOne(true);
        layout()->addWidget(m_TabWidget);

        m_TabWidget->addTab(aboutWidget, aboutWidget->windowIcon(), aboutWidget->windowTitle());

        WindowManager &windowManager = WindowManager::instance();
        foreach(QWidget *aboutWidget, windowManager.aboutWidgets()) {
            m_TabWidget->addTab(aboutWidget, aboutWidget->windowIcon(), aboutWidget->windowTitle());
        }
    }

    setWindowTitle(aboutWidget->windowTitle());
    setWindowIcon(aboutWidget->windowIcon());
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::splash(int ms)
{
    AboutDialog *aboutDialog = new AboutDialog(true);
    aboutDialog->setAttribute(Qt::WA_DeleteOnClose);    // So we don't have to worry about deleting it
    aboutDialog->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    aboutDialog->setModal(true);
    aboutDialog->layout()->setMargin(0);

    const QRect screen = QApplication::desktop()->screenGeometry();
    aboutDialog->move(screen.center() - aboutDialog->rect().center());

    aboutDialog->open();

    /*! \todo I would like to see this timer start when the entire system is done loading and is
     *        displayed to the user.  Doing it this way, means the user is either not seeing it
     *        long enough, or too long depending on their system speed.
     */
    QTimer::singleShot(ms, aboutDialog, SLOT(close()));
}

} // namespace WindowManager
} // namespace Core
