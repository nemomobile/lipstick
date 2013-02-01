include(../common.pri)
TARGET = ut_usbmodeselector
INCLUDEPATH += $$SRCDIR $$NOTIFICATIONSRCDIR $$UTILITYSRCDIR /usr/include/qmsystem2
QT += declarative

# unit test and unit
SOURCES += \
    $$SRCDIR/usbmodeselector.cpp \
    $$STUBSDIR/stubbase.cpp \
    ut_usbmodeselector.cpp

HEADERS += \
    $$SRCDIR/usbmodeselector.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$UTILITYSRCDIR/closeeventeater.h \
    ut_usbmodeselector.h \
    /usr/include/qmsystem2/qmlocks.h \
    /usr/include/qmsystem2/qmusbmode.h
