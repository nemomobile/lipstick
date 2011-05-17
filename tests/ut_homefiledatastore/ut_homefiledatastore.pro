include(../common_top.pri)
TARGET = ut_homefiledatastore
INCLUDEPATH += $$MSRCDIR/mashup/appletcommunication $$MSRCDIR/mashup/appletinterface  $$MSRCDIR/mashup/mashup

SOURCES += \
    ut_homefiledatastore.cpp \
    $$SRCDIR/homefiledatastore.cpp

HEADERS += \
    ut_homefiledatastore.h \
    $$SRCDIR/homefiledatastore.h

include(../common_bot.pri)
