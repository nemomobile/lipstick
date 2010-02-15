include(../common_top.pri)
TARGET = ut_clockview

STYLE_HEADERS += $$SRCDIR/clockstyle.h
MODEL_HEADERS += $$SRCDIR/clockmodel.h

# unit test and unit
SOURCES += \
    ut_clockview.cpp \
    clockview.cpp

# service classes
SOURCES += ../stubs/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_clockview.h \
    $$SRCDIR/clockview.h \
    $$SRCDIR/clockmodel.h \
    $$SRCDIR/clockstyle.h \
    $$SRCDIR/clock.h

include(../common_bot.pri)
