/*!
   \file Slurm.cpp
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

#include "Slurm.h"
#include "stdlib.h"

namespace Plugins {
namespace NodeListView {

Slurm::Slurm(QObject *parent) :
    QObject(parent)
{
}

/*!
   \brief Returns a list of nodes in a job allocation
   Returns an empty string if a failure occurs while gathering the information
   \return
 */
QString Slurm::nodeList()
{
    QString retval = QString(getenv("SLURM_JOB_NODELIST"));

    if(retval.isEmpty()) {
        retval = QString(getenv("SLURM_NODELIST"));
    }

    return retval;
}

/*!
   \brief Returns the number of nodes in an job allocation
   Returns 0 if a failure occurs while gathering the information
   \return
 */
quint64 Slurm::nodeCount()
{
    QString string = QString(getenv("SLURM_JOB_NUM_NODES"));

    bool okay;
    quint64 retval = string.toLongLong(&okay);
    if(!okay) {
        return 0;
    }

    return retval;
}

/*!
   \brief Returns the CPU count per node in a job allocation
   Returns 0 if a failure occurs while gathering the information
   \return
 */
int Slurm::cpusPerNode()
{
    QString string = QString(getenv("SLURM_JOB_CPUS_PER_NODE"));
    if(string.isEmpty()) {
        return 0;
    }

    bool okay;
    int retval = string.left(string.indexOf('(')).toInt(&okay);
    if(!okay) {
        return 0;
    }

    return retval;
}




} // namespace NodeListView
} // namespace Plugins
