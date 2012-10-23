include(../common.pri)
TARGET = ut_notification
INCLUDEPATH += $$NOTIFICATIONSRCDIR
QT += dbus

# unit test and unit
SOURCES += \
    ut_notification.cpp \
    $$NOTIFICATIONSRCDIR/notification.cpp \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_notification.h \
    $$NOTIFICATIONSRCDIR/notification.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    qdbusargument_fake.h

