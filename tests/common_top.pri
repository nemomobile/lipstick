include(../check.pri)
include(../../mconfig.pri)
SRCDIR = ../../src/libmeegotouchhome
STUBSDIR = ../stubs
INCLUDEPATH += $$SRCDIR $$HOMELIBDIR $$STUBSDIR $$M_INSTALL_HEADERS
DEPENDPATH = $$INCLUDEPATH
CONFIG += debug meegotouch link_pkgconfig
PKGCONFIG += contextsubscriber-1.0 \
    contentaction-0.1 \
    xcomposite

QT += testlib network gui dbus xml
exists($$[QT_INSTALL_LIBS]/libQtOpenGL.so):QT += opengl
TEMPLATE = app
DEFINES += UNIT_TEST
LIBS += -L../../lib

# For setting the coverage flag ON
contains(COV_OPTION, on) {
LIBS += -lgcov
QMAKE_CXXFLAGS += -ftest-coverage \
    -fprofile-arcs
}

DEFINES += M_XDG_DIR=\\\"\"$$M_XDG_DIR\"\\\"

# install tests
target.path = $$[QT_INSTALL_LIBS]/meegotouchhome-tests
INSTALLS += target
