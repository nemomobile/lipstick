include(../common.pri)
TARGET = ut_batterynotifier
CONFIG += link_pkgconfig
PKGCONFIG += contextsubscriber-1.0
INCLUDEPATH += $$NOTIFICATIONSRCDIR /usr/include/qmsystem2
QT += dbus

HEADERS += \
    $$NOTIFICATIONSRCDIR/batterynotifier.h \
    $$NOTIFICATIONSRCDIR/lowbatterynotifier.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$NOTIFICATIONSRCDIR/notification.h \
    /usr/include/qmsystem2/qmled.h \
    /usr/include/qmsystem2/qmbattery.h \
    /usr/include/qmsystem2/qmdevicemode.h \
    /usr/include/qmsystem2/qmdisplaystate.h \
    $$STUBSDIR/stubbase.h \
    ut_batterynotifier.h

SOURCES += \
    $$NOTIFICATIONSRCDIR/batterynotifier.cpp \
    $$NOTIFICATIONSRCDIR/notification.cpp \
    $$STUBSDIR/stubbase.cpp \
    ut_batterynotifier.cpp
