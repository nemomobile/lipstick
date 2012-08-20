TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += src plugin example

QMAKE_CLEAN += \
    Makefile \
    */Makefile \
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
