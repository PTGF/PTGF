/*!
   \file SettingManager.h
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

#ifndef CORE_SETTINGMANAGER_SETTINGMANAGER_H
#define CORE_SETTINGMANAGER_SETTINGMANAGER_H

#include <QObject>
#include <QVariant>
#include <QString>

#include "SettingManagerLibrary.h"

namespace Core {
namespace SettingManager {

class SettingManagerPrivate;

class SETTINGMANAGER_EXPORT SettingManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(SettingManager)
    DECLARE_PRIVATE(SettingManager)

public:
    static SettingManager &instance();
    ~SettingManager();

    bool initialize();
    void shutdown();

    void setValue(const QString &key, const QVariant &value);
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;

    void remove(const QString &key);
    bool contains(const QString &key) const;

    void beginGroup(const QString &prefix);
    void endGroup();
    QString group() const;

public slots:
    void settingDialog();

protected:
    explicit SettingManager(QObject *parent = NULL);

};

} // namespace SettingManager
} // namespace Core
#endif // CORE_SETTINGMANAGER_SETTINGMANAGER_H
