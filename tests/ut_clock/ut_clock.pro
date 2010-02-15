include(../common_top.pri)
TARGET = ut_clock

STYLE_HEADERS += $$SRCDIR/clockstyle.h
MODEL_HEADERS += $$SRCDIR/clockmodel.h

# unit test and unit
SOURCES += \
    ut_clock.cpp \
    $$SRCDIR/clock.cpp

# unit test and unit
HEADERS += \
    ut_clock.h \
    $$SRCDIR/clock.h \
    $$SRCDIR/clockstyle.h \
    $$SRCDIR/clockmodel.h

include(../common_bot.pri)
