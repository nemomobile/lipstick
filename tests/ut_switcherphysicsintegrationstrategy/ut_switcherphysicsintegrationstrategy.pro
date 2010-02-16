include(../common_top.pri)
TARGET = ut_switcherphysicsintegrationstrategy

# unit test and unit
SOURCES += \
    ut_switcherphysicsintegrationstrategy.cpp \
    $$SRCDIR/switcherphysicsintegrationstrategy.cpp

# base classes
#SOURCES += \
#    mkutbasesources

# unit test and unit
HEADERS += \
    ut_switcherphysicsintegrationstrategy.h \
    $$SRCDIR/switcherphysicsintegrationstrategy.h

# base classes
#HEADERS += \
#    mkutbaseheaders

# service classes
#HEADERS += \
#    mkutserviceheaders

include(../common_bot.pri)
