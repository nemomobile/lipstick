include(../common_top.pri)
TARGET = ut_mkutclassname

# unit test and unit
SOURCES += \
    ut_mkutclassname.cpp \
    $$SRCDIR/mkutclassname.cpp

# base classes
SOURCES += \
    mkutbasesources

# unit test and unit
HEADERS += \
    ut_mkutclassname.h \
    $$SRCDIR/mkutclassname.h \

# base classes
HEADERS += \
    mkutbaseheaders

# service classes
HEADERS += \
    mkutserviceheaders

include(../common_bot.pri)
