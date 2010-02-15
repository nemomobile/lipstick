include(../common_top.pri)
TARGET = ut_eventtypestore
INCLUDEPATH += $$SRCDIR/notifications

# unit test and unit
SOURCES += \
    ut_eventtypestore.cpp \
    $$SRCDIR/notifications/eventtypestore.cpp

# unit test and unit
HEADERS += \
    ut_eventtypestore.h \
    $$SRCDIR/notifications/eventtypestore.h \

include(../common_bot.pri)
