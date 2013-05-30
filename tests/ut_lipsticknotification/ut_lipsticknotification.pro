include(../common.pri)
TARGET = ut_lipsticknotification
INCLUDEPATH += $$NOTIFICATIONSRCDIR
QT += dbus

# unit test and unit
SOURCES += \
    ut_lipsticknotification.cpp \
    $$NOTIFICATIONSRCDIR/lipsticknotification.cpp \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_lipsticknotification.h \
    $$NOTIFICATIONSRCDIR/lipsticknotification.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    qdbusargument_fake.h

