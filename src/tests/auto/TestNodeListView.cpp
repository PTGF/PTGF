/*!
   \file TestNodeListView.cpp
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

#include "TestNodeListView.h"

#include <QTest>
#include <QStringList>
#include <QDebug>

#include <NodeListView/HostRange.h>
using namespace Plugins::NodeListView;

TestNodeListView::TestNodeListView(QObject *parent) :
    QObject(parent)
{
}

void TestNodeListView::initTestCase()
{
}

void TestNodeListView::cleanupTestCase()
{
}

void TestNodeListView::testHostRange()
{
        HostRange hostRange("node010");
        QCOMPARE(hostRange.count(), (quint64)1);
        QCOMPARE(hostRange.toString(), QString("node010"));

        QCOMPARE(hostRange.merge("node012"), true);
        QCOMPARE(hostRange.count(), (quint64)2);
        QCOMPARE(hostRange.toString(), QString("node[010,012]"));

        QCOMPARE(hostRange.merge("node[001-005]"), true);
        QCOMPARE(hostRange.count(), (quint64)7);
        QCOMPARE(hostRange.toString(), QString("node[001-005,010,012]"));

        QCOMPARE(hostRange.merge("node[006-010]"), true);
        QCOMPARE(hostRange.count(), (quint64)11);
        QCOMPARE(hostRange.toString(), QString("node[001-010,012]"));

        QCOMPARE(hostRange.merge("node[012-015]"), true);
        QCOMPARE(hostRange.count(), (quint64)14);
        QCOMPARE(hostRange.toString(), QString("node[001-010,012-015]"));

        QCOMPARE(hostRange.merge("node[003-007]"), true);
        QCOMPARE(hostRange.count(), (quint64)14);
        QCOMPARE(hostRange.toString(), QString("node[001-010,012-015]"));

        QCOMPARE(hostRange.merge("node[100-125]"), true);
        QCOMPARE(hostRange.count(), (quint64)40);
        QCOMPARE(hostRange.toString(), QString("node[001-010,012-015,100-125]"));

        QCOMPARE(hostRange.merge("node[013-098]"), true);
        QCOMPARE(hostRange.count(), (quint64)123);
        QCOMPARE(hostRange.toString(), QString("node[001-010,012-098,100-125]"));
    }

void TestNodeListView::testHostRangeSequentialMerge_data()
{
    QTest::addColumn<int>("count");
    QTest::newRow("10 hosts") << 10;
    QTest::newRow("100 hosts") << 100;
    QTest::newRow("1000 hosts") << 1000;
    QTest::newRow("10000 hosts") << 10000;
    QTest::newRow("100000 hosts") << 100000;
}
void TestNodeListView::testHostRangeSequentialMerge()
{
    HostRange hostRange("node0");
    QFETCH(int, count);

    QBENCHMARK {
        for(int i=1; i < count; ++i) {
            QVERIFY(hostRange.merge(QString("node%1").arg(i)));
        }
    }

    QCOMPARE(hostRange.count(), (quint64)count);

    const QString countString = QString("%1").arg((quint64)count-1);
    QCOMPARE(hostRange.toString(),
             QString("node[%1-%2]").arg((quint64)0,       countString.count(), 10, QChar('0')).
                                    arg((quint64)count-1, countString.count(), 10, QChar('0')));

    QCOMPARE(hostRange.expanded(500).count(), qMin(500, (int)hostRange.count()));
}

void TestNodeListView::testHostRangeRandomMerge_data()
{
    QTest::addColumn<int>("count");
    QTest::newRow("10 hosts") << 10;
    QTest::newRow("100 hosts") << 100;
    QTest::newRow("1000 hosts") << 1000;
    QTest::newRow("10000 hosts") << 10000;
}
void TestNodeListView::testHostRangeRandomMerge()
{
    HostRange hostRange("node0");
    QFETCH(int, count);

    const QString countString = QString("%1").arg(count);
    const int maxRangeSize = qBound(2, count / 100, 100);
    const int magnitude = 1;

    QBENCHMARK {
        for(int i=1; i < count; ++i) {
            quint64 rand = qrand() % (count * 10 * magnitude);
            QString newHost = QString("node[%1-%2]")
                    .arg(rand,                            countString.count() + magnitude, 10, QChar('0'))
                    .arg(rand + (qrand() % maxRangeSize), countString.count() + magnitude, 10, QChar('0'));
            QVERIFY(hostRange.merge(newHost));
        }
    }

    QVERIFY(hostRange.count() <= (quint64)(count * 10 * magnitude + maxRangeSize));
    QVERIFY(hostRange.count() >= (quint64)count);

    QCOMPARE(hostRange.toString().mid(0,4), QString("node"));

    QCOMPARE(hostRange.expanded(500).count(), qMin(500, (int)hostRange.count()));
}
