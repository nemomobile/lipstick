include(../common_top.pri)
TARGET = ut_plaindesktopbackgroundextension

EXTDIR = ../../src/extensions/plaindesktopbackgroundextension
INCLUDEPATH += $$EXTDIR
DEFINES += M_LIBRARY=

# unit test and unit
SOURCES += \
    ut_plaindesktopbackgroundextension.cpp \
    $$EXTDIR/plaindesktopbackgroundextension.cpp \
    $$STUBSDIR/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_plaindesktopbackgroundextension.h \
    $$EXTDIR/plaindesktopbackgroundextension.h \
    $$EXTDIR/plaindesktopbackgroundstyle.h \
    $$EXTDIR/plaindesktopbackgroundpixmap.h \
    $$SRCDIR/windowinfo.h

STYLE_HEADERS += $$EXTDIR/plaindesktopbackgroundstyle.h

include(../common_bot.pri)
