include(../common.pri)
TARGET = ut_screenlock
QT += dbus qml quick
CONFIG += link_pkgconfig

PKGCONFIG += qmsystem2-qt5

INCLUDEPATH += $$SCREENLOCKSRCDIR $$UTILITYSRCDIR $$SRCDIR/xtools

SOURCES += ut_screenlock.cpp \
    $$SCREENLOCKSRCDIR/screenlock.cpp \
    $$STUBSDIR/stubbase.cpp

HEADERS += ut_screenlock.h \
    $$SCREENLOCKSRCDIR/screenlock.h \
    $$UTILITYSRCDIR/closeeventeater.h
