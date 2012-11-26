include(../common.pri)
TARGET = ut_closeeventeater

INCLUDEPATH += $$UTILITYSRCDIR

# unit test and unit
SOURCES += \
    ut_closeeventeater.cpp \
    $$UTILITYSRCDIR/closeeventeater.cpp

# unit test and unit
HEADERS += \
    ut_closeeventeater.h \
    $$UTILITYSRCDIR/closeeventeater.h
