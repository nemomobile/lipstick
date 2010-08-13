include(../common_top.pri)
TARGET = ut_xeventlistener

# unit test and unit
SOURCES += \
    ut_xeventlistener.cpp \
    $$SRCDIR/xeventlistener.cpp \
    $$STUBSDIR/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_xeventlistener.h \
    $$SRCDIR/xeventlistener.h \
    $$SRCDIR/homeapplication.h \
    $$STUBSDIR/stubbase.h

include(../common_bot.pri)
