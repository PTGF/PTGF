# This file is part of the Parallel Tools GUI Framework (PTGF)
# Copyright (C) 2010-2015 Argo Navis Technologies, LLC
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

include(PTGF.pri)

TEMPLATE = subdirs
CONFIG  += ordered
SUBDIRS  = core plugins tests

OTHER_FILES += fileheader.txt

core.subdir = core

plugins.subdir = plugins
plugins.depends = core

tests.subdir = tests
tests.depends = plugins


# Add target for documentation generation (via Doxygen)
OTHER_FILES += Doxyfile
DOXYSCRIPT = $$quote($${SOURCE_PATH}/../bin/doxygen.sh)
DOXYFILE = $$quote($${SOURCE_PATH}/Doxyfile)
DOXYINSTALL= $$quote(${INSTALL_ROOT}/share/ptgf)

doxygen.target = doc
doxygen.commands = @-test -x $${DOXYSCRIPT} && \
                     $${DOXYSCRIPT} --source=$${SOURCE_PATH} --doxyfile=$${DOXYFILE};

doxygen-install.target = doc-install
doxygen-install.commands = @-test -x $${DOXYSCRIPT} && \
                             $${DOXYSCRIPT} --source=$${SOURCE_PATH} --doxyfile=$${DOXYFILE} --install=$${DOXYINSTALL};
doxygen-install.depends = doxygen

doxygen-uninstall.target = doc-uninstall
doxygen-uninstall.commands = @-test -x $${DOXYSCRIPT} && \
                               $${DOXYSCRIPT} --uninstall=$${DOXYINSTALL};

QMAKE_EXTRA_TARGETS += doxygen doxygen-install doxygen-uninstall
