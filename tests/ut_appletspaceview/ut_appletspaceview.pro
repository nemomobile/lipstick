include(../common_top.pri)
TARGET = ut_appletspaceview

STYLE_HEADERS += $$SRCDIR/appletspacestyle.h \

MODEL_HEADERS += $$SRCDIR/appletspacemodel.h \

# unit test and unit
SOURCES += \
    ut_appletspaceview.cpp \
    $$SRCDIR/appletspaceview.cpp \
    $$SRCDIR/appletspace.cpp

# base classes
SOURCES += \


# unit test and unit
HEADERS += \
    ut_appletspaceview.h \
    $$SRCDIR/appletspaceview.h \
    $$SRCDIR/appletspacemodel.h \
    $$SRCDIR/appletspacestyle.h \
    $$SRCDIR/appletspace.h \

# base classes
HEADERS += \


# service classes
HEADERS += \


include(../common_bot.pri)
