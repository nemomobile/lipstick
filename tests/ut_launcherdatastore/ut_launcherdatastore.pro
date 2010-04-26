include(../common_top.pri)
TARGET = ut_launcherdatastore

# unit test and unit
SOURCES += \
    ut_launcherdatastore.cpp \
    $$SRCDIR/launcherdatastore.cpp

# unit test and unit
HEADERS += \
    ut_launcherdatastore.h \
    $$SRCDIR/launcherdatastore.h

DEFINES += APPLICATIONS_DIRECTORY=\'$$quote(\"/tmp/ut_launcherdatastore/applications/\")\'

include(../common_bot.pri)
