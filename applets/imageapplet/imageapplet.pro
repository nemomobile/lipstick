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
style.path = $$M_THEME_DIR/../imageapplet/themes/style
style.files = *.css
view_conf.path = $$M_THEME_DIR/../imageapplet/themes
view_conf.files = *.conf
svg.path = $$M_THEME_DIR/../imageapplet/themes/svg
svg.files = images/*.svg
images.path = $$M_THEME_DIR/../imageapplet/themes/images
images.files = images/*.png

include(../check.pri)
