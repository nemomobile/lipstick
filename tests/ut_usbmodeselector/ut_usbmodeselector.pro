include(../common.pri)
TARGET = ut_usbmodeselector
USBMODEDQTINCLUDEDIR = $$system(pkg-config --cflags-only-I usb-moded-qt5 | sed s/^-I//g)
INCLUDEPATH += $$SRCDIR $$NOTIFICATIONSRCDIR $$UTILITYSRCDIR $$USBMODEDQTINCLUDEDIR ../../src/qmsystem2
PKGCONFIG += usb_moded
QT += qml quick dbus

# unit test and unit
SOURCES += \
    $$SRCDIR/usbmodeselector.cpp \
    $$NOTIFICATIONSRCDIR/lipsticknotification.cpp \
    $$STUBSDIR/stubbase.cpp \
    ut_usbmodeselector.cpp \
    qusbmoded_stub.cpp

HEADERS += \
    $$SRCDIR/usbmodeselector.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$NOTIFICATIONSRCDIR/lipsticknotification.h \
    $$UTILITYSRCDIR/closeeventeater.h \
    $$USBMODEDQTINCLUDEDIR/qusbmoded.h \
    $$USBMODEDQTINCLUDEDIR/qusbmode.h \
    ../../src/qmsystem2/qmlocks.h \
    ut_usbmodeselector.h \
    $$SRCDIR/homewindow.h \
