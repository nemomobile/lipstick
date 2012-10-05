include(../common.pri)
TARGET = ut_notificationmanager
INCLUDEPATH += $$NOTIFICATIONSRCDIR
QT += sql dbus

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

