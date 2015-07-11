#
#    Copyright (c) 2008-2014 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center (DKFZ) & IWR, University of Heidelberg
#	
#	 last modification: $LastChangedDate$  (revision $Rev$)
#
#    This file is part of QuickFit 3 (http://www.dkfz.de/Macromol/quickfit).
#
#    This software is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

TEMPLATE = lib
CONFIG += plugin

TARGET = importers_basicimages
DEPENDPATH += ./

include(../plugins.pri)
include(../../extlibs/zlib.pri)
include(../../extlibs/tiff.pri)
DESTDIR = $$QFOUTPUT/plugins

# Input
HEADERS += qfbasicimageimporters.h \
           qfimagereaderlibtiff.h \
    ../../global_lib/libtiff_tools.h \
    qfimagereaderradhard.h \
    ../../global_lib/crc_ccitt.h \
    ../../global_lib/crc.h \
    ../../global_lib/frame.h \
    qfbasicimageimportersoptionswidget.h \
    qfimagereadertinytiff.h \
    ../../global_lib/tinytiffwriter.h \
    ../../global_lib/tinytiffreader.h \
    qfimagereaderdiffusion4binary.h \
    ../../global_lib/helper.h \
    qfimagereaderraw.h

SOURCES += qfbasicimageimporters.cpp\
           qfimagereaderlibtiff.cpp \
    ../../global_lib/libtiff_tools.cpp \
    qfimagereaderradhard.cpp \
    qfbasicimageimportersoptionswidget.cpp \
    qfimagereadertinytiff.cpp \
    ../../global_lib/tinytiffwriter.cpp \
    ../../global_lib/tinytiffreader.cpp \
    qfimagereaderdiffusion4binary.cpp \
    qfimagereaderraw.cpp

FORMS = \
    qfbasicimageimportersoptionswidget.ui

RESOURCES += \
    basicimageimporters.qrc

TRANSLATIONS= ./translations/de.importers_basicimages.ts


INCLUDEPATH += ../../lib/


TINYTIFF_FILES.files = ../../global_lib/tinytiffwriter.cpp \
                       ../../global_lib/tinytiffreader.cpp  \
                       ../../global_lib/tinytiffwriter.h \
                       ../../global_lib/tinytiffreader.h \
                       $$GPL3LICENSE
TINYTIFF_FILES.path = $${QFOUTPUT}/source/tinytiff/
tinytiff_reader_test_FILES.files = ../../global_lib/test/tinytiff_reader_test/*.*
tinytiff_reader_test_FILES.path = $${QFOUTPUT}/source/tinytiff/test/tinytiff_reader_test
tinytiffwriter_test_FILES.files = ../../global_lib/test/tinytiffwriter_test/*.*
tinytiffwriter_test_FILES.path = $${QFOUTPUT}/source/tinytiff/test/tinytiffwriter_test

INSTALLS += TINYTIFF_FILES \
            tinytiff_reader_test_FILES \
            tinytiffwriter_test_FILES


