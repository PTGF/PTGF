#ifndef HELPPLUGIN_H
#define HELPPLUGIN_H

#include <QObject>
class QHelpEngine;

#include <PluginManager/IPlugin.h>
#include <SettingManager/ISettingPageFactory.h>

namespace Plugins {
namespace Help {

class HelpPlugin :
        public QObject,
        public Core::PluginManager::IPlugin,
        public Core::SettingManager::ISettingPageFactory
{
    Q_OBJECT

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA("org.krellinst.ptgf.HelpPlugin")
#endif

    Q_INTERFACES(Core::PluginManager::IPlugin)
    Q_INTERFACES(Core::SettingManager::ISettingPageFactory)

public:
    explicit HelpPlugin(QObject *parent = 0);
    ~HelpPlugin();

    /* BEGIN IPlugin */
    bool initialize(QStringList &args, QString *err);
    void shutdown();
    QString name();
    QString version();
    QList<Core::PluginManager::Dependency> dependencies();
    /* END IPlugin */

    /* BEGIN ISettingPageFactory */
    QIcon settingPageIcon();
    QString settingPageName();
    int settingPagePriority();
    Core::SettingManager::ISettingPage *createSettingPage();
    /* END ISettingPageFactory */

private:
    QString m_Name;
    QString m_Version;
    QList<Core::PluginManager::Dependency> m_Dependencies;

    QHelpEngine *m_HelpEngine;
};

} // namespace Help
} // namespace Plugins

#endif // HELPPLUGIN_H
