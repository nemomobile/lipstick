MOC_DIR = .moc
M_MGEN_OUTDIR = .gen
OBJECTS_DIR = .obj

include(../../m_targets.pri)
include(../../mconfig.pri)

SOURCES += main.cpp

TEMPLATE = app
TARGET = meegotouchhome
INCLUDEPATH += ../libmeegotouchhome
QMAKE_LIBDIR += ../../lib
LIBS += -lmeegotouchhome

CONFIG += link_pkgconfig \
    meegotouch

QMAKE_EXTRA_TARGETS += check
check.commands = $$system(true)
QMAKE_EXTRA_TARGETS += check-xml
check-xml.commands = $$system(true)
QMAKE_CLEAN += *.gcov \
    *.gcno \
    ./.obj/*.gcno

target.path = /usr/bin

symlink.target = duihome
symlink.depends = $$TARGET
symlink.commands = cd $(INSTALL_ROOT)$$target.path && ln -sf $$symlink.depends $$symlink.target
symlink.path = $$target.path

INSTALLS += target
INSTALLS += symlink
