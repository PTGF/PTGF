#ifndef PLUGINS_HELP_HELPBROWSER_H
#define PLUGINS_HELP_HELPBROWSER_H

#include <QTextBrowser>
#include <QUrl>

class QHelpEngine;


namespace Plugins {
namespace Help {

class HelpBrowser : public QTextBrowser
{
    Q_OBJECT
public:
    explicit HelpBrowser(QHelpEngine *helpEngine, QWidget *parent = 0);
    virtual QVariant loadResource(int type, const QUrl &url);

private:
    QHelpEngine *m_HelpEngine;
};

} // namespace Help
} // namespace Plugins

#endif // PLUGINS_HELP_HELPBROWSER_H
