include(../common.pri)
TARGET = ut_shutdownscreen
INCLUDEPATH += $$SRCDIR $$NOTIFICATIONSRCDIR $$UTILITYSRCDIR $$XTOOLSRCDIR ../../src/qmsystem2
QT += qml quick dbus

CONFIG += link_pkgconfig
PKGCONFIG += dsme_dbus_if

# unit test and unit
SOURCES += \
    $$SRCDIR/shutdownscreen.cpp \
    $$NOTIFICATIONSRCDIR/lipsticknotification.cpp \
    $$STUBSDIR/stubbase.cpp \
    $$STUBSDIR/homewindow.cpp \
    $$STUBSDIR/homeapplication.cpp \
    ../../src/qmsystem2/qmsystemstate.cpp \
    ../../src/qmsystem2/qmipcinterface.cpp \
    ut_shutdownscreen.cpp

HEADERS += \
    $$SRCDIR/shutdownscreen.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$NOTIFICATIONSRCDIR/lipsticknotification.h \
    $$UTILITYSRCDIR/closeeventeater.h \
    $$SRCDIR/homeapplication.h \
    $$SRCDIR/homewindow.h \
    ../../src/qmsystem2/qmsystemstate.h \
    ../../src/qmsystem2/qmsystemstate_p.h \
    ../../src/qmsystem2/qmipcinterface_p.h \
    ut_shutdownscreen.h
