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

equals(QT_MAJOR_VERSION, 4): QMSYSTEM = qmsystem2
equals(QT_MAJOR_VERSION, 5): QMSYSTEM = qmsystem2-qt5

packagesExist($$QMSYSTEM) {
    DEFINES += HAVE_QMSYSTEM
    INCLUDEPATH += /usr/include/$$QMSYSTEM
    HEADERS += /usr/include/$$QMSYSTEM/qmlocks.h \
    /usr/include/$$QMSYSTEM/qmusbmode.h
} else {
    warning("QmSystem2 not found")
}

