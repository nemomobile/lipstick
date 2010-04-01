HOME_SRC_DIR = .
MOC_DIR = .moc
DUIGEN_OUTDIR = .gen
OBJECTS_DIR = .obj

include(../dui_targets.pri)
include(../duiconfig.pri)
include(files.pri)

SOURCES += main.cpp

TEMPLATE = app
TARGET = duihome
DESTDIR = ../
QT += network \
    svg \
    dbus \
    xml
exists($$[QT_INSTALL_LIBS]/libQtOpenGL.so):QT += opengl
INCLUDEPATH += $$DUI_INSTALL_HEADERS

# For setting the coverage flag ON
contains(COV_OPTION, on) {
LIBS += -lgcov
QMAKE_CXXFLAGS += \
    -ftest-coverage \
    -fprofile-arcs \

}
contains(TESTABILITY, on) {
    DEFINES += TESTABILITY_ON
}

contains(BENCHMARKS, on) {
    DEFINES += BENCHMARKS_ON
}
QMAKE_CXXFLAGS += \
    -Werror \
    -g
CONFIG += link_pkgconfig \
    dui
PKGCONFIG += xcomposite \
    contextsubscriber-1.0 \
    ContentManagerSearchIf

# Support for deprecated DuiValueSpace. Remove this define when new ContextSubscriber is used.
DEFINES += DUIVALUESPACE_USE_DEPRECATED

QMAKE_EXTRA_TARGETS += check
check.commands = $$system(true)
QMAKE_EXTRA_TARGETS += check-xml
check-xml.commands = $$system(true)
QMAKE_CLEAN += *.gcov \
    *.gcno \
    ./.obj/*.gcno
target.path = /usr/bin
INSTALLS += target

DEFINES += APPLET_DATA=\'$$quote(\"$$DUI_APPLET_DATA_DIR\")\'
DEFINES += APPLET_SETTINGS=\'$$quote(\"$$DUI_APPLET_SETTINGS_DIR\")\'
DEFINES += APPLICATIONS_DIRECTORY=\'$$quote(\"/usr/share/applications/\")\'
DEFINES += CATEGORIES_DIRECTORY=\'$$quote(\"/usr/share/desktop-directories\")\'
