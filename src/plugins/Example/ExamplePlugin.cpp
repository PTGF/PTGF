/*!
   \file ExamplePlugin.cpp
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


#include "ExamplePlugin.h"

#ifdef QT_DEBUG
#include <QTime>
#include <QDialog>
#include <QVBoxLayout>
#include <QStandardItemModel>
#include <QAbstractItemView>
#endif

#include <QDebug>

#include <ActionManager/ActionManager.h>
#include <ViewManager/ViewManager.h>

namespace Plugins {
namespace Example {

/*! \namespace Plugins::Example
    \brief Contains the ExamplePlugin, which helps demonstrate the
           methodologies for creating a GUI plugin.
 */

/*! \class ExamplePlugin
    \version 0.3.dev
    \brief This is an example of the methods for producing a GUI plugin.

    \todo Document this more explicitly as a developer example.
 */

ExamplePlugin::ExamplePlugin()
{
    m_Name = "Example";
    m_Version = QString("%1.%2.%3").arg(VER_MAJ).arg(VER_MIN).arg(VER_PAT);
}

ExamplePlugin::~ExamplePlugin()
{
}

bool ExamplePlugin::initialize(QStringList &args, QString *err)
{
    Q_UNUSED(args)
    Q_UNUSED(err)

#ifdef QT_DEBUG
    Core::ActionManager::ActionManager &actionManager = Core::ActionManager::ActionManager::instance();
    Core::ActionManager::MenuPath menuPath("Example", 16);

    QAction *action = actionManager.createAction(menuPath);
    action->setText("Display List of Views");
    connect(action, SIGNAL(triggered()), this, SLOT(exampleMenuItem_Triggered()));
#endif

    return true;
}

void ExamplePlugin::shutdown()
{
}

QString ExamplePlugin::name()
{
    return m_Name;
}

QString ExamplePlugin::version()
{
    return m_Version;
}

QList<Core::PluginManager::Dependency> ExamplePlugin::dependencies()
{
    return m_Dependencies;
}

#ifdef QT_DEBUG
void ExamplePlugin::exampleMenuItem_Triggered()
{
    Core::ViewManager::ViewManager &viewManager = Core::ViewManager::ViewManager::instance();

    qDebug() << Q_FUNC_INFO << viewManager.viewNames();

    const int maxColumns = 5;
    const int maxRows = 100;

    // Create a model
    QStandardItemModel *model = new QStandardItemModel();
    for(int row = 0; row < maxRows; ++row) {
        model->setHeaderData(row, Qt::Vertical, row * 10);
    }

    for(int column = 0; column < maxColumns; ++column) {
        QStandardItem *headerItem = new QStandardItem(tr("Column %1").arg(column));
        model->setHorizontalHeaderItem(column, headerItem);

        for(int row = 0; row < maxRows; ++row) {
            QStandardItem *item = new QStandardItem();
            item->setData(qrand(), Qt::DisplayRole);
            model->setItem(row, column, item);
        }
    }

    // Check for the PlotView plugin, and ensure it can show our model
    if(!viewManager.viewNames(model).contains("PlotView")) {
        return;
    }


    QAbstractItemView *view = viewManager.viewWidget("PlotView", model);
    model->setParent(view);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->addWidget(view);

    QDialog *dlg = new QDialog();
    dlg->setAttribute(Qt::WA_DeleteOnClose, true);
    dlg->resize(800, 480);
    dlg->setLayout(layout);
    dlg->show();
}
#endif

} // namespace Example
} // namespace Plugins

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN(Plugins::Example::ExamplePlugin)
#endif
