include(../common.pri)
TARGET = ut_volumecontrol
INCLUDEPATH += $$VOLUMESRCDIR $$UTILITYSRCDIR
CONFIG += link_pkgconfig
PKGCONFIG += dbus-1 dbus-glib-1 libresourceqt5
QT += dbus qml quick
QMAKE_CXXFLAGS += `pkg-config --cflags-only-I mlite5 qmsystem2-qt5`

HEADERS += \
    ut_volumecontrol.h \
    $$VOLUMESRCDIR/volumecontrol.h \
    $$VOLUMESRCDIR/pulseaudiocontrol.h \
    $$UTILITYSRCDIR/closeeventeater.h \
    $$SRCDIR/homewindow.h \
    /usr/include/mlite5/mgconfitem.h \

SOURCES += \
    ut_volumecontrol.cpp \
    $$VOLUMESRCDIR/volumecontrol.cpp \
    $$STUBSDIR/stubbase.cpp \
    $$STUBSDIR/homewindow.cpp \
