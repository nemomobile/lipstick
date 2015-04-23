include(../common.pri)
TARGET = ut_screenlock
QT += dbus qml quick

INCLUDEPATH += $$SCREENLOCKSRCDIR $$UTILITYSRCDIR $$SRCDIR/xtools

SOURCES += ut_screenlock.cpp \
    $$SCREENLOCKSRCDIR/screenlock.cpp \
    $$STUBSDIR/stubbase.cpp

HEADERS += ut_screenlock.h \
    $$SCREENLOCKSRCDIR/screenlock.h \
    $$UTILITYSRCDIR/closeeventeater.h
