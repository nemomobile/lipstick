MOC_DIR = .moc
MGEN_OUTDIR = .gen
OBJECTS_DIR = .obj

include(../../mconfig.pri)

TEMPLATE = lib
CONFIG += plugin \
    gui \
    meegotouch
INCLUDEPATH += $$M_INSTALL_HEADERS
HEADERS = imageapplet.h \
    imagewidget.h
SOURCES = imageapplet.cpp \
    imagewidget.cpp
TARGET = $$qtLibraryTarget(imageapplet)
DESTDIR = ../../lib
target.path += $$M_APPLET_DIR
INSTALLS += target \
    desktop_entry \
    style \
    view_conf \
    svg \
    images

desktop_entry.path = $$M_APPLET_DATA_DIR
desktop_entry.files = *.desktop
style.path = $$M_THEME_DIR/base/meegotouch/imageapplet/style
style.files = *.css
view_conf.path = $$M_THEME_DIR/base/meegotouch/imageapplet
view_conf.files = *.conf
svg.path = $$M_THEME_DIR/base/meegotouch/imageapplet/svg
svg.files = images/*.svg
images.path = $$M_THEME_DIR/base/meegotouch/imageapplet/images
images.files = images/*.png

include(../check.pri)
