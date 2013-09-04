#include "HelpPlugin.h"

#include <QHelpEngine>
#include <QApplication>
#include <QDebug>

#include <PluginManager/PluginManager.h>
#include <CoreWindow/CoreWindow.h>

#include "Settings/SettingPage.h"
#include "HelpWidget.h"

namespace Plugins {
namespace Help {

HelpPlugin::HelpPlugin(QObject *parent) :
    QObject(parent)
{
    m_Name = "Help";
    m_Version = QString("%1.%2.%3").arg(VER_MAJ).arg(VER_MIN).arg(VER_PAT);

# ifdef Q_OS_WIN
    m_HelpEngine = new QHelpEngine(QString("%1/ptgf/PTGF.qhc").arg(QApplication::instance()->applicationDirPath()));
# else
    m_HelpEngine = new QHelpEngine(QString("%1/../share/ptgf/PTGF.qhc").arg(QApplication::instance()->applicationDirPath()));
# endif
    m_HelpEngine->setupData();
    m_HelpEngine->setAutoSaveFilter(false);

#ifdef QT_DEBUG
# ifdef Q_OS_WIN
    QString helpFile = QString("%1/ptgf/PTGF.qch").arg(QApplication::instance()->applicationDirPath());
# else
    QString helpFile = QString("%1/../share/ptgf/PTGF.qch").arg(QApplication::instance()->applicationDirPath());
# endif
    QString helpNamespace = "org.krellinst.ptgf";
    if(!m_HelpEngine->registeredDocumentations().contains(helpNamespace) &&
            !m_HelpEngine->registerDocumentation(helpFile)) {
//        qWarning() << Q_FUNC_INFO << tr("Registration of help file, \"%1\", failed: %2").arg(helpFile).arg(m_HelpEngine->error());
    }
#endif
}

HelpPlugin::~HelpPlugin()
{
    if(m_HelpEngine) {
        /*! \bug There is a segfault bug(?) in Qt 4.7.2 that crashes at ~QHelpEngine */
//        delete m_HelpEngine;
        m_HelpEngine = 0;
    }
}


/* BEGIN IPlugin */
bool HelpPlugin::initialize(QStringList &args, QString *err)
{
    Q_UNUSED(args)
    Q_UNUSED(err)

    try {
        using namespace Core;

        HelpWidget *helpWidget = new HelpWidget(m_HelpEngine);

        CoreWindow::CoreWindow &coreWindow = CoreWindow::CoreWindow::instance();
        coreWindow.addCentralWidget(helpWidget, 256);

        PluginManager::PluginManager &pluginManager = PluginManager::PluginManager::instance();
        pluginManager.addObject(this);
        pluginManager.addObject(helpWidget);

    } catch(...) {
        return false;
    }

    return true;
}

void HelpPlugin::shutdown()
{
}

QString HelpPlugin::name()
{
    return m_Name;
}

QString HelpPlugin::version()
{
    return m_Version;
}

QList<Core::PluginManager::Dependency> HelpPlugin::dependencies()
{
    return m_Dependencies;
}
/* END IPlugin */


/* BEGIN ISettingPageFactory */
QIcon HelpPlugin::settingPageIcon()
{
    return QIcon(":/Help/windowIcon.svg");
}

QString HelpPlugin::settingPageName()
{
    return tr("Help");
}

int HelpPlugin::settingPagePriority()
{
    return 256;
}

Core::SettingManager::ISettingPage *HelpPlugin::createSettingPage()
{
    return new SettingPage();
}
/* END ISettingPageFactory */


} // namespace Help
} // namespace Plugins

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN(Plugins::Help::HelpPlugin)
#endif
