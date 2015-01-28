include(../common.pri)
TARGET = ut_notificationlistmodel
INCLUDEPATH += $$NOTIFICATIONSRCDIR
INCLUDEPATH += $$UTILITYSRCDIR
INCLUDEPATH += $$3RDPARTYSRCDIR
QT += sql dbus

# unit test and unit
SOURCES += \
    ut_notificationlistmodel.cpp \
    $$NOTIFICATIONSRCDIR/notificationlistmodel.cpp \
    $$NOTIFICATIONSRCDIR/lipsticknotification.cpp \
    $$UTILITYSRCDIR/qobjectlistmodel.cpp \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_notificationlistmodel.h \
    $$NOTIFICATIONSRCDIR/notificationlistmodel.h \
    $$NOTIFICATIONSRCDIR/lipsticknotification.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$UTILITYSRCDIR/qobjectlistmodel.h \
    $$3RDPARTYSRCDIR/synchronizelists.h
