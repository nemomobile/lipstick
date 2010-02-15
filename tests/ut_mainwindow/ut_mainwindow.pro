include(../common_top.pri)
TARGET = ut_mainwindow

# unit test and unit
SOURCES += \
    ut_mainwindow.cpp \
    $$SRCDIR/mainwindow.cpp \
    ../stubs/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_mainwindow.h \
    $$SRCDIR/mainwindow.h

SOURCES +=
include(../common_bot.pri)
