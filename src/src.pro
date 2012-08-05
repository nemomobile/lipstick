
# This file is part of lipstick, a QML desktop library
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License version 2.1 as published by the Free Software Foundation
# and appearing in the file LICENSE.LGPL included in the packaging
# of this file.
#
# This code is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# Copyright (c) 2011, Robin Burchell
# Copyright (c) 2012, Timur Kristóf <venemo@fedoraproject.org>

TEMPLATE = app
TARGET = lipstick
VERSION = 0.1

target.path += /usr/bin
INSTALLS += target


QT += network \
    svg \
    dbus \
    xml \
    declarative \
    opengl

system(m-servicefwgen -a com.meego.core.HomeScreen)

# Input
HEADERS += homeapplication.h \
    utilities/qobjectlistmodel.h \
    xtools/homewindowmonitor.h \
    xtools/windowmonitor.h \
    xtools/xeventlistener.h \
    xtools/xatomcache.h \
    xtools/xwindowmanager.h \
    components/windowinfo.h \
    components/launcheritem.h \
    components/launchermodel.h \
    components/switchermodel.h \
    components/switcherpixmapitem.h \
    components/statusbar.h \
    components/windowmanager.h

SOURCES += main.cpp \
    homeapplication.cpp \
    utilities/qobjectlistmodel.cpp \
    xtools/homewindowmonitor.cpp \
    xtools/xeventlistener.cpp \
    xtools/xatomcache.cpp \
    xtools/xwindowmanager.cpp \
    components/windowinfo.cpp \
    components/launcheritem.cpp \
    components/launchermodel.cpp \
    components/switchermodel.cpp \
    components/switcherpixmapitem.cpp \
    components/statusbar.cpp \
    components/windowmanager.cpp

RESOURCES += \
    res.qrc

OTHER_FILES += \
    qml/main.qml \
    qml/Launcher.qml \
    qml/Switcher.qml \
    qml/SwitcherItem.qml

CONFIG += link_pkgconfig mobility qt warn_on depend_includepath qmake_cache target_qt
MOBILITY += sensors
PKGCONFIG += xcomposite mlite xdamage x11

packagesExist(contentaction-0.1) {
    message("Using contentaction to launch applications")
    PKGCONFIG += contentaction-0.1
    DEFINES += HAS_CONTENTACTION
}
else {
    warning("contentaction doesn't exist; falling back to exec - this may not work so great")
}

MOC_DIR = .moc
M_MGEN_OUTDIR = .gen
OBJECTS_DIR = .obj
QMAKE_STRIP = echo

QMAKE_CXXFLAGS += \
    -Werror \
    -g \
    -std=c++0x

QMAKE_CLEAN += \
    *.gcov \
    ./.obj/*.gcno
