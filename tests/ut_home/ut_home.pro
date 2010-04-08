include(../common_top.pri)
TARGET = ut_home
INCLUDEPATH += $$SRCDIR/notifications

MODEL_HEADERS += $$SRCDIR/desktopmodel.h

# unit test and unit
SOURCES += \
    ut_home.cpp \
    $$SRCDIR/home.cpp

# base classes
SOURCES += \

# unit test and unit
HEADERS += \
    ut_home.h \
    $$SRCDIR/home.h \

# base classes
HEADERS += \

# service classes
SOURCES += ../stubs/stubbase.cpp

# service classes
HEADERS += \
    $$SRCDIR/desktop.h \
    $$SRCDIR/desktopmodel.h \
    $$SRCDIR/homeapplication.h \

include(../common_bot.pri)
