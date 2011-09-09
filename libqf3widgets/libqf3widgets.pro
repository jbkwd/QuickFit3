TEMPLATE = lib
TARGET = quickfit3widgets

include(../quickfit3.pri)

release {
    message("building $$TARGET $$TEMPLATE in RELEASE mode, output is in $$QFOUTPUT")
} else {
    message("building $$TARGET $$TEMPLATE in DEBUG mode, output is in $$QFOUTPUT")
}

DEPENDPATH += .

INCLUDEPATH += ../../../../LIB/trunk/ \
               . \
               ../lib/ \
               ../plugins/interfaces/

LIBS += -L$$QFOUTPUT -lquickfit3lib

QT += gui xml svg opengl



DESTDIR = $$QFOUTPUT


# Input
HEADERS += ../../../../LIB/trunk/qt/jkqtpbaseplotter.h \
           ../../../../LIB/trunk/qt/jkqtpdatastorage.h \
           ../../../../LIB/trunk/qt/jkqtpelements.h \
           ../../../../LIB/trunk/qt/jkqtpgeoelements.h \
           ../../../../LIB/trunk/qt/jkqtmathtext.h \
           ../../../../LIB/trunk/qt/jkqtpbaseelements.h \
           ../../../../LIB/trunk/qt/jkqtplotter.h \
           ../../../../LIB/trunk/qt/jkqtptools.h \
           ../../../../LIB/trunk/qt/jkqttools.h \
           ../../../../LIB/trunk/qt/flowlayout.h \
           ../../../../LIB/trunk/qt/jkqtfastplotter.h \
           ../../../../LIB/trunk/qt/qtlogfile.h \
           ../../../../LIB/trunk/qt/qtshortcutwidgets.h \
           ../../../../LIB/trunk/tools.h \
           ../../../../LIB/trunk/datatable2.h \
           ../../../../LIB/trunk/qt/numberedit.h \
           ../../../../LIB/trunk/qt/jkdoubleedit.h \
           ../../../../LIB/trunk/qt/jkverticalscrollarea.h \
           ../../../../LIB/trunk/qt/colorcombobox.h \
           ../../../../LIB/trunk/qt/jkstyledbutton.h \
           ../../../../LIB/trunk/qt/qenhancedlineedit.h \
           ../../../../LIB/trunk/qt/qspecialtoolbutton.h \
           ../../../../LIB/trunk/qt/qmodernprogresswidget.h \
           ../../../../LIB/trunk/qt/qmoretextobject.h \
           qehtreeview.h \
           datacutslider.h \
           qffitparameterbasicinterface.h \
           qffitparameterwidget.h \
           qfdlgcsvparameters.h \
    qfstagecombobox.h \
    qfcameraconfigcombobox.h \
    qfcameracombobox.h \
    objectives.h \
    ../plugins/interfaces/qfrdrfcsdatainterface.h \
    ../plugins/interfaces/qfrdrcountratesinterface.h \
    ../plugins/interfaces/qfextensionparameterdevice.h \
    ../plugins/interfaces/qfextensionlinearstage.h \
    ../plugins/interfaces/qfextensioncamera.h \
    libwid_imexport.h \
    ../../../../LIB/trunk/lib_imexport.h \
    ../../../../LIB/trunk/qt/jkqtpimagetools.h \
    ../../../../LIB/trunk/qt/jkqtpimageelements.h \
    ../../../../LIB/trunk/qt/htmldelegate.h


SOURCES += ../../../../LIB/trunk/qt/jkqtpbaseplotter.cpp \
           ../../../../LIB/trunk/qt/jkqtpdatastorage.cpp \
           ../../../../LIB/trunk/qt/jkqtpelements.cpp \
           ../../../../LIB/trunk/qt/jkqtpgeoelements.cpp \
           ../../../../LIB/trunk/qt/jkqtmathtext.cpp \
           ../../../../LIB/trunk/qt/jkqtpbaseelements.cpp \
           ../../../../LIB/trunk/qt/jkqtplotter.cpp \
           ../../../../LIB/trunk/qt/jkqtptools.cpp \
           ../../../../LIB/trunk/qt/jkqttools.cpp \
            ../../../../LIB/trunk/qt/flowlayout.cpp \
           ../../../../LIB/trunk/qt/qtlogfile.cpp \
           ../../../../LIB/trunk/qt/jkqtfastplotter.cpp \
           ../../../../LIB/trunk/qt/qtshortcutwidgets.cpp \
           ../../../../LIB/trunk/tools.cpp \
           ../../../../LIB/trunk/datatable2.cpp \
           ../../../../LIB/trunk/qt/numberedit.cpp \
           ../../../../LIB/trunk/qt/jkdoubleedit.cpp \
           ../../../../LIB/trunk/qt/jkverticalscrollarea.cpp \
           ../../../../LIB/trunk/qt/colorcombobox.cpp \
           ../../../../LIB/trunk/qt/jkstyledbutton.cpp \
           ../../../../LIB/trunk/qt/qenhancedlineedit.cpp \
           ../../../../LIB/trunk/qt/qspecialtoolbutton.cpp \
           ../../../../LIB/trunk/qt/qmodernprogresswidget.cpp \
           ../../../../LIB/trunk/qt/qmoretextobject.cpp \
           qehtreeview.cpp \
           datacutslider.cpp \
           qffitparameterwidget.cpp \
           qfdlgcsvparameters.cpp \
    qfstagecombobox.cpp \
    qfcameraconfigcombobox.cpp \
    qfcameracombobox.cpp \
    objectives.cpp \
    ../../../../LIB/trunk/qt/jkqtpimageelements.cpp \
    ../../../../LIB/trunk/qt/htmldelegate.cpp

FORMS += qfdlg_csvparameters.ui \
    qf3objectiveeditor.ui

RESOURCES += ../../../../LIB/trunk/qt/jkqtpbaseplotter.qrc \
             ../../../../LIB/trunk/qt/qspecialtoolbutton.qrc \
             ../../../../LIB/trunk/qt/jkstyledbutton.qrc \
    libqf3widgets.qrc \
    ../../../../LIB/trunk/qt/libqt.qrc

TRANSLATIONS= ./translations/de.quickfit3widgets.ts

INCLUDEPATH += ../../../../LIB/trunk/ \
               ../../../../LIB/trunk/qt/

CONFIG += dll lib_bundle exceptions rtti stl create_prl

DEFINES += QFWIDLIB_LIBRARY LIB_IN_DLL LIB_EXPORT_LIBRARY






