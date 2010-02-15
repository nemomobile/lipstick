# This is a project include file for common things to all testsuites/ta_*
# test applications

MOC_DIR = .moc
OBJECTS_DIR = .obj

include(../duiconfig.pri)

TEMPLATE = app
CONFIG += \
    link_pkgconfig \
    dui
PKGCONFIG += \
    mtestapp

target.path=$$TEST_APP_INSTALL_LOCATION

INSTALLS += target

support_files.files =
support_files.path = $$TEST_APP_INSTALL_LOCATION

TEST_THEME_LOCATION = $${TEST_THEME_ROOT_LOCATION}/$${TARGET}

theme_root.files+= $${TARGET}.conf
theme_root.path= $${TEST_THEME_LOCATION}
theme_style.files+= $${TARGET}.css
theme_style.path= $${TEST_THEME_LOCATION}/style
theme_images.path = $$TEST_THEME_LOCATION/images 
theme_images.files = 

INSTALLS += support_files theme_root theme_style theme_images

