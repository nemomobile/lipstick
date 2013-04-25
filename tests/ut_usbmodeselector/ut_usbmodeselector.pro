include(../common.pri)
TARGET = ut_usbmodeselector
INCLUDEPATH += $$SRCDIR $$NOTIFICATIONSRCDIR $$UTILITYSRCDIR
QT += declarative

# unit test and unit
SOURCES += \
    $$SRCDIR/usbmodeselector.cpp \
    $$STUBSDIR/stubbase.cpp \
    ut_usbmodeselector.cpp

HEADERS += \
    $$SRCDIR/usbmodeselector.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$UTILITYSRCDIR/closeeventeater.h \
    ut_usbmodeselector.h

packagesExist(qmsystem2) {
    DEFINES += HAVE_QMSYSTEM
    INCLUDEPATH += /usr/include/qmsystem2
    HEADERS += /usr/include/qmsystem2/qmlocks.h \
    /usr/include/qmsystem2/qmusbmode.h
} else {
    warning("QmSystem2 not found")
}

