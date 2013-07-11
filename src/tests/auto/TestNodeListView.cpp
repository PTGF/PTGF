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

#include <NodeListView/Range.h>
#include <NodeListView/NodeRange.h>
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

void TestNodeListView::testRange()
{
    quint64 lower = 0, upper = 0;
    Range range;

    // Check helper function that assigns both range value simultaneously
    lower = 10;
    upper = 9999999;
    range.setValue(lower, upper);
    QCOMPARE(range.lower(), lower);
    QCOMPARE(range.upper(), upper);

    // Check individual range value assignments
    lower = 5;
    upper = 9999;
    range.setLower(lower);
    QCOMPARE(range.lower(), lower);
    range.setUpper(upper);
    QCOMPARE(range.upper(), upper);

    // Check that setting the lower or upper in an odd way automatically assigns the proper value
    range.setLower(upper);
    QCOMPARE(range.lower(), upper);
    QCOMPARE(range.upper(), upper);
    range.setUpper(lower);
    QCOMPARE(range.lower(), lower);
    QCOMPARE(range.upper(), upper);

    // Check manually assigned widths
    int width = 7;
    QCOMPARE(range.toString(width), QString("%1-%2").arg(lower, width, 10, QLatin1Char('0')).arg(upper, width, 10, QLatin1Char('0')));

    // Check that auto width discovery matches upper range number's width
    width = 4;
    QCOMPARE(range.toString(-1), QString("%1-%2").arg(lower, width, 10, QLatin1Char('0')).arg(upper, width, 10, QLatin1Char('0')));
}


void TestNodeListView::testNodeRange()
{
        NodeRange nodeRange("node010");
        QCOMPARE(nodeRange.count(), (quint64)1);
        QCOMPARE(nodeRange.toString(), QString("node010"));

        QCOMPARE(nodeRange.merge("node012"), true);
        QCOMPARE(nodeRange.count(), (quint64)2);
        QCOMPARE(nodeRange.toString(), QString("node[010,012]"));

        QCOMPARE(nodeRange.merge("node[001-005]"), true);
        QCOMPARE(nodeRange.count(), (quint64)7);
        QCOMPARE(nodeRange.toString(), QString("node[001-005,010,012]"));

        QCOMPARE(nodeRange.merge("node[006-010]"), true);
        QCOMPARE(nodeRange.count(), (quint64)11);
        QCOMPARE(nodeRange.toString(), QString("node[001-010,012]"));

        QCOMPARE(nodeRange.merge("node[012-015]"), true);
        QCOMPARE(nodeRange.count(), (quint64)14);
        QCOMPARE(nodeRange.toString(), QString("node[001-010,012-015]"));

        QCOMPARE(nodeRange.merge("node[003-007]"), true);
        QCOMPARE(nodeRange.count(), (quint64)14);
        QCOMPARE(nodeRange.toString(), QString("node[001-010,012-015]"));

        QCOMPARE(nodeRange.merge("node[100-125]"), true);
        QCOMPARE(nodeRange.count(), (quint64)40);
        QCOMPARE(nodeRange.toString(), QString("node[001-010,012-015,100-125]"));

        QCOMPARE(nodeRange.merge("node[013-098]"), true);
        QCOMPARE(nodeRange.count(), (quint64)123);
        QCOMPARE(nodeRange.toString(), QString("node[001-010,012-098,100-125]"));
    }

void TestNodeListView::testNodeRangeSequentialMerge_data()
{
    QTest::addColumn<int>("count");
    QTest::newRow("10 nodes") << 10;
    QTest::newRow("100 nodes") << 100;
    QTest::newRow("1000 nodes") << 1000;
    QTest::newRow("10000 nodes") << 10000;
    QTest::newRow("100000 nodes") << 100000;
}
void TestNodeListView::testNodeRangeSequentialMerge()
{
    NodeRange nodeRange("node0");
    QFETCH(int, count);

    QBENCHMARK {
        for(int i=1; i < count; ++i) {
            QVERIFY(nodeRange.merge(QString("node%1").arg(i)));
        }
    }

    QCOMPARE(nodeRange.count(), (quint64)count);

    const QString countString = QString("%1").arg((quint64)count-1);
    QCOMPARE(nodeRange.toString(),
             QString("node[%1-%2]").arg((quint64)0,       countString.count(), 10, QChar('0')).
                                    arg((quint64)count-1, countString.count(), 10, QChar('0')));

    QCOMPARE(nodeRange.expanded(500).count(), qMin(500, (int)nodeRange.count()));
}

void TestNodeListView::testNodeRangeRandomMerge_data()
{
    QTest::addColumn<int>("count");
    QTest::newRow("10 nodes") << 10;
    QTest::newRow("100 nodes") << 100;
    QTest::newRow("1000 nodes") << 1000;
    QTest::newRow("10000 nodes") << 10000;
}
void TestNodeListView::testNodeRangeRandomMerge()
{
    NodeRange nodeRange("node0");
    QFETCH(int, count);

    const QString countString = QString("%1").arg(count);
    const int maxRangeSize = qBound(2, count / 100, 100);
    const int magnitude = 1;

    QBENCHMARK {
        for(int i=1; i < count; ++i) {
            quint64 rand = qrand() % (count * 10 * magnitude);
            QString newNode = QString("node[%1-%2]")
                    .arg(rand,                            countString.count() + magnitude, 10, QChar('0'))
                    .arg(rand + (qrand() % maxRangeSize), countString.count() + magnitude, 10, QChar('0'));
            QVERIFY(nodeRange.merge(newNode));
        }
    }

    QVERIFY(nodeRange.count() <= (quint64)(count * 10 * magnitude + maxRangeSize));
    QVERIFY(nodeRange.count() >= (quint64)count);

    QCOMPARE(nodeRange.toString().mid(0,4), QString("node"));

    QCOMPARE(nodeRange.expanded(500).count(), qMin(500, (int)nodeRange.count()));
}
