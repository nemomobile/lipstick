include(../common.pri)
TARGET = ut_shutdownscreen
CONFIG += link_pkgconfig
INCLUDEPATH += $$SRCDIR $$NOTIFICATIONSRCDIR $$UTILITYSRCDIR $$XTOOLSRCDIR
QT += declarative

equals(QT_MAJOR_VERSION, 4): QMSYSTEM = qmsystem2
equals(QT_MAJOR_VERSION, 5): QMSYSTEM = qmsystem2-qt5

packagesExist($$QMSYSTEM) {
    PKGCONFIG += $$QMSYSTEM
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
