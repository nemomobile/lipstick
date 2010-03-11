include(../common_top.pri)
TARGET = ut_pagedviewport

# unit test and unit
SOURCES += \
    ut_pagedviewport.cpp \
    $$SRCDIR/pagedviewport.cpp 
    
# unit test and unit
HEADERS += \
    ut_pagedviewport.h \
    $$SRCDIR/pagedviewport.h \
    $$SRCDIR/switcherphysicsintegrationstrategy.h 

include(../common_bot.pri)
