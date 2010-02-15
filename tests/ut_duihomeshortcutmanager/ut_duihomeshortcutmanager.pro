include(../common_top.pri)
TARGET = ut_duihomeshortcutmanager

# libqttracker support
LIBS += -lqttracker
DEFINES += ENABLE_QTTRACKER

# unit test and unit classes
SOURCES += \
    ut_duihomeshortcutmanager.cpp \
    $$HOMELIBDIR/duihomeshortcutmanager.cpp

# unit test and unit classes
HEADERS += \
    ut_duihomeshortcutmanager.h \
    $$HOMELIBDIR/duihomeshortcutmanager.h

include(../common_bot.pri)
