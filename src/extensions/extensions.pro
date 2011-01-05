include(../../shared.pri)
TEMPLATE=subdirs

addSubDirs(plaindesktopbackgroundextension)

check.target = check
check.CONFIG = recursive
QMAKE_EXTRA_TARGETS += check

check-xml.target = check-xml
check-xml.CONFIG = recursive
QMAKE_EXTRA_TARGETS += check-xml
