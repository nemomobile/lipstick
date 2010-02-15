MOC_DIR = .moc
DUIGEN_OUTDIR = .gen
OBJECTS_DIR = .obj

include(../../duiconfig.pri)

TEMPLATE      = lib
CONFIG       += plugin gui dui
INCLUDEPATH  += $$DUI_INSTALL_HEADERS

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

target.path += $$DUI_APPLET_DIR

INSTALLS += \
    target \
    desktop_entry \
    settings \
    style \
    view_conf \
    svg \
    images

desktop_entry.path = $$DUI_APPLET_DATA_DIR
desktop_entry.files = *.desktop
settings.path = $$DUI_APPLET_SETTINGS_DIR
settings.files = *.xml
style.path = $$DUI_THEME_DIR/base/dui/weatherappletres/style
style.files = *.css
view_conf.path = $$DUI_THEME_DIR/base/dui/weatherappletres
view_conf.files = *.conf
svg.path = $$DUI_THEME_DIR/base/dui/weatherappletres/svg
svg.files = images/*.svg
images.path = $$DUI_THEME_DIR/base/dui/weatherappletres/images
images.files = images/*.png

include(../check.pri)
