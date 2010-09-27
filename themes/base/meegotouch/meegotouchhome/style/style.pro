include(../paths.pri)

TEMPLATE = subdirs
INSTALLS += style

style.files = *.css
style.path = $$THEME_DIR/style

QMAKE_EXTRA_TARGETS += check
check.commands = $$system(true)
QMAKE_EXTRA_TARGETS += check-xml
check-xml.commands = $$system(true)
