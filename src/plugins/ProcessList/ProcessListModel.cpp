/*!
   \file ProcessList.cpp
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

#include "ProcessListModelPrivate.h"

#include <QProcess>


static const QString defaultRemoteHostName("localhost");
static const QString defaultRemoteShell("ssh");


namespace Plugins {
namespace ProcessList {

ProcessListModel::ProcessListModel(QObject *parent) :
    QStandardItemModel(parent),
    d(new ProcessListModelPrivate)
{
    d->q = this;

    d->changeBackgroundColorOnUpdate = true;

    d->remoteHostName = defaultRemoteHostName;
    d->remoteShell = defaultRemoteShell;

    setHorizontalHeaderItem(0, new QStandardItem(tr("PID")));
    setHorizontalHeaderItem(1, new QStandardItem(tr("Command")));
}

ProcessListModel::~ProcessListModel()
{

}

QString ProcessListModel::remoteHost() const
{
    return d->remoteHostName;
}

void ProcessListModel::setRemoteHost(const QString &hostName)
{
    QString temp = hostName;
    if(temp.isEmpty()) {
        temp = defaultRemoteHostName;
    }

    if(d->remoteHostName == temp) {
        return;
    }

    d->remoteHostName = temp;
    update();
}

QString ProcessListModel::remoteShell() const
{
    return d->remoteShell;
}

void ProcessListModel::setRemoteShell(const QString &shell)
{
    QString temp = shell;

    if(temp.isEmpty()) {
        temp = defaultRemoteShell;
    }

    if(d->remoteShell == temp) {
        return;
    }

    d->remoteShell = temp;
    update();
}

void ProcessListModel::update()
{
    QByteArray output = d->getProcessList();
    if(output.isEmpty()) {
        return;
    }

    // BEGIN Parse the process list
    QStringList lines = QString(output).split('\n');
    QMap<quint64, QString> processes;

    static const QRegExp rxPid("\\s*PID");
    static const QRegExp rxCommand("COMMAND\\s*$");

    int commandIndex = 0;
    foreach(QString line, lines) {
        if(line.contains(rxPid)) {
            commandIndex = line.indexOf(rxCommand);
            continue;
        }

        bool okay = false;
        QString trimmed = line.trimmed();
        quint64 pid = trimmed.left(trimmed.indexOf(' ')).toULongLong(&okay);

        if(okay) {
            processes.insert(pid, line.right(line.count() - commandIndex));
        }
    }
    // END Parse process list


    // BEGIN Remove or merge anything in the existing model
    bool updating = d->processes.count();
    foreach(quint64 pid, d->processes.keys()) {
        QStandardItem *item = d->processes.value(pid);
        if(!processes.contains(pid)) {
            removeRow(item->row());
            d->processes.remove(pid);
        } else {
            QModelIndex index = this->index(item->row(), 1);
            if(index.data().toString() != processes.value(pid)) {
                setData(index, processes.value(pid), Qt::DisplayRole);
                if(d->changeBackgroundColorOnUpdate) {
                    setData(index, QBrush(QColor(Qt::yellow).lighter()), Qt::BackgroundRole);
                    item->setData(QBrush(QColor(Qt::yellow).lighter()), Qt::BackgroundRole);
                }
            } else {
                if(d->changeBackgroundColorOnUpdate) {
                    setData(index, QVariant(), Qt::BackgroundRole);
                    item->setData(QVariant(), Qt::BackgroundRole);
                }
            }
            processes.remove(pid);
        }
    }
    // END Remove or merge anything in the existing model

    // BEGIN Add everything else to the model
    foreach(quint64 pid, processes.keys()) {
        QStandardItem *item = new QStandardItem();
        item->setEditable(false);
        item->setData(QVariant(pid), Qt::DisplayRole);
        d->processes.insert(pid, item);
        appendRow(item);

        QStandardItem *commandItem = new QStandardItem();
        commandItem->setEditable(false);
        commandItem->setText(processes.value(pid));
        setItem(item->row(), 1, commandItem);

        if(d->changeBackgroundColorOnUpdate && updating) {
            item->setData(QBrush(QColor(Qt::green).lighter()), Qt::BackgroundRole);
            commandItem->setData(QBrush(QColor(Qt::green).lighter()), Qt::BackgroundRole);
        }
    }
    // END Add everything else to the model

}



bool ProcessListModel::changeBackgroundColorOnUpdate() const
{
    return d->changeBackgroundColorOnUpdate;
}
void ProcessListModel::setChangeBackgroundColorOnUpdate(const bool &change)
{
    if(d->changeBackgroundColorOnUpdate == change) {
        return;
    }

    d->changeBackgroundColorOnUpdate = change;

    // Clear all existing colors if we've set this to false
    if(!d->changeBackgroundColorOnUpdate) {
        for(int row = 0; row < rowCount(); ++row) {
            for(int column = 0; column < columnCount(); ++column) {
                item(row, column)->setData(QVariant(), Qt::BackgroundRole);
            }
        }
    }
}





ProcessListModelPrivate::ProcessListModelPrivate()
{

}

ProcessListModelPrivate::~ProcessListModelPrivate()
{

}

QByteArray ProcessListModelPrivate::getProcessList()
{
    QString program = "/bin/ps";
    QStringList arguments;
    arguments << "w" << "w" << "x" << "o" << "pid,command";

    // If it's not the local system that we're gathering from, use the remote shell to get the list
    if(remoteHostName != "localhost") {
        QStringList newArguments;
        newArguments << remoteHostName << "c" << program << arguments;
        arguments = newArguments;
        program = remoteShell;
    }

    QProcess process;
    process.start(program, arguments);

    if(!process.waitForStarted()) {
        return QByteArray();
    }

    if(!process.waitForFinished()) {
        return QByteArray();
    }

    return process.readAll();

}



} // namespace ProcessList
} // namespace Plugins
