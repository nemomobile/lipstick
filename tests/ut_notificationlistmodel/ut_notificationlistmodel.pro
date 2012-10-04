include(../common.pri)
TARGET = ut_notificationlistmodel
INCLUDEPATH += $$NOTIFICATIONSRCDIR $$SRCDIR/utilities
QT += sql dbus

# unit test and unit
SOURCES += \
    ut_notificationlistmodel.cpp \
    $$NOTIFICATIONSRCDIR/notificationlistmodel.cpp \
    $$NOTIFICATIONSRCDIR/notification.cpp \
    $$NOTIFICATIONSRCDIR/notificationhints.cpp \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_notificationlistmodel.h \
    $$NOTIFICATIONSRCDIR/notificationlistmodel.h \
    $$NOTIFICATIONSRCDIR/notification.h \
    $$NOTIFICATIONSRCDIR/notificationhints.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$SRCDIR/utilities/qobjectlistmodel.h
