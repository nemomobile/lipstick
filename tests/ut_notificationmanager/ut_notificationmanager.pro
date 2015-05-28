include(../common.pri)
TARGET = ut_notificationmanager
INCLUDEPATH += $$NOTIFICATIONSRCDIR
CONFIG += link_pkgconfig
QT += sql dbus
PKGCONFIG += mlite5

# unit test and unit
SOURCES += \
    ut_notificationmanager.cpp \
    $$NOTIFICATIONSRCDIR/notificationmanager.cpp \
    $$NOTIFICATIONSRCDIR/lipsticknotification.cpp \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_notificationmanager.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$NOTIFICATIONSRCDIR/lipsticknotification.h \
    $$NOTIFICATIONSRCDIR/notificationmanageradaptor.h \
    $$NOTIFICATIONSRCDIR/categorydefinitionstore.h \
    $$NOTIFICATIONSRCDIR/androidprioritystore.h

