TEMPLATE = lib
CONFIG += dll dui
TARGET = duihome
DEPENDPATH += .
INCLUDEPATH += ../include \
    . \

# For setting the coverage flag ON
contains(COV_OPTION, on) { 
    LIBS += -lgcov
    QMAKE_CXXFLAGS += -ftest-coverage \
        -fprofile-arcs
}

include(base.pri)

# Input
HEADERS +=
SOURCES += 
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
