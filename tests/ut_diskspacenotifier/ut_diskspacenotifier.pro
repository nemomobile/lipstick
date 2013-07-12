include(../common.pri)
TARGET = ut_diskspacenotifier
QT += dbus
INCLUDEPATH += $$NOTIFICATIONSRCDIR

# unit test and unit
SOURCES += \
    ut_diskspacenotifier.cpp \
    $$NOTIFICATIONSRCDIR/diskspacenotifier.cpp \
    $$NOTIFICATIONSRCDIR/lipsticknotification.cpp \
    $$STUBSDIR/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_diskspacenotifier.h \
    $$NOTIFICATIONSRCDIR/diskspacenotifier.h \
    $$NOTIFICATIONSRCDIR/lipsticknotification.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$STUBSDIR/stubbase.h

