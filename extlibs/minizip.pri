#
#    Copyright (c) 2008-2014 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center
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

include(zlib.pri)

!contains(DEFINES, QF_HAS_MINIZIP) {
	DEFINES += QF_HAS_MINIZIP

	INCLUDEPATH += $$QFOUTPUT/../extlibs/minizip/

	HEADERS += $$QFOUTPUT/../extlibs/minizip/minizip/unzip11/crypt.h \
						$$QFOUTPUT/../extlibs/minizip/minizip/unzip11/ioapi.h \
						$$QFOUTPUT/../extlibs/minizip/minizip/unzip11/iowin32.h \
						$$QFOUTPUT/../extlibs/minizip/minizip/unzip11/mztools.h \
						$$QFOUTPUT/../extlibs/minizip/minizip/unzip11/unzip.h \
						$$QFOUTPUT/../extlibs/minizip/minizip/unzip11/zip.h 
				

	SOURCES += $$QFOUTPUT/../extlibs/minizip/minizip/unzip11/ioapi.c \
						$$QFOUTPUT/../extlibs/minizip/minizip/unzip11/iowin32.c \
						$$QFOUTPUT/../extlibs/minizip/minizip/unzip11/mztools.c \
						$$QFOUTPUT/../extlibs/minizip/minizip/unzip11/unzip.c \
						$$QFOUTPUT/../extlibs/minizip/minizip/unzip11/zip.c 

	win32 {
		HEADERS += $$QFOUTPUT/../extlibs/minizip/minizip/unzip11/iowin32.h
		SOURCES += $$QFOUTPUT/../extlibs/minizip/minizip/unzip11/iowin32.c
	}
}else{
	INCLUDEPATH += /usr/include/minizip/
}

