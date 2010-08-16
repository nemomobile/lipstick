include(../paths.pri)

TEMPLATE = subdirs
INSTALLS += svg

svg.files = *.svg
svg.path = $$THEME_DIR/svg

QMAKE_EXTRA_TARGETS += check
check.commands = $$system(true)
QMAKE_EXTRA_TARGETS += check-xml
check-xml.commands = $$system(true)
