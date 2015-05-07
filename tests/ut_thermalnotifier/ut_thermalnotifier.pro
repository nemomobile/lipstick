include(../common.pri)
TARGET = ut_thermalnotifier
CONFIG += link_pkgconfig
INCLUDEPATH += $$SRCDIR $$NOTIFICATIONSRCDIR $$UTILITYSRCDIR $$XTOOLSRCDIR ../../src/qmsystem2
QT += qml quick dbus

# unit test and unit
SOURCES += \
    $$NOTIFICATIONSRCDIR/thermalnotifier.cpp \
    $$NOTIFICATIONSRCDIR/lipsticknotification.cpp \
    $$STUBSDIR/stubbase.cpp \
    $$STUBSDIR/homeapplication.cpp \
    ut_thermalnotifier.cpp

HEADERS += \
    $$NOTIFICATIONSRCDIR/thermalnotifier.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$NOTIFICATIONSRCDIR/lipsticknotification.h \
    $$SRCDIR/homeapplication.h \
    ../../src/qmsystem2/qmthermal.h \
    ../../src/qmsystem2/qmdisplaystate.h \
    ut_thermalnotifier.h
