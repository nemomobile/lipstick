include(../common.pri)
TARGET = ut_volumecontrol
INCLUDEPATH += $$VOLUMESRCDIR $$UTILITYSRCDIR $$XTOOLSRCDIR
CONFIG += link_pkgconfig
PKGCONFIG += dbus-1 dbus-glib-1 libresourceqt1
QT += dbus declarative

HEADERS += \
    ut_volumecontrol.h \
    $$VOLUMESRCDIR/volumecontrol.h \
    $$VOLUMESRCDIR/pulseaudiocontrol.h \
    $$VOLUMESRCDIR/volumekeylistener.h \
    $$UTILITYSRCDIR/closeeventeater.h \
    $$XTOOLSRCDIR/x11wrapper.h \
    $$XTOOLSRCDIR/x11eventlistener.h

SOURCES += \
    ut_volumecontrol.cpp \
    $$VOLUMESRCDIR/volumecontrol.cpp \
    $$STUBSDIR/stubbase.cpp

