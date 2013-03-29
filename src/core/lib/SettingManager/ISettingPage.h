/*!
   \file ISettingPage.h
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

#ifndef CORE_SETTINGMANAGER_ISETTINGPAGE_H
#define CORE_SETTINGMANAGER_ISETTINGPAGE_H

#include <QWidget>
#include <QtPlugin>

#include "SettingManagerLibrary.h"

namespace Core {
namespace SettingManager {

class SETTINGMANAGER_EXPORT ISettingPage : public QWidget
{
public:
    explicit ISettingPage(QWidget *parent) : QWidget(parent) {}
    virtual ~ISettingPage() {}

public slots:
    virtual void apply() = 0;
    virtual void reset() = 0;
};

} // namespace SettingManager
} // namespace Core

#define ISETTINGPAGE_VERSION "org.krellinst.ptgf.ISettingPage/" STRINGIFY(VER_MAJ) "." STRINGIFY(VER_MIN)
Q_DECLARE_INTERFACE(Core::SettingManager::ISettingPage, ISETTINGPAGE_VERSION)

#endif // CORE_SETTINGMANAGER_ISETTINGPAGE_H
