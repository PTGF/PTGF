/*!
   \file TestNodeListView.h
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

#ifndef TESTNODELISTVIEW_H
#define TESTNODELISTVIEW_H

#include <QObject>

class TestNodeListView : public QObject
{
    Q_OBJECT
public:
    explicit TestNodeListView(QObject *parent = 0);

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testNodeListView_data();
    void testNodeListView();

    void testSlurm();

    void testRange();

    void testNodeRange();

    void testNodeRangeSequentialMerge_data();
    void testNodeRangeSequentialMerge();

    void testNodeRangeRandomMerge_data();
    void testNodeRangeRandomMerge();

};

#endif // TESTNODELISTVIEW_H
