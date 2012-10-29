TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += src plugin tools tests

tools.depends = src

QMAKE_CLEAN += \
    build-stamp \
    configure-stamp \
    artifacts/*.deb \
    *.log.xml \
    *.log

QMAKE_DISTCLEAN += \
    build-stamp \
    configure-stamp \
    *.log.xml \
    *.log
