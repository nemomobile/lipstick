include(../common_top.pri)
TARGET = ut_desktopbackground

STYLE_HEADERS += $$SRCDIR/desktopstyle.h
# unit test and unit
SOURCES += \
    ut_desktopbackground.cpp \
    $$SRCDIR/desktopbackground.cpp \
    ../stubs/stubbase.cpp \
    ../stubs/mainwindow_stub.h

# unit test and unit
HEADERS += \
    ut_desktopbackground.h \
    $$SRCDIR/mainwindow.h \
    $$SRCDIR/desktopbackground.h \
    $$SRCDIR/desktopstyle.h


include(../common_bot.pri)
