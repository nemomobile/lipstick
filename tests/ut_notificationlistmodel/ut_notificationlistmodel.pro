include(../common.pri)
TARGET = ut_notificationlistmodel
INCLUDEPATH += $$NOTIFICATIONSRCDIR $$SRCDIR/utilities
QT += sql dbus

# unit test and unit
SOURCES += \
    ut_notificationlistmodel.cpp \
    $$NOTIFICATIONSRCDIR/notificationlistmodel.cpp \
    $$NOTIFICATIONSRCDIR/lipsticknotification.cpp \
    $$SRCDIR/utilities/qobjectlistmodel.cpp \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_notificationlistmodel.h \
    $$NOTIFICATIONSRCDIR/notificationlistmodel.h \
    $$NOTIFICATIONSRCDIR/lipsticknotification.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$SRCDIR/utilities/qobjectlistmodel.h
