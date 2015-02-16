/*!
   \file SettingManagerPrivate.h
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

#ifndef CORE_SETTINGMANAGER_SETTINGMANAGERPRIVATE_H
#define CORE_SETTINGMANAGER_SETTINGMANAGERPRIVATE_H


//
//  W A R N I N G
//  -------------
//
// This file is not part of the public PTGF API.  This header file may change
// from version to version without notice, or even be removed.
//


#include <QObject>
#include <QSettings>

#include "SettingManager.h"

namespace Core {
namespace SettingManager {

class ISettingPageFactory;

class SettingManagerPrivate : public QObject
{
    Q_OBJECT
    DECLARE_PUBLIC(SettingManager)

public:
    explicit SettingManagerPrivate();

protected:
    void registerPageFactory(ISettingPageFactory *page);
    void deregisterPageFactory(ISettingPageFactory *page);

#ifndef NO_XML_MODULE
    static bool readXmlFile(QIODevice &device, QSettings::SettingsMap &map);
    static bool writeXmlFile(QIODevice &device, const QSettings::SettingsMap &map);
#endif


#ifndef NO_SQL_MODULE
    static bool readSqliteFile(QIODevice &device, QSettings::SettingsMap &map);
    static bool writeSqliteFile(QIODevice &device, const QSettings::SettingsMap &map);
#endif


protected slots:
    void pluginObjectRegistered(QObject *object);
    void pluginObjectDeregistered(QObject *object);

private:
    bool m_Initialized;
    QSettings *m_Settings;
    QList<ISettingPageFactory *> m_Pages;
    SettingManager::FormatTypes m_FormatType;

#ifndef NO_XML_MODULE
    QSettings::Format m_XmlFormat;
#endif

#ifndef NO_SQL_MODULE
    QSettings::Format m_SqliteFormat;
#endif

};

} // namespace SettingManager
} // namespace Core

#endif // CORE_SETTINGMANAGER_SETTINGMANAGERPRIVATE_H
