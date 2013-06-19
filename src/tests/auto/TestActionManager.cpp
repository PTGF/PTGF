/*!
   \file TestActionManager.cpp
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

#include "TestActionManager.h"

#include <QTest>

#include <ActionManager/ActionManager.h>
using namespace Core::ActionManager;
#define INSTANCE ActionManager &instance = ActionManager::instance()

TestActionManager::TestActionManager(QObject *parent) :
    QObject(parent)
{
}


void TestActionManager::initTestCase()
{
    INSTANCE;
    QCOMPARE(instance.initialize(), true);
}

void TestActionManager::cleanupTestCase()
{
    INSTANCE;
    instance.shutdown();
}


void TestActionManager::testContext()
{
    INSTANCE;

    Context *context0 = instance.createContext();
    QVERIFY(context0 != NULL);

    Context *context1 = instance.createContext(context0);
    QVERIFY(context1 != NULL);
    QVERIFY(context1->parentContext() == context0);
}


void TestActionManager::testMenuPath()
{
    INSTANCE;

    MenuWidget *widget = instance.createMenuPath(MenuPath("File"));
    QVERIFY(widget != NULL);
}

void TestActionManager::testAction()
{
    INSTANCE;

    //todo; verify menuitem priorities

}

void TestActionManager::testShortcut()
{
    INSTANCE;

}



