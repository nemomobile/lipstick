include(../common.pri)
TARGET = ut_shutdownscreen
CONFIG += link_pkgconfig
PKGCONFIG += qmsystem2
INCLUDEPATH += $$SRCDIR $$NOTIFICATIONSRCDIR $$UTILITYSRCDIR $$XTOOLSRCDIR
QT += declarative

# unit test and unit
SOURCES += \
    $$SRCDIR/shutdownscreen.cpp \
    $$STUBSDIR/stubbase.cpp \
    ut_shutdownscreen.cpp

HEADERS += \
    $$SRCDIR/shutdownscreen.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$UTILITYSRCDIR/closeeventeater.h \
    $$SRCDIR/homeapplication.h \
    ut_shutdownscreen.h
