include(../common.pri)
TARGET = ut_usbmodeselector
INCLUDEPATH += $$SRCDIR $$NOTIFICATIONSRCDIR $$UTILITYSRCDIR ../../src/qmsystem2
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
    ../../src/qmsystem2/qmlocks.h \
    ../../src/qmsystem2/qmusbmode.h \
    ut_usbmodeselector.h \
    $$SRCDIR/homewindow.h \
