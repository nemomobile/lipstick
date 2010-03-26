TARGET = fixture_mashupcanvas
include(../common.pri)

TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH += /usr/include

target.path = $$[QT_INSTALL_PLUGINS]/tasfixtures

SOURCES += \
    fixture_mashupcanvas.cpp

HEADERS += \
    fixture_mashupcanvas.h

INSTALLS -= support_files theme_root theme_style theme_images
