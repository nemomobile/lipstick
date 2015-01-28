include(../common.pri)
TARGET = ut_qobjectlistmodel
INCLUDEPATH += $$UTILITYSRCDIR
INCLUDEPATH += $$3RDPARTYSRCDIR

# unit test and unit
SOURCES += \
    ut_qobjectlistmodel.cpp \
    $$UTILITYSRCDIR/qobjectlistmodel.cpp

# unit test and unit
HEADERS += \
    $$UTILITYSRCDIR/qobjectlistmodel.h \
    $$3RDPARTYSRCDIR/synchronizelists.h
