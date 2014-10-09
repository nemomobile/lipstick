TEMPLATE = subdirs
SUBDIRS += src plugin tools tests doc

plugin.depends = src
tools.depends = src
tests.depends = src

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
