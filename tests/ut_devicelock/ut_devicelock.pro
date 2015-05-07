include(../common.pri)
TARGET = ut_devicelock

INCLUDEPATH += $$DEVICELOCKSRCDIR ../../src/qmsystem2

QT += dbus

LIBS += -lrt -L $$SRCDIR -llipstick-qt5

# unit test and unit
SOURCES += \
    ut_devicelock.cpp \
    $$DEVICELOCKSRCDIR/devicelock.cpp \
    $$STUBSDIR/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_devicelock.h \
    $$DEVICELOCKSRCDIR/devicelock.h \
    ../../src/qmsystem2/qmlocks.h \
    ../../src/qmsystem2/qmactivity.h \
    ../../src/qmsystem2/qmdisplaystate.h
