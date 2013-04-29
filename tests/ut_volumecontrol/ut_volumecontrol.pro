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
    $$UTILITYSRCDIR/closeeventeater.h

SOURCES += \
    ut_volumecontrol.cpp \
    $$VOLUMESRCDIR/volumecontrol.cpp \
    $$STUBSDIR/stubbase.cpp

equals(QT_MAJOR_VERSION, 4): HEADERS += $$VOLUMESRCDIR/volumekeylistener.h $$XTOOLSRCDIR/x11wrapper.h
