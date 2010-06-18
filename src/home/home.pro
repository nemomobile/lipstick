HOME_SRC_DIR = .
MOC_DIR = .moc
MGEN_OUTDIR = .gen
OBJECTS_DIR = .obj

system(m-servicefwgen -a com.meego.core.HomeScreen)

include(../../m_targets.pri)
include(../../mconfig.pri)
include(files.pri)

SOURCES += main.cpp

TEMPLATE = app
TARGET = duihome
DESTDIR = ../../
QT += network \
    svg \
    dbus \
    xml
exists($$[QT_INSTALL_LIBS]/libQtOpenGL.so):QT += opengl
INCLUDEPATH += ../libmeegotouchhome

# For setting the coverage flag ON
contains(COV_OPTION, on) {
LIBS += -lgcov
QMAKE_CXXFLAGS += \
    -ftest-coverage \
    -fprofile-arcs \

}

contains(BENCHMARKS, on) {
    DEFINES += BENCHMARKS_ON
}
QMAKE_CXXFLAGS += \
    -Werror \
    -g
CONFIG += link_pkgconfig \
    meegotouch
PKGCONFIG += xcomposite \
    contextsubscriber-1.0 \
    contentaction-0.1

QMAKE_EXTRA_TARGETS += check
check.commands = $$system(true)
QMAKE_EXTRA_TARGETS += check-xml
check-xml.commands = $$system(true)
QMAKE_CLEAN += *.gcov \
    *.gcno \
    ./.obj/*.gcno
target.path = /usr/bin
INSTALLS += target

DEFINES += APPLET_DATA=\'$$quote(\"$$M_APPLET_DATA_DIR\")\'
DEFINES += APPLET_SETTINGS=\'$$quote(\"$$M_APPLET_SETTINGS_DIR\")\'
DEFINES += APPLICATIONS_DIRECTORY=\'$$quote(\"/usr/share/applications/\")\'
DEFINES += M_XDG_DIR=\\\"\"$$M_XDG_DIR\"\\\"
