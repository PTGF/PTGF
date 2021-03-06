/*!
   \file TestNodeListView.cpp
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

#include "TestNodeListView.h"

#include <stdlib.h>

#include <QTest>
#include <QStringList>
#include <QDebug>

#include <NodeListView/Range.h>
#include <NodeListView/NodeRange.h>
#include <NodeListView/Slurm.h>
#include <NodeListView/NodeListView.h>
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

void TestNodeListView::testNodeListView_data()
{
    QTest::addColumn<QString>("nodeList");
    QTest::addColumn<QString>("nodeSearch");
    QTest::addColumn<QString>("nodeSelection");
    QTest::addColumn<bool>("nodeSearchIsValid");


    QTest::newRow("Single node") << "nodes[000-999]" << "nodes005" << "nodes005" << true;
    QTest::newRow("Single node low") << "nodes[000-999]" << "nodes000" << "nodes000" << true;
    QTest::newRow("Single node high") << "nodes[000-999]" << "nodes999" << "nodes999" << true;
    QTest::newRow("Single node outside") << "nodes[000-999]" << "nodes1000" << "" << false;

    QTest::newRow("Single range") << "nodes[000-999]" << "nodes[005-010]" << "nodes[005-010]" << true;
    QTest::newRow("Single range low") << "nodes[000-999]" << "nodes[000-100]" << "nodes[000-100]" << true;
    QTest::newRow("Single range high") << "nodes[000-999]" << "nodes[900-999]" << "nodes[900-999]" << true;
    QTest::newRow("Single range outside") << "nodes[000-899]" << "nodes[850-999]" << "nodes[850-899]" << false;

    QTest::newRow("Multi range") << "nodes[000-999]" << "nodes[005-010,123,256-310]" << "nodes[005-010,123,256-310]" << true;
    QTest::newRow("Multi range low/high") << "nodes[000-999]" << "nodes[000-100,123,256-310,900-999]" << "nodes[000-100,123,256-310,900-999]" << true;
    QTest::newRow("Multi range outside") << "nodes[000-899]" << "nodes[005-010,123,256-310,850-999]" << "nodes[005-010,123,256-310,850-899]" << false;


    QTest::newRow("Suffix multi range 0") << "nodes[000-999]ib" << "nodes[005-010,123,256-310]ib" << "nodes[005-010,123,256-310]ib" << true;
    QTest::newRow("Suffix multi range 1") << "nodes[000-999],nodes[000-999]ib" << "nodes[005-010,123,256-310]ib" << "nodes[005-010,123,256-310]ib" << true;
    QTest::newRow("Suffix multi range 2") << "nodes[000-999],nodes[000-999]ib" << "nodes[005-010],nodes[123,256-310]ib" << "nodes[005-010],nodes[123,256-310]ib" << true;


    QTest::newRow("Malformed range") << "nodes[000-999]" << "nodes[005-010,123,256-310" << "" << false;

    QTest::newRow("Search with spaces") << "nodes[000-999]" << " nodes[ 005-010, 123 , 256 - 310 ]  " << "nodes[005-010,123,256-310]" << true;
    QTest::newRow("Search multi range suffix with spaces") << "nodes[000-999],nodes[000-999]ib"
                                        << " nodes[ 005-010, 123 , 256 - 310 ] , nodes[ 015-018, 128 , 360 - 720 ]ib  "
                                        << "nodes[005-010,123,256-310],nodes[015-018,128,360-720]ib"
                                        << true;

}

void TestNodeListView::testNodeListView()
{
    NodeListView view;

    QFETCH(QString, nodeList);
    QFETCH(QString, nodeSearch);
    QFETCH(QString, nodeSelection);
    QFETCH(bool, nodeSearchIsValid);

    view.setNodes(nodeList);
    view.setSearchText(nodeSearch);

    QCOMPARE(view.nodes(), nodeList);
    QCOMPARE(view.selectedNodes(), nodeSelection);
    QCOMPARE(view.isValid(), nodeSearchIsValid);
}


void TestNodeListView::testSlurm()
{
    QString nodeList = QString("node[000-123,125,127-128]");
    setenv("SLURM_JOB_NODELIST", nodeList.toLocal8Bit().data(), 1);
    QCOMPARE(Slurm::nodeList(), nodeList);

    quint64 nodeCount = 127;
    setenv("SLURM_JOB_NUM_NODES", (QString("%1").arg(nodeCount)).toLocal8Bit().data(), 1);
    QCOMPARE(Slurm::nodeCount(), nodeCount);

    int cpuCount = 64;
    setenv("SLURM_JOB_CPUS_PER_NODE", (QString("%1(x%2)").arg(cpuCount).arg(nodeCount)).toLocal8Bit().data(), 1);
    QCOMPARE(Slurm::cpusPerNode(), cpuCount);
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
