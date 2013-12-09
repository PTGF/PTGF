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


#ifdef EXAMPLE_BUILD
#include <ActionManager/ActionManager.h>
#include <ViewManager/ViewManager.h>

#include <NodeListView/NodeListView.h>
#include <NodeListView/NodeRange.h>

#include <QTime>
#include <QDialog>
#include <QVBoxLayout>
#include <QStandardItemModel>
#include <QAbstractItemView>

#include <PrettyWidgets/GroupBox.h>
#include <PrettyWidgets/LineEdit.h>

#include <ProcessList/ProcessListModel.h>
#include <ProcessList/ProcessListWidget.h>

#include <QDebug>
#endif


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

#ifdef EXAMPLE_BUILD
    Core::ActionManager::ActionManager &actionManager = Core::ActionManager::ActionManager::instance();
    Core::ActionManager::MenuPath menuPath("Example", 16);

    action = actionManager.createAction(menuPath);
    action->setText("Display sample Node List View");
    connect(action, SIGNAL(triggered()), this, SLOT(exampleNodeListView_Triggered()));

    action = actionManager.createAction(menuPath);
    action->setText("Display sample PrettyWidgets::GroupBox");
    connect(action, SIGNAL(triggered()), this, SLOT(exampleGroupBox_Triggered()));

    action = actionManager.createAction(menuPath);
    action->setText("Display sample Plugins::ProcessList");
    connect(action, SIGNAL(triggered()), this, SLOT(exampleProcessList_Triggered()));

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

#ifdef EXAMPLE_BUILD
void ExamplePlugin::exampleGroupBox_Triggered()
{
    GroupBox *gb = new GroupBox();
    gb->setCheckable(true);
    gb->setCollapsible(true);
    gb->setTitle(tr("Test title"));

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(0);

    LineEdit *le = new LineEdit(gb);
    le->setPlaceholderText(tr("This is a placeholder text"));
    layout->addWidget(le);

    layout->addWidget(new LineEdit(gb));

    le = new LineEdit(gb);
    le->setPlaceholderText(tr("Username"));
    layout->addWidget(le);

    le = new LineEdit(gb);
    le->setPlaceholderText(tr("Password"));
    layout->addWidget(le);

    layout->addWidget(new LineEdit(gb));

    le = new LineEdit(gb);
    le->setPlaceholderText(tr("Path to executable"));
    layout->addWidget(le);

    layout->addWidget(new LineEdit(gb));

    gb->setLayout(layout);

    layout = new QVBoxLayout();
    layout->setMargin(0);

    QDialog *dlg = new QDialog();
    dlg->setAttribute(Qt::WA_DeleteOnClose, true);
    dlg->resize(800, 480);

    layout->setParent(dlg);
    gb->setParent(dlg);
    dlg->setLayout(layout);
    layout->addWidget(gb);
    layout->addStretch();

    dlg->show();
}

void ExamplePlugin::exampleNodeListView_Triggered()
{

    Plugins::NodeListView::NodeListView *view = new Plugins::NodeListView::NodeListView();

    QString slurmNodeList(getenv("SLURM_NODELIST"));
    if(slurmNodeList.isEmpty()) {
        QStringList nodes;
        for(int i = 1; i < 10; ++i) {
            quint64 rand = qrand() % 10000;
            nodes << QString("node[%1-%2]").arg(rand, 4, 10, QChar('0')).arg(rand + (qrand() % 50), 4, 10, QChar('0'));
        }
        view->setNodes(nodes.join(","));
    }

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->addWidget(view);

    QDialog *dlg = new QDialog();
    dlg->setAttribute(Qt::WA_DeleteOnClose, true);
    dlg->resize(800, 480);
    dlg->setLayout(layout);
    dlg->show();
}

void ExamplePlugin::exampleProcessList_Triggered()
{
    using namespace Plugins::ProcessList;

    ProcessListModel *model = new ProcessListModel();
    model->update();

    ProcessListWidget *view = new ProcessListWidget();
    view->setFilter("mpirun|srun|orterun");
    view->setModel(model);


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
