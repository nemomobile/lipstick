include(../common.pri)
TARGET = ut_statusindicator
INCLUDEPATH += $$COMPONENTSRCDIR
QT += dbus
CONFIG += qt link_pkgconfig
PKGCONFIG += contextsubscriber-1.0

# unit test and unit
SOURCES += \
    ut_statusindicator.cpp \
    $$COMPONENTSRCDIR/statusindicator.cpp

# unit test and unit
HEADERS += \
    ut_statusindicator.h \
    $$COMPONENTSRCDIR/statusindicator.h
