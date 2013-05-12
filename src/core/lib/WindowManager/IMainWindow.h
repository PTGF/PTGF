/*!
   \file IMainWindow.h
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

#ifndef CORE_WINDOWMANAGER_IMAINWINDOW_H
#define CORE_WINDOWMANAGER_IMAINWINDOW_H

#include <QObject>
#include <QString>
#include <QIcon>
#include <QtPlugin>

#include "WindowManagerLibrary.h"

class QWidget;

namespace Core {
namespace WindowManager {

class WINDOWMANAGER_EXPORT IMainWindow : public QObject
{
    Q_OBJECT
public:
    IMainWindow(QObject *parent = 0) : QObject(parent) {}
    virtual ~IMainWindow() {}

    virtual QWidget *mainWindowWidget() = 0;
    virtual QString mainWindowName() = 0;
    virtual int mainWindowPriority() = 0;
    virtual QIcon mainWindowIcon() = 0;

    virtual QWidget *createAboutWidget() = 0;

signals:
    void active();
    void notify(const int &level, const QString &message);

};

} // namespace WindowManager
} // namespace Core


Q_DECLARE_INTERFACE(Core::WindowManager::IMainWindow, "org.krellinst.ptgf.IMainWindow")

#endif // CORE_WINDOWMANAGER_IMAINWINDOW_H
