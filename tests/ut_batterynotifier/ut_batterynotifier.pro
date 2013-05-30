include(../common.pri)
TARGET = ut_batterynotifier
CONFIG += link_pkgconfig mobility
MOBILITY += systeminfo
INCLUDEPATH += $$NOTIFICATIONSRCDIR /usr/include/QtSystemInfo
QT += dbus

packagesExist(contextsubscriber-1.0) {
    PKGCONFIG += contextsubscriber-1.0
    DEFINES += HAVE_CONTEXTSUBSCRIBER
} else {
    warning("Contextsubscriber not found")
}

packagesExist(qmsystem2) {
    DEFINES += HAVE_QMSYSTEM
    INCLUDEPATH += /usr/include/qmsystem2
} else {
    warning("QmSystem2 not found")
}

HEADERS += \
    $$NOTIFICATIONSRCDIR/batterynotifier.h \
    $$NOTIFICATIONSRCDIR/lowbatterynotifier.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$NOTIFICATIONSRCDIR/lipsticknotification.h \
    /usr/include/qmsystem2/qmled.h \
    /usr/include/qmsystem2/qmdevicemode.h \
    /usr/include/qmsystem2/qmdisplaystate.h \
    $$STUBSDIR/stubbase.h \
    ut_batterynotifier.h

SOURCES += \
    $$NOTIFICATIONSRCDIR/batterynotifier.cpp \
    $$NOTIFICATIONSRCDIR/lipsticknotification.cpp \
    $$STUBSDIR/stubbase.cpp \
    ut_batterynotifier.cpp
