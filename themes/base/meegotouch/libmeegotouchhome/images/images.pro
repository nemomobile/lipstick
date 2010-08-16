include(../paths.pri)

TEMPLATE = subdirs
INSTALLS += images

images.files = *.png
images.path = $$THEME_DIR/images

QMAKE_EXTRA_TARGETS += check
check.commands = $$system(true)
QMAKE_EXTRA_TARGETS += check-xml
check-xml.commands = $$system(true)
