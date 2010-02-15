include(../common_top.pri)
TARGET = ut_appletspace

MODEL_HEADERS += $$SRCDIR/appletspacemodel.h \

# unit test and unit
SOURCES += \
    ut_appletspace.cpp \
    $$SRCDIR/appletspace.cpp

# base classes
SOURCES += \


# unit test and unit
HEADERS += \
    ut_appletspace.h \
    $$SRCDIR/appletspace.h \
    $$SRCDIR/appletspacemodel.h \

# base classes
HEADERS += \


# service classes
HEADERS += \


include(../common_bot.pri)
