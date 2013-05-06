include(../common.pri)
TARGET = ut_usbmodeselector
INCLUDEPATH += $$SRCDIR $$NOTIFICATIONSRCDIR $$UTILITYSRCDIR /usr/include/qmsystem2-qt5
QT += qml quick

# unit test and unit
SOURCES += \
    $$SRCDIR/usbmodeselector.cpp \
    $$STUBSDIR/stubbase.cpp \
    ut_usbmodeselector.cpp

HEADERS += \
    $$SRCDIR/usbmodeselector.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$UTILITYSRCDIR/closeeventeater.h \
    /usr/include/qmsystem2-qt5/qmlocks.h \
    /usr/include/qmsystem2-qt5/qmusbmode.h \
    ut_usbmodeselector.h

