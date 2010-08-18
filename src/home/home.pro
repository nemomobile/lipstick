MOC_DIR = .moc
M_MGEN_OUTDIR = .gen
OBJECTS_DIR = .obj

include(../../m_targets.pri)
include(../../mconfig.pri)

SOURCES += main.cpp

TEMPLATE = app
TARGET = duihome
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
INSTALLS += target
