include(../common.pri)
TARGET = ut_lipsticksettings
INCLUDEPATH += $$SCREENLOCKSRCDIR
QT += dbus declarative
CONFIG += link_pkgconfig mobility
PKGCONFIG += x11 mce
MOBILITY += sensors

# unit test and unit
SOURCES += \
    ut_lipsticksettings.cpp \
    $$SRCDIR/lipsticksettings.cpp \
    $$STUBSDIR/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_lipsticksettings.h \
    $$SRCDIR/lipsticksettings.h \
    $$SCREENLOCKSRCDIR/screenlock.h
