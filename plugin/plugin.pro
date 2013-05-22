TEMPLATE = lib
TARGET = lipstickplugin
VERSION = 0.1

CONFIG += qt plugin link_pkgconfig
QT += core gui qml quick compositor
PKGCONFIG += qmsystem2-qt5

INSTALLS = target qmldirfile
qmldirfile.files = qmldir
qmldirfile.path = $$[QT_INSTALL_QML]/org/nemomobile/lipstick
target.path = $$[QT_INSTALL_QML]/org/nemomobile/lipstick

DEPENDPATH += "../src"
INCLUDEPATH += "../src" "../src/utilities" "../src/xtools"
LIBS += -L"../src" -llipstick-qt5

HEADERS += \
    lipstickplugin.h

SOURCES += \
    lipstickplugin.cpp

OTHER_FILES += \
    qmldir

QMAKE_CXXFLAGS += \
    -Werror \
    -g \
    -std=c++0x \
    -fPIC \
    -fvisibility=hidden \
    -fvisibility-inlines-hidden

QMAKE_LFLAGS += \
    -pie \
    -rdynamic
