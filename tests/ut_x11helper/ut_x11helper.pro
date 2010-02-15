include(../common_top.pri)
TARGET = ut_x11helper

# unit test and unit
SOURCES += \
    ut_x11helper.cpp \
    $$SRCDIR/x11helper.cpp

# unit test and unit
HEADERS += \
    ut_x11helper.h \
    $$SRCDIR/x11helper.h

include(../common_bot.pri)
