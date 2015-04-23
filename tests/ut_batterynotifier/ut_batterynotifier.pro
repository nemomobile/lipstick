include(../common.pri)
TARGET = ut_batterynotifier
CONFIG += link_pkgconfig
INCLUDEPATH += $$NOTIFICATIONSRCDIR /usr/include/QtSystemInfo ../../src/qmsystem2
QT += dbus systeminfo

HEADERS += \
    $$NOTIFICATIONSRCDIR/batterynotifier.h \
    $$NOTIFICATIONSRCDIR/lowbatterynotifier.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$NOTIFICATIONSRCDIR/lipsticknotification.h \
    ../../src/qmsystem2/qmdisplaystate.h \
    $$STUBSDIR/stubbase.h \
    ut_batterynotifier.h

SOURCES += \
    $$NOTIFICATIONSRCDIR/batterynotifier.cpp \
    $$NOTIFICATIONSRCDIR/lipsticknotification.cpp \
    $$STUBSDIR/stubbase.cpp \
    ut_batterynotifier.cpp

packagesExist(contextkit-statefs) {
    DEFINES += HAVE_CONTEXTSUBSCRIBER
    HEADERS += /usr/include/contextproperty.h
} else {
    warning("Contextsubscriber not found")
}
