MOC_DIR = .moc
M_MGEN_OUTDIR = .gen
OBJECTS_DIR = .obj

include(../../m_targets.pri)
include(../../mconfig.pri)

TEMPLATE = app
TARGET = meegotouchhome
CONFIG += meegotouch

# For setting the coverage flag ON
contains(COV_OPTION, on) {
    LIBS += -lgcov
    QMAKE_CXXFLAGS += -ftest-coverage \
        -fprofile-arcs
}

system(m-servicefwgen -a com.meego.core.HomeScreen)

include(../../localconfig.pri)

QMAKE_STRIP = echo
OBJECTS_DIR = .obj
MOC_DIR = .moc

CONFIG += qt warn_on depend_includepath
CONFIG += qmake_cache target_qt 

#contains(QT_CONFIG, reduce_exports):CONFIG += hide_symbols
#unix:contains(QT_CONFIG, reduce_relocations):CONFIG += bsymbolic_functions

include(../../m_targets.pri)

# Input
HEADERS += homeapplication.h \
    windowinfo.h \
    mainwindow.h \
    x11wrapper.h \
    menumodel.h \
    menuitem.h \
    desktop.h \
    homescreenservice.h \
    homewindowmonitor.h \
    windowmonitor.h \
    xeventlistener.h \
    switchermodel.h \
    windowinfo.h \
    qticonloader.h

SOURCES += main.cpp \
    homeapplication.cpp \
    windowinfo.cpp \
    mainwindow.cpp \
    x11wrapper.cpp \
    menumodel.cpp \
    menuitem.cpp \
    desktop.cpp \
    homescreenservice.cpp \
    homewindowmonitor.cpp \
    xeventlistener.cpp \
    switchermodel.cpp \
    qticonloader.cpp

INSTALL_HEADERS += $$HEADERS \
    mdesktopbackgroundextensioninterface.h

# Input
headers.path += /usr/include/$$TARGET
headers.files += $$INSTALL_HEADERS
target.path += /usr/lib
INSTALLS += target \
    headers

CONFIG += link_pkgconfig
PKGCONFIG += xcomposite mlite

QT += network \
    svg \
    dbus \
    xml \
    declarative

exists($$[QT_INSTALL_LIBS]/libQtOpenGL.so):QT += opengl
DEFINES += APPLICATIONS_DIRECTORY=\'$$quote(\"/usr/share/applications/\")\'
DEFINES += M_XDG_DIR=\\\"\"$$M_XDG_DIR\"\\\"

contains(BENCHMARKS, on) {
    DEFINES += BENCHMARKS_ON
}
QMAKE_CXXFLAGS += \
    -Werror \
    -g
QMAKE_CLEAN += *.gcov \
    ./.obj/*.gcno
QMAKE_EXTRA_TARGETS += check
check.commands = $$system(true)
QMAKE_EXTRA_TARGETS += check-xml
check-xml.commands = $$system(true)
