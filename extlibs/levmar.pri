contains( HAS_LAPACK, true ) {
   LIBS += $$LAPACK_LINK -L../../extlibs/levmar/ -llevmar -L$$QFOUTPUT -lquickfit3lib -lquickfit3widgets
   INCLUDEPATH += $$LAPACK_INCLUDE \
                  ../../extlibs/
} else {
   LIBS += -L../../extlibs/levmar/ -llevmar -L$$QFOUTPUT -lquickfit3lib -lquickfit3widgets
   INCLUDEPATH += ../../extlibs/
}
