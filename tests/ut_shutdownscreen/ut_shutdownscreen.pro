include(../common.pri)
TARGET = ut_shutdownscreen
INCLUDEPATH += $$SRCDIR $$NOTIFICATIONSRCDIR $$UTILITYSRCDIR $$XTOOLSRCDIR
QT += qml quick dbus

# unit test and unit
SOURCES += \
    $$SRCDIR/shutdownscreen.cpp \
    $$NOTIFICATIONSRCDIR/lipsticknotification.cpp \
    $$STUBSDIR/stubbase.cpp \
    $$STUBSDIR/homewindow.cpp \
    $$STUBSDIR/homeapplication.cpp \
    ut_shutdownscreen.cpp

HEADERS += \
    $$SRCDIR/shutdownscreen.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$NOTIFICATIONSRCDIR/lipsticknotification.h \
    $$UTILITYSRCDIR/closeeventeater.h \
    $$SRCDIR/homeapplication.h \
    $$SRCDIR/homewindow.h \
    ut_shutdownscreen.h
