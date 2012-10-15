include(../common.pri)
TARGET = ut_batterystatusindicator
INCLUDEPATH += $$COMPONENTSRCDIR
QT += dbus
CONFIG += qt link_pkgconfig
PKGCONFIG += contextsubscriber-1.0

# unit test and unit
SOURCES += \
    ut_batterystatusindicator.cpp \
    $$COMPONENTSRCDIR/batterystatusindicator.cpp \
    $$COMPONENTSRCDIR/statusindicator.cpp

# unit test and unit
HEADERS += \
    ut_batterystatusindicator.h \
    $$COMPONENTSRCDIR/batterystatusindicator.h \
    $$COMPONENTSRCDIR/statusindicator.h
