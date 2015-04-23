include(../common.pri)
TARGET = ut_usbmodeselector
INCLUDEPATH += $$SRCDIR $$NOTIFICATIONSRCDIR $$UTILITYSRCDIR
QT += qml quick dbus

# unit test and unit
SOURCES += \
    $$SRCDIR/usbmodeselector.cpp \
    $$NOTIFICATIONSRCDIR/lipsticknotification.cpp \
    $$STUBSDIR/stubbase.cpp \
    ut_usbmodeselector.cpp

HEADERS += \
    $$SRCDIR/usbmodeselector.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$NOTIFICATIONSRCDIR/lipsticknotification.h \
    $$UTILITYSRCDIR/closeeventeater.h \
    ut_usbmodeselector.h \
    $$SRCDIR/homewindow.h \
