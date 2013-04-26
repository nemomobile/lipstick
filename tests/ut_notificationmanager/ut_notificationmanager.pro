include(../common.pri)
TARGET = ut_notificationmanager
INCLUDEPATH += $$NOTIFICATIONSRCDIR
CONFIG += link_pkgconfig
QT += sql dbus
equals(QT_MAJOR_VERSION, 4): PKGCONFIG += mlite
equals(QT_MAJOR_VERSION, 5): PKGCONFIG += mlite5

# unit test and unit
SOURCES += \
    ut_notificationmanager.cpp \
    $$NOTIFICATIONSRCDIR/notificationmanager.cpp \
    $$NOTIFICATIONSRCDIR/notification.cpp \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_notificationmanager.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$NOTIFICATIONSRCDIR/notification.h \
    $$NOTIFICATIONSRCDIR/notificationmanageradaptor.h \
    $$NOTIFICATIONSRCDIR/categorydefinitionstore.h

