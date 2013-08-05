SRCDIR = ../../src
NOTIFICATIONSRCDIR = $$SRCDIR/notifications
SCREENLOCKSRCDIR = $$SRCDIR/screenlock
UTILITYSRCDIR = $$SRCDIR/utilities
VOLUMESRCDIR = $$SRCDIR/volume
COMPOSITORSRCDIR = $$SRCDIR/compositor
STUBSDIR = ../stubs
COMMONDIR = ../common
INCLUDEPATH += $$SRCDIR $$STUBSDIR
DEPENDPATH = $$INCLUDEPATH
QT += testlib
TEMPLATE = app
DEFINES += UNIT_TEST
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x000000
CONFIG -= link_prl

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
