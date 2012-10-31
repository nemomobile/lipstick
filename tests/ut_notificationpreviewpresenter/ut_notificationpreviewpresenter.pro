include(../common.pri)
TARGET = ut_notificationpreviewpresenter
INCLUDEPATH += $$SRCDIR $$NOTIFICATIONSRCDIR
QT += declarative dbus

# unit test and unit
SOURCES += \
    ut_notificationpreviewpresenter.cpp \
    $$NOTIFICATIONSRCDIR/notificationpreviewpresenter.cpp \
    $$NOTIFICATIONSRCDIR/notification.cpp

# unit test and unit
HEADERS += \
    ut_notificationpreviewpresenter.h \
    $$NOTIFICATIONSRCDIR/notificationpreviewpresenter.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$NOTIFICATIONSRCDIR/notification.h
