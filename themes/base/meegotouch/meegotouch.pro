include(../../../shared.pri)
TEMPLATE = subdirs
addSubDirs(libmeegotouchhome)
addSubDirs(meegotouchhome)

QMAKE_EXTRA_TARGETS += check
check.commands = $$system(true)
QMAKE_EXTRA_TARGETS += check-xml
check-xml.commands = $$system(true)
