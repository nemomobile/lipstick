MOC_DIR = .moc
MGEN_OUTDIR = .gen
OBJECTS_DIR = .obj

include(../../mconfig.pri)

TEMPLATE      = lib
CONFIG       += plugin gui meegotouch
INCLUDEPATH  += $$M_INSTALL_HEADERS

MODEL_HEADERS = weathermodel.h \
    weatherbuttonmodel.h
STYLE_HEADERS = weatherbuttonstyle.h

HEADERS = \
    weatherapplet.h \
    weather.h \
    weatherview.h \
    weatherbutton.h \
    weatherbuttonview.h \
    $$MODEL_HEADERS \
    $$STYLE_HEADERS

SOURCES = \
    weatherapplet.cpp \
    weather.cpp \
    weatherview.cpp \
    weatherbutton.cpp \
    weatherbuttonview.cpp

TARGET        = $$qtLibraryTarget(weatherapplet)
DESTDIR       = ../../lib

target.path += $$M_APPLET_DIR

INSTALLS += \
    target \
    desktop_entry \
    settings \
    style \
    view_conf \
    svg \
    images

desktop_entry.path = $$M_APPLET_DATA_DIR
desktop_entry.files = *.desktop
settings.path = $$M_APPLET_SETTINGS_DIR
settings.files = *.xml
style.path = $$M_THEME_DIR/base/meegotouch/weatherappletres/style
style.files = *.css
view_conf.path = $$M_THEME_DIR/base/meegotouch/weatherappletres
view_conf.files = *.conf
svg.path = $$M_THEME_DIR/base/meegotouch/weatherappletres/svg
svg.files = images/*.svg
images.path = $$M_THEME_DIR/base/meegotouch/weatherappletres/images
images.files = images/*.png

include(../check.pri)
