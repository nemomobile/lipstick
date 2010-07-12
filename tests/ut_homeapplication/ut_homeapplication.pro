include(../common_top.pri)
TARGET = ut_homeapplication
INCLUDEPATH += $$SRCDIR/notifications

# unit test and unit
SOURCES += \
    ut_homeapplication.cpp \
    $$SRCDIR/homeapplication.cpp

# helper classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \
    $$SRCDIR/homescreenservice.cpp \
    $$SRCDIR/homescreenadaptor.cpp

# unit test and unit
HEADERS += \
    ut_homeapplication.h \
    $$SRCDIR/homeapplication.h \

# helper classes
HEADERS += \
    $$SRCDIR/windowinfo.h \
    $$SRCDIR/mainwindow.h \
    $$SRCDIR/homescreenservice.h \
    $$SRCDIR/homescreenadaptor.h \
    $$SRCDIR/switcher.h

SOURCES +=
include(../common_bot.pri)
