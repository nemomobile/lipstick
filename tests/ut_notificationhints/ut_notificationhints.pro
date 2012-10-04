include(../common.pri)
TARGET = ut_notificationhints
INCLUDEPATH += $$NOTIFICATIONSRCDIR
QT += dbus

# unit test and unit
SOURCES += \
    ut_notificationhints.cpp \
    $$NOTIFICATIONSRCDIR/notificationhints.cpp \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_notificationhints.h \
    $$NOTIFICATIONSRCDIR/notificationhints.h
