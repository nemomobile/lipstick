include(../common_top.pri)
TARGET = ut_plaindesktopbackgroundpixmap

EXTDIR = ../../src/extensions/plaindesktopbackgroundextension
INCLUDEPATH += $$EXTDIR

# unit test and unit
SOURCES += \
    ut_plaindesktopbackgroundpixmap.cpp \
    $$EXTDIR/plaindesktopbackgroundpixmap.cpp

# unit test and unit
HEADERS += \
    ut_plaindesktopbackgroundpixmap.h \
    $$EXTDIR/plaindesktopbackgroundpixmap.h

include(../common_bot.pri)
