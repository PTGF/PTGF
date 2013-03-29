/*!
   \file AboutDialog.cpp
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

#include "AboutDialog.h"
#include "ui_AboutDialog.h"

#include <QDesktopWidget>
#include <QApplication>
#include <QTimer>

#include <PrettyWidgets/TabWidget.h>
#include <WindowManager/WindowManager.h>
#include "AboutWidget.h"

namespace Core {
namespace WindowManager {

AboutDialog::AboutDialog(bool splash, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog),
    m_TabWidget(new TabWidget(this))
{
    ui->setupUi(this);

    AboutWidget *aboutWidget = new AboutWidget(this);

    if(splash) {
        layout()->addWidget(aboutWidget);
    } else {
        m_TabWidget->setHideBarOnOne(true);
        layout()->addWidget(m_TabWidget);

        m_TabWidget->addTab(aboutWidget, aboutWidget->windowIcon(), aboutWidget->windowTitle());

        WindowManager &windowManager = WindowManager::instance();
        foreach(QWidget *aboutWidget, windowManager.aboutWidgets()) {
            m_TabWidget->addTab(aboutWidget, aboutWidget->windowIcon(), aboutWidget->windowTitle());
        }
    }

    setWindowTitle(aboutWidget->windowTitle());
    setWindowIcon(aboutWidget->windowIcon());
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::splash(int ms)
{
    AboutDialog *aboutDialog = new AboutDialog(true);
    aboutDialog->setAttribute(Qt::WA_DeleteOnClose);    // So we don't have to worry about deleting it
    aboutDialog->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    aboutDialog->setModal(true);
    aboutDialog->layout()->setMargin(0);

    const QRect screen = QApplication::desktop()->screenGeometry();
    aboutDialog->move(screen.center() - aboutDialog->rect().center());

    aboutDialog->open();

    /*! \todo I would like to see this timer start when the entire system is done loading and is
     *        displayed to the user.  Doing it this way, means the user is either not seeing it
     *        long enough, or too long depending on their system speed.
     */
    QTimer::singleShot(ms, aboutDialog, SLOT(close()));
}

} // namespace WindowManager
} // namespace Core
