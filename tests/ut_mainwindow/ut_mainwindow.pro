include(../common_top.pri)
TARGET = ut_mainwindow

STYLE_HEADERS += mainwindowstyle.h

# unit test and unit
SOURCES += \
    ut_mainwindow.cpp \
    $$SRCDIR/mainwindow.cpp \
    ../stubs/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_mainwindow.h \
    $$SRCDIR/mainwindow.h \
    $$SRCDIR/mainwindowstyle.h \
    $$SRCDIR/homeapplication.h

SOURCES +=
include(../common_bot.pri)
