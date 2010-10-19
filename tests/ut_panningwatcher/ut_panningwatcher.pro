include(../common_top.pri)
TARGET = ut_panningwatcher

# unit test and unit
SOURCES += \
    ut_panningwatcher.cpp \
    $$SRCDIR/panningwatcher.cpp

# unit test and unit
HEADERS += \
    ut_panningwatcher.h \
    $$SRCDIR/panningwatcher.h \

# service classes
SOURCES += ../stubs/stubbase.cpp

include(../common_bot.pri)
