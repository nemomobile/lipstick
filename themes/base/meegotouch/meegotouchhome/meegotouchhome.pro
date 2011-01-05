include(paths.pri)
include(../../../../shared.pri)

TEMPLATE = subdirs
addSubDirs(style)

INSTALLS += conf
conf.files = *.conf
conf.path = $$THEME_DIR

QMAKE_EXTRA_TARGETS += check
check.commands = $$system(true)
QMAKE_EXTRA_TARGETS += check-xml
check-xml.commands = $$system(true)
