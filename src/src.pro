VERSION = 0.2.1
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
    desktop.h \
    homescreenservice.h \
    homewindowmonitor.h \
    windowmonitor.h \
    xeventlistener.h \
    qticonloader.h \
    components/menumodel.h \
    components/menuitem.h \
    components/switchermodel.h \
    components/switcherpixmapitem.h \
    components/statusbar.h

SOURCES += main.cpp \
    homeapplication.cpp \
    windowinfo.cpp \
    mainwindow.cpp \
    desktop.cpp \
    homescreenservice.cpp \
    homewindowmonitor.cpp \
    xeventlistener.cpp \
    qticonloader.cpp \
    components/menumodel.cpp \
    components/menuitem.cpp \
    components/switchermodel.cpp \
    components/switcherpixmapitem.cpp \
    components/statusbar.cpp

RESOURCES += \
    res.qrc

OTHER_FILES += \
    qml/main.qml \
    qml/Launcher.qml \
    qml/Switcher.qml \
    qml/SwitcherItem.qml


# Input
target.path += /usr/bin
INSTALLS += target

CONFIG += link_pkgconfig
PKGCONFIG += xcomposite mlite xdamage x11

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
    declarative \
    opengl

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


