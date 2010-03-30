include(../common_top.pri)
TARGET = ut_pagedpanning

# unit test and unit
SOURCES += \
    ut_pagedpanning.cpp \
    $$SRCDIR/pagedpanning.cpp

# unit test and unit
HEADERS += \
    ut_pagedpanning.h \
    $$SRCDIR/pagedpanning.h

include(../common_bot.pri)
