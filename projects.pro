# ####################################################################
# Main projectfile
# ####################################################################

system(./configure)
include(shared.pri)
include(mconfig.pri)

TEMPLATE = subdirs

addSubDirs(src)
addSubDirs(themes)
addSubDirs(themes)
addSubDirs(tests)

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
    tests/*/*.log.xml \
    tests/*/*.log \
    *.log.xml \
    *.log

include(check.pri)
include(debian/deb.pri)

NODOCS:{
  message("Not building the documentation for this package")
} else {
  addSubDirs(doc)
}
