/*!
   \file ISettingPageFactory.h
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

#ifndef CORE_SETTINGMANAGER_ISETTINGPAGEFACTORY_H
#define CORE_SETTINGMANAGER_ISETTINGPAGEFACTORY_H

#include <QIcon>
#include <QString>
#include <QtPlugin>

#include "SettingManagerLibrary.h"

namespace Core {
namespace SettingManager {

class ISettingPage;

class SETTINGMANAGER_EXPORT ISettingPageFactory
{
public:
    virtual QIcon settingPageIcon() = 0;
    virtual QString settingPageName() = 0;
    virtual int settingPagePriority() = 0;
    virtual ISettingPage *createSettingPage() = 0;
};

} // namespace SettingManager
} // namespace Core


#if QT_VERSION >= 0x050000
Q_DECLARE_INTERFACE(Core::SettingManager::ISettingPageFactory, "org.krellinst.ptgf.ISettingPageFactory")
#else
#define ISETTINGPAGEFACTORY_VERSION "org.krellinst.ptgf.ISettingPageFactory/" STRINGIFY(VER_MAJ) "." STRINGIFY(VER_MIN)
Q_DECLARE_INTERFACE(Core::SettingManager::ISettingPageFactory, ISETTINGPAGEFACTORY_VERSION)
#endif

#endif // CORE_SETTINGMANAGER_ISETTINGPAGEFACTORY_H
