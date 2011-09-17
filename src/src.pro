VERSION = 0.0.5
MOC_DIR = .moc
M_MGEN_OUTDIR = .gen
OBJECTS_DIR = .obj

TEMPLATE = app
TARGET = lipstick

# For setting the coverage flag ON
contains(COV_OPTION, on) {
    LIBS += -lgcov
    QMAKE_CXXFLAGS += -ftest-coverage \
        -fprofile-arcs
}

system(m-servicefwgen -a com.meego.core.HomeScreen)

QMAKE_STRIP = echo
OBJECTS_DIR = .obj
MOC_DIR = .moc

CONFIG += qt warn_on depend_includepath
CONFIG += qmake_cache target_qt 

#contains(QT_CONFIG, reduce_exports):CONFIG += hide_symbols
#unix:contains(QT_CONFIG, reduce_relocations):CONFIG += bsymbolic_functions

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
    windowpixmapprovider.h \
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
    windowpixmapprovider.cpp \
    qticonloader.cpp

INSTALL_HEADERS += $$HEADERS \
    mdesktopbackgroundextensioninterface.h

# Input
target.path += /usr/bin
INSTALLS += target

CONFIG += link_pkgconfig
PKGCONFIG += xcomposite mlite xdamage

packagesExist(contentaction-0.1) {
    message("Using contentaction to launch applications")
    PKGCONFIG += contentaction-0.1
    DEFINES += HAS_CONTENTACTION
    HEADERS += launcheraction.h
    SOURCES += launcheraction.cpp

} else {
    warning("contentaction doesn't exist; falling back to exec - this may not work so great")
}

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
