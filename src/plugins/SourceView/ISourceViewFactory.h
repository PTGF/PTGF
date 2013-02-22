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

#ifndef ISOURCEVIEWFACTORY_H
#define ISOURCEVIEWFACTORY_H

#include <QtCore>
#include <QtGui>

#include "SourceViewLibrary.h"

namespace Plugins {
namespace SourceView {

class SourceView;

class SOURCEVIEW_EXPORT ISourceViewFactory
{
public:
    virtual SourceView *sourceViewWidget(const QString &text) = 0;
};

} // namespace SourceView
} // namespace Plugins

#ifndef STRINGIFY
#define STRINGIFY(X) # X
#endif

#define ISOURCEVIEWFACTORY_VERSION "org.krellinst.ptgf.ISourceViewFactory/" STRINGIFY(VER_MAJ) "." STRINGIFY(VER_MIN)
Q_DECLARE_INTERFACE(Plugins::SourceView::ISourceViewFactory, ISOURCEVIEWFACTORY_VERSION)

#endif // ISOURCEVIEWFACTORY_H
