include(../common_top.pri)
TARGET = ut_desktop

MODEL_HEADERS += $$SRCDIR/desktopmodel.h

# unit test and unit classes
SOURCES += \
    ut_desktop.cpp \
    $$SRCDIR/desktop.cpp

# service classes
SOURCES += \
    $$SRCDIR/windowinfo.cpp \
    $$SRCDIR/x11wrapper.cpp

# unit test and unit classes
HEADERS += \
    ut_desktop.h \
    $$SRCDIR/desktopmodel.h \
    $$SRCDIR/desktop.h \
    $$SRCDIR/windowmonitor.h \
    $$SRCDIR/windowinfo.h
