MOC_DIR = .moc
DUIGEN_OUTDIR = .gen
OBJECTS_DIR = .obj

include(../../duiconfig.pri)

TEMPLATE = lib
CONFIG += plugin \
    gui \
    dui
INCLUDEPATH += $$DUI_INSTALL_HEADERS
HEADERS = imageapplet.h \
    imagewidget.h
SOURCES = imageapplet.cpp \
    imagewidget.cpp
TARGET = $$qtLibraryTarget(imageapplet)
DESTDIR = ../../lib
target.path += $$DUI_APPLET_DIR
INSTALLS += target \
    desktop_entry \
    style \
    view_conf \
    svg \
    images

desktop_entry.path = $$DUI_APPLET_DATA_DIR
desktop_entry.files = *.desktop
style.path = $$DUI_THEME_DIR/../imageapplet/themes/style
style.files = *.css
view_conf.path = $$DUI_THEME_DIR/../imageapplet/themes
view_conf.files = *.conf
svg.path = $$DUI_THEME_DIR/../imageapplet/themes/svg
svg.files = images/*.svg
images.path = $$DUI_THEME_DIR/../imageapplet/themes/images
images.files = images/*.png

include(../check.pri)
