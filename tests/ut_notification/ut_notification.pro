include(../common.pri)
TARGET = ut_notification
INCLUDEPATH += $$NOTIFICATIONSRCDIR
QT += dbus

# unit test and unit
SOURCES += \
    ut_notification.cpp \
    $$NOTIFICATIONSRCDIR/notification.cpp \
    $$NOTIFICATIONSRCDIR/notificationhints.cpp \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_notification.h \
    $$NOTIFICATIONSRCDIR/notification.h \
    $$NOTIFICATIONSRCDIR/notificationhints.h
