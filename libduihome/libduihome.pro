TEMPLATE = lib
CONFIG += dll
TARGET = duihome
DEPENDPATH += .
INCLUDEPATH += ../include \
    . \
LIBS += -ldui

# For setting the coverage flag ON
contains(COV_OPTION, on) { 
    LIBS += -lgcov
    QMAKE_CXXFLAGS += -ftest-coverage \
        -fprofile-arcs
}

# libqttracker support
LIBS += -lqttracker
DEFINES += ENABLE_QTTRACKER
include(base.pri)

# Input
INSTALL_HEADERS += duihomeshortcutmanager.h \
    duihomesystemnotificationpluginbase.h \
    duihomesystemnotificationsourceinterface.h \
    duihomesystemnotificationplugininterface.h
HEADERS += \
    $$INSTALL_HEADERS
SOURCES += duihomeshortcutmanager.cpp \
    duihomesystemnotificationpluginbase.cpp
headers.path += /usr/include/$$TARGET
headers.files += $$INSTALL_HEADERS
target.path += /usr/lib
INSTALLS += target \
    headers
QMAKE_CLEAN += *.gcov \
    ./.obj/*.gcno
QMAKE_EXTRA_TARGETS += check
check.commands = $$system(true)
QMAKE_EXTRA_TARGETS += check-xml
check-xml.commands = $$system(true)
