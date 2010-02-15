include(../common_top.pri)
TARGET = ut_desktop

MODEL_HEADERS += $$SRCDIR/desktopmodel.h

# unit test and unit classes
SOURCES += \
    ut_desktop.cpp \
    $$SRCDIR/desktop.cpp

# unit test and unit classes
HEADERS += \
    ut_desktop.h \
    $$SRCDIR/desktopmodel.h \
    $$SRCDIR/desktop.h
