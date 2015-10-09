include(../common.pri)
TARGET = ut_devicelock

INCLUDEPATH += $$DEVICELOCKSRCDIR

QT += dbus

LIBS += -lrt -L $$SRCDIR -llipstick-qt5

CONFIG += link_pkgconfig
PKGCONFIG += mce keepalive

# unit test and unit
SOURCES += \
    ut_devicelock.cpp \
    $$DEVICELOCKSRCDIR/devicelock.cpp \

# unit test and unit
HEADERS += \
    ut_devicelock.h \
    $$DEVICELOCKSRCDIR/devicelock.h \
