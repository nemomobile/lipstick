include(../common_top.pri)
TARGET = ut_homescreenservice

# unit test and unit
SOURCES += \
    ut_homescreenservice.cpp \
    $$SRCDIR/homescreenservice.cpp

# base classes
#SOURCES += \
#    mkutbasesources

# unit test and unit
HEADERS += \
    ut_homescreenservice.h \
    $$SRCDIR/homescreenservice.h \

# base classes
#HEADERS += \
#    mkutbaseheaders

# service classes
#HEADERS += \
#    mkutserviceheaders

include(../common_bot.pri)
