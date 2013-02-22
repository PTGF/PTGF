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

#ifndef IWELCOMEDATA_H
#define IWELCOMEDATA_H

#include <QtCore>
#include <QtGui>

#include "WelcomeLibrary.h"

namespace Plugins {
namespace Welcome {

struct WELCOME_EXPORT Link {
    enum Type { Type_None, Type_Web, Type_Text, Type_Video };

    Link(QString title, QString description, Type type, int priority, const QObject *receiver, const char *method)
    {
        this->title = title;
        this->description = description;
        this->type = type;
        this->priority = priority;
        this->receiver = receiver;
        this->method = method;
    }

    Link(QString title, QString description, Type type, int priority, QUrl url)
    {
        this->title = title;
        this->description = description;
        this->type = type;
        this->priority = priority;
        this->url = url;
    }

private:
    QString title;
    QString description;
    Type type;
    int priority;
    QUrl url;
    const QObject *receiver;
    const char *method;

    friend class WelcomeWidget;
};

class WELCOME_EXPORT IWelcomeData
{
public:
    virtual bool initialize() = 0;
    virtual QList<Link> actions() = 0;
    virtual QList<Link> recent() = 0;
    virtual QList<Link> tutorials() = 0;
    virtual QList<Link> examples() = 0;
    virtual QList<Link> support() = 0;
    virtual QStringList tipsAndTricks() = 0;
    virtual QList<QUrl> latestNewsRss() = 0;
    virtual QList<QWidget *> additionalTabs() = 0;
};

} // namespace Welcome
} // namespace Plugins

#ifndef STRINGIFY
#define STRINGIFY(X) # X
#endif

#define IWELCOMEDATA_VERSION "org.krellinst.ptgf.IWelcomeData/" STRINGIFY(VER_MAJ) "." STRINGIFY(VER_MIN)
Q_DECLARE_INTERFACE(Plugins::Welcome::IWelcomeData, IWELCOMEDATA_VERSION)

#endif // IWELCOMEDATA_H
