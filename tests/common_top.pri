include(../check.pri)
include(../../duiconfig.pri)
SRCDIR = ../../home
HOMELIBDIR = ../../libduihome
STUBSDIR = ../stubs
INCLUDEPATH += $$SRCDIR $$HOMELIBDIR $$STUBSDIR $$DUI_INSTALL_HEADERS
DEPENDPATH = $$INCLUDEPATH
CONFIG += debug dui link_pkgconfig
PKGCONFIG += contextsubscriber-1.0 \
    ContentManagerSearchIf
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

# Support for deprecated DuiValueSpace. Remove this define when new ContextSubscriber is used.
DEFINES += DUIVALUESPACE_USE_DEPRECATED

# install tests
target.path = $$[QT_INSTALL_LIBS]/duihomescreen-tests
INSTALLS += target
