include(../common.pri)
TARGET = ut_screenlock
QT += dbus declarative
CONFIG += link_pkgconfig
PKGCONFIG += x11

INCLUDEPATH += $$SCREENLOCKSRCDIR $$UTILITYSRCDIR $$SRCDIR/xtools

SOURCES += ut_screenlock.cpp \
    $$SCREENLOCKSRCDIR/screenlock.cpp \
    $$STUBSDIR/stubbase.cpp

HEADERS += ut_screenlock.h \
    $$SCREENLOCKSRCDIR/screenlock.h \
    $$SCREENLOCKSRCDIR/eventeater.h \
    $$UTILITYSRCDIR/closeeventeater.h
