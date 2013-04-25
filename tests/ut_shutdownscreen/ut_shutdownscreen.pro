include(../common.pri)
TARGET = ut_shutdownscreen
CONFIG += link_pkgconfig
INCLUDEPATH += $$SRCDIR $$NOTIFICATIONSRCDIR $$UTILITYSRCDIR $$XTOOLSRCDIR
QT += declarative

packagesExist(qmsystem2) {
    PKGCONFIG += qmsystem2
    DEFINES += HAVE_QMSYSTEM
} else {
    warning("QmSystem2 not found")
}

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
