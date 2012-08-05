
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

TEMPLATE = lib
TARGET = lipstickplugin
VERSION = 0.1

CONFIG += qt plugin
QT += core gui declarative

INSTALLS += target qmldirfile
qmldirfile.files = qmldir
qmldirfile.path += /usr/lib/qt4/imports/org/nemomobile/lipstick
target.path += /usr/lib/qt4/imports/org/nemomobile/lipstick

linux-g++-64 {
    qmldirfile.path += /usr/lib64/qt4/imports/org/nemomobile/lipstick
    target.path += /usr/lib64/qt4/imports/org/nemomobile/lipstick
}

DEPENDPATH += "../src"
INCLUDEPATH += "../src"
LIBS += -L"../src" -llipstick

HEADERS += \
    lipstickplugin.h

SOURCES += \
    lipstickplugin.cpp

OTHER_FILES += \
    qmldir
