include(../common.pri)
TARGET = ut_volumecontrol
INCLUDEPATH += $$VOLUMESRCDIR $$UTILITYSRCDIR
CONFIG += link_pkgconfig
PKGCONFIG += dbus-1 dbus-glib-1 libresourceqt5
QT += dbus qml quick

HEADERS += \
    ut_volumecontrol.h \
    $$VOLUMESRCDIR/volumecontrol.h \
    $$VOLUMESRCDIR/volumekeylistener.h \
    $$VOLUMESRCDIR/pulseaudiocontrol.h \
    $$UTILITYSRCDIR/closeeventeater.h \
    $$SRCDIR/homewindow.h \

SOURCES += \
    ut_volumecontrol.cpp \
    $$VOLUMESRCDIR/volumecontrol.cpp \
    $$STUBSDIR/stubbase.cpp \
    $$STUBSDIR/homewindow.cpp \
