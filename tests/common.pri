SRCDIR = ../../src
NOTIFICATIONSRCDIR = $$SRCDIR/notifications
SCREENLOCKSRCDIR = $$SRCDIR/screenlock
UTILITYSRCDIR = $$SRCDIR/utilities
VOLUMESRCDIR = $$SRCDIR/volume
XTOOLSRCDIR = $$SRCDIR/xtools
STUBSDIR = ../stubs
COMMONDIR = ../common
INCLUDEPATH += $$SRCDIR $$STUBSDIR
DEPENDPATH = $$INCLUDEPATH
QT += testlib
TEMPLATE = app
DEFINES += UNIT_TEST

QMAKE_CXXFLAGS += \
    -Werror \
    -g \
    -std=c++0x \
    -fPIC \
    -fvisibility=hidden \
    -fvisibility-inlines-hidden \
    -Wno-error=format-security

target.path = /opt/tests/lipstick-tests
INSTALLS += target
