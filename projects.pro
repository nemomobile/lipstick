# ####################################################################
# Main projectfile
# ####################################################################

include(duiconfig.pri)

CONFIG += ordered
TEMPLATE = subdirs
SUBDIRS = \
    libduihome \
    home \
    themes \
    applets \
    tests \
    translations

QMAKE_CLEAN += \
    build-stamp \
    configure-stamp \
    artifacts/*.deb \
    tests/*/*.log.xml \
    tests/*/*.log \
    *.log.xml \
    *.log

QMAKE_DISTCLEAN += \
    build-stamp \
    configure-stamp \
    artifacts/*.deb \
    tests/*/*.log.xml \
    tests/*/*.log \
    *.log.xml \
    *.log
check.target = check
check.CONFIG = recursive
QMAKE_EXTRA_TARGETS += check
check-xml.target = check-xml
check-xml.CONFIG = recursive
QMAKE_EXTRA_TARGETS += check-xml

include(shared.pri)
include(debian/deb.pri)

include(doc/doc.pri)
