include(../common.pri)
TARGET = ut_batterynotifier
CONFIG += link_pkgconfig
INCLUDEPATH += $$NOTIFICATIONSRCDIR /usr/include/QtSystemInfo /usr/include/qmsystem2-qt5
QT += dbus systeminfo

HEADERS += \
    $$NOTIFICATIONSRCDIR/batterynotifier.h \
    $$NOTIFICATIONSRCDIR/lowbatterynotifier.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$NOTIFICATIONSRCDIR/notification.h \
    /usr/include/qmsystem2-qt5/qmled.h \
    /usr/include/qmsystem2-qt5/qmdevicemode.h \
    /usr/include/qmsystem2-qt5/qmdisplaystate.h \
    $$STUBSDIR/stubbase.h \
    ut_batterynotifier.h

SOURCES += \
    $$NOTIFICATIONSRCDIR/batterynotifier.cpp \
    $$NOTIFICATIONSRCDIR/notification.cpp \
    $$STUBSDIR/stubbase.cpp \
    ut_batterynotifier.cpp
