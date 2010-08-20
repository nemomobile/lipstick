include(../common_top.pri)
TARGET = ut_homewindowmonitor

# unit test and unit
SOURCES += \
    ut_homewindowmonitor.cpp \
    $$SRCDIR/homewindowmonitor.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp 
#    $$SRCDIR/x11wrapper.cpp

# unit test and unit
HEADERS += \
    ut_homewindowmonitor.h \
    $$SRCDIR/homewindowmonitor.h \
    $$SRCDIR/xeventlistener.h \
    $$SRCDIR/windowmonitor.h
    

include(../common_bot.pri)
