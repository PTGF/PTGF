/*!
   \file WelcomeWidget.h
   \author Dane Gardner <dane.gardner@gmail.com>

   \section LICENSE
   This file is part of the Parallel Tools GUI Framework (PTGF)
   Copyright (C) 2010-2015 Argo Navis Technologies, LLC

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU Lesser General Public License as published by the
   Free Software Foundation; either version 2.1 of the License, or (at your
   option) any later version.

   This library is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
   for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this library; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef PLUGINS_WELCOME_WELCOMEWIDGET_H
#define PLUGINS_WELCOME_WELCOMEWIDGET_H

#include <QWidget>
#include <QSignalMapper>
#include <QUrl>

namespace Plugins {
namespace Welcome {

struct Link;
class IWelcomeData;
class RssReaderWidget;

namespace Ui {
    class WelcomeWidget;
}

class WelcomeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomeWidget(QWidget *parent = 0);
    ~WelcomeWidget();


protected:
    QStringList rssNewsFeeds();
    void setNewsTabVisible(bool visible);

    void registerWelcomeData(IWelcomeData *welcomeData);
    void deregisterWelcomeData(IWelcomeData *welcomeData);
    void addCommandButton(const Link &link, QWidget *parent);
    void setCurrentTip(int index);
    void refreshRss();

protected slots:
    void pluginObjectRegistered(QObject *object);
    void pluginObjectDeregistered(QObject *object);

    void urlClicked(int index);

    void randomTip();
    void on_btnTipNext_clicked();
    void on_btnTipPrevious_clicked();


private:
    Ui::WelcomeWidget *ui;
    QList<IWelcomeData *> m_WelcomeData;
    int m_CurrentTip;
    QStringList m_TipsAndTricks;
    QList<QUrl> m_Urls;

    QSignalMapper m_UrlMapper;
    RssReaderWidget *m_RssReaderWidget;

    friend class SettingPage;
};


} // namespace Welcome
} // namespace Plugins
#endif // PLUGINS_WELCOME_WELCOMEWIDGET_H
