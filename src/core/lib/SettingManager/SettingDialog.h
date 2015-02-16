/*!
   \file SettingDialog.h
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

#ifndef CORE_SETTINGMANAGER_SETTINGDIALOG_H
#define CORE_SETTINGMANAGER_SETTINGDIALOG_H


//
//  W A R N I N G
//  -------------
//
// This file is not part of the public PTGF API.  This header file may change
// from version to version without notice, or even be removed.
//


#include <QDialog>

#include "ISettingPageFactory.h"

namespace Core {
namespace SettingManager {

namespace Ui {
    class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QList<ISettingPageFactory *> pages, QWidget *parent = 0);
    ~SettingDialog();

    void readSettings();
    void writeSettings();

protected:
    static bool ascending(ISettingPageFactory *left, ISettingPageFactory *right);
    QList<ISettingPageFactory *> m_Pages;

private:
    Ui::SettingDialog *ui;

};

} // namespace SettingManager
} // namespace Core
#endif // CORE_SETTINGMANAGER_SETTINGDIALOG_H
