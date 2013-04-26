include(../common.pri)
TARGET = ut_screenlock
QT += dbus declarative
CONFIG += link_pkgconfig
equals(QT_MAJOR_VERSION, 4): PKGCONFIG += x11

INCLUDEPATH += $$SCREENLOCKSRCDIR $$UTILITYSRCDIR $$SRCDIR/xtools

SOURCES += ut_screenlock.cpp \
    $$SCREENLOCKSRCDIR/screenlock.cpp \
    $$STUBSDIR/stubbase.cpp

HEADERS += ut_screenlock.h \
    $$SCREENLOCKSRCDIR/screenlock.h \
    $$UTILITYSRCDIR/closeeventeater.h

equals(QT_MAJOR_VERSION, 4): HEADERS += $$SCREENLOCKSRCDIR/eventeater.h
