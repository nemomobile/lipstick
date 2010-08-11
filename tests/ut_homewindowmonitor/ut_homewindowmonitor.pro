include(../common_top.pri)
TARGET = ut_homewindowmonitor

# unit test and unit
SOURCES += \
    ut_homewindowmonitor.cpp \
    $$SRCDIR/homewindowmonitor.cpp \
    ../stubs/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_homewindowmonitor.h \
    $$SRCDIR/homewindowmonitor.h \
    ../stubs/stubbase.h

include(../common_bot.pri)
