/*!
   \file ISettingPage.cpp
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

#include "ISettingPage.h"

namespace Core {
namespace SettingManager {

/*! \interface Core::SettingManager::ISettingPage
    \brief Interface for setting pages.
    This allows a setting page to be registered with the SettingManager and displayed to the user in the settings dialog.
    Pure virtual class.
 */

/*! \fn ISettingPage::apply()
    \brief Apply the changed settings.
 */

/*! \fn ISettingPage::reset()
    \brief Reset changed settings to their original state.
 */


} // namespace SettingManager
} // namespace Core
