include(../common.pri)
TARGET = ut_thermalnotifier
CONFIG += link_pkgconfig
INCLUDEPATH += $$SRCDIR $$NOTIFICATIONSRCDIR $$UTILITYSRCDIR $$XTOOLSRCDIR /usr/include/qmsystem2-qt5
QT += qml quick dbus

# unit test and unit
SOURCES += \
    $$NOTIFICATIONSRCDIR/thermalnotifier.cpp \
    $$NOTIFICATIONSRCDIR/lipsticknotification.cpp \
    $$STUBSDIR/stubbase.cpp \
    $$STUBSDIR/homeapplication.cpp \
    ut_thermalnotifier.cpp

HEADERS += \
    $$NOTIFICATIONSRCDIR/thermalnotifier.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$NOTIFICATIONSRCDIR/lipsticknotification.h \
    $$SRCDIR/homeapplication.h \
    /usr/include/qmsystem2-qt5/qmthermal.h \
    /usr/include/qmsystem2-qt5/qmdisplaystate.h \
    ut_thermalnotifier.h
