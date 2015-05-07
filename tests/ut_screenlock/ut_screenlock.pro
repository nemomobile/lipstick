include(../common.pri)
TARGET = ut_screenlock
QT += dbus qml quick

INCLUDEPATH += $$SCREENLOCKSRCDIR $$UTILITYSRCDIR $$SRCDIR/xtools ../../src/qmsystem2

SOURCES += ut_screenlock.cpp \
    $$SCREENLOCKSRCDIR/screenlock.cpp \
    ../../src/qmsystem2/qmdisplaystate.cpp \
    $$STUBSDIR/stubbase.cpp

HEADERS += ut_screenlock.h \
    $$SCREENLOCKSRCDIR/screenlock.h \
    ../../src/qmsystem2/qmdisplaystate.h \
    ../../src/qmsystem2/qmdisplaystate_p.h \
    $$UTILITYSRCDIR/closeeventeater.h
