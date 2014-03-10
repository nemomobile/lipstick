include(../common.pri)
TARGET = ut_thermalnotifier
CONFIG += link_pkgconfig
INCLUDEPATH += $$SRCDIR $$NOTIFICATIONSRCDIR $$UTILITYSRCDIR $$XTOOLSRCDIR
QT += qml quick dbus
PKGCONFIG += qmsystem2-qt5

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
    ut_thermalnotifier.h
