include(../common.pri)
TARGET = ut_notificationfeedbackplayer
equals(QT_MAJOR_VERSION, 4): INCLUDEPATH += $$NOTIFICATIONSRCDIR /usr/include/ngf-qt
equals(QT_MAJOR_VERSION, 5): INCLUDEPATH += $$NOTIFICATIONSRCDIR /usr/include/ngf-qt5
CONFIG += link_pkgconfig
QT += dbus

HEADERS += \
    ut_notificationfeedbackplayer.h \
    $$NOTIFICATIONSRCDIR/notificationfeedbackplayer.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$NOTIFICATIONSRCDIR/notification.h

equals(QT_MAJOR_VERSION, 4): HEADERS += /usr/include/ngf-qt/ngfclient.h
equals(QT_MAJOR_VERSION, 5): HEADERS += /usr/include/ngf-qt5/ngfclient.h

SOURCES += \
    ut_notificationfeedbackplayer.cpp \
    $$NOTIFICATIONSRCDIR/notificationfeedbackplayer.cpp \
    $$NOTIFICATIONSRCDIR/notification.cpp \
    $$STUBSDIR/stubbase.cpp

