include(../common.pri)
TARGET = ut_lipstickdbusinterface
QT += dbus

# unit test and unit
SOURCES += \
    ut_lipstickdbusinterface.cpp \
    $$SRCDIR/lipstickdbusinterface.cpp

# unit test and unit
HEADERS += \
    ut_lipstickdbusinterface.h \
    $$SRCDIR/lipstickdbusinterface.h
