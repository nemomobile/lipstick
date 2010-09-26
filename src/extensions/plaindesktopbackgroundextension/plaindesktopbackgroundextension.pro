MOC_DIR = .moc
M_MGEN_OUTDIR = .gen
OBJECTS_DIR = .obj

TEMPLATE = lib
CONFIG += plugin \
    gui \
    meegotouch

load(meegotouch_defines)

INCLUDEPATH += ../../libmeegotouchhome
HEADERS = plaindesktopbackgroundextension.h \
          plaindesktopbackgroundpixmap.h \
          plaindesktopbackgroundstyle.h
SOURCES = plaindesktopbackgroundextension.cpp \
          plaindesktopbackgroundpixmap.cpp
STYLE_HEADERS += plaindesktopbackgroundstyle.h

TARGET = $$qtLibraryTarget(meegotouchhome-plaindesktopbackgroundextension)
DESTDIR = ../../../lib
target.path += $$M_APPLICATION_EXTENSION_DIR
INSTALLS += target desktop_entry style

desktop_entry.path = $$M_APPLICATION_EXTENSION_DATA_DIR
desktop_entry.files = *.desktop
style.files = *.css
style.path = /usr/share/themes/base/meegotouch/libmeegotouchhome-plaindesktopbackgroundextension/style
