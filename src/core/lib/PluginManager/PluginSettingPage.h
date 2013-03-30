/*!
   \file SettingPage.h
   \author Dane Gardner <dane.gardner@gmail.com>

   \section LICENSE
   This file is part of the Parallel Tools GUI Framework (PTGF)
   Copyright (C) 2010-2013 Argo Navis Technologies, LLC

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

#ifndef CORE_PLUGINMANAGER_PLUGINSETTINGPAGE_H
#define CORE_PLUGINMANAGER_PLUGINSETTINGPAGE_H


//
//  W A R N I N G
//  -------------
//
// This file is not part of the public PTGF API.  This header file may change
// from version to version without notice, or even be removed.
//


#include <SettingManager/ISettingPage.h>

class QTreeWidgetItem;

namespace Core {
namespace PluginManager {

class PluginWrapper;

namespace Ui {
    class PluginSettingPage;
}

class PluginSettingPage : public SettingManager::ISettingPage
{
    Q_OBJECT
    Q_INTERFACES(Core::SettingManager::ISettingPage)

public:
    explicit PluginSettingPage(QList<PluginWrapper *> plugins, QWidget *parent = 0);
    explicit PluginSettingPage(QWidget *parent = 0);
    ~PluginSettingPage();

public slots:
    void apply();
    void reset();

protected:
    void changeEvent(QEvent *e);

    void readSettings();
    void writeSettings();

    void buildTree(QList<PluginWrapper *> plugins);
    QList<QTreeWidgetItem *> m_Plugins;

private:
    Ui::PluginSettingPage *ui;

};

} // namespace PluginManager
} // namespace Core
#endif // CORE_PLUGINMANAGER_PLUGINSETTINGPAGE_H
