/*!
   \file
   \author Dane Gardner <dane.gardner@gmail.com>
   \version

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

   \section DESCRIPTION

 */

#ifndef HELPSETTINGPAGE_H
#define HELPSETTINGPAGE_H

#include <QtCore>
#include <QtGui>
#include <SettingManager/ISettingPage.h>

namespace Plugins {
namespace Help {

namespace Ui {
    class SettingPage;
}

class SettingPage : public Core::SettingManager::ISettingPage
{
    Q_OBJECT
    Q_INTERFACES(Core::SettingManager::ISettingPage)

public:
    explicit SettingPage(QWidget *parent = 0);
    ~SettingPage();

public slots:
    void apply();
    void reset();

private:
    Ui::SettingPage *ui;
};


} // namespace Help
} // namespace Plugins
#endif // HELPSETTINGPAGE_H
