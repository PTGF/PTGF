/*!
   \file AboutDialog.h
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

#ifndef CORE_WINDOWMANAGER_ABOUTDIALOG_H
#define CORE_WINDOWMANAGER_ABOUTDIALOG_H


//
//  W A R N I N G
//  -------------
//
// This file is not part of the public PTGF API.  This header file may change
// from version to version without notice, or even be removed.
//


#include <QDialog>

class TabWidget;

namespace Core {
namespace WindowManager {

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(bool splash = false, QWidget *parent = 0);
    ~AboutDialog();

    static void splash(int ms = 2000);

private:
    Ui::AboutDialog *ui;
    TabWidget *m_TabWidget;
};

} // namespace WindowManager
} // namespace Core
#endif // CORE_WINDOWMANAGER_ABOUTDIALOG_H
