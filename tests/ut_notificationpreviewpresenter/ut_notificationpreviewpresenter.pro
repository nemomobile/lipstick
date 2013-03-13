include(../common.pri)
TARGET = ut_notificationpreviewpresenter
INCLUDEPATH += $$SRCDIR $$NOTIFICATIONSRCDIR $$UTILITYSRCDIR /usr/include/qmsystem2
QT += declarative dbus

# unit test and unit
SOURCES += \
    ut_notificationpreviewpresenter.cpp \
    $$NOTIFICATIONSRCDIR/notificationpreviewpresenter.cpp \
    $$NOTIFICATIONSRCDIR/notification.cpp \
    $$STUBSDIR/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_notificationpreviewpresenter.h \
    $$NOTIFICATIONSRCDIR/notificationpreviewpresenter.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$NOTIFICATIONSRCDIR/notification.h \
    $$UTILITYSRCDIR/closeeventeater.h \
    /usr/include/qmsystem2/qmlocks.h \
    /usr/include/qmsystem2/qmdisplaystate.h
