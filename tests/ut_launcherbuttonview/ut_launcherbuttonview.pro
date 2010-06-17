include(../common_top.pri)
TARGET = ut_launcherbuttonview

# unit test and unit
SOURCES += \
    ut_launcherbuttonview.cpp \
    $$SRCDIR/launcherbuttonview.cpp

# base classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \
    $$SRCDIR/windowinfo.cpp

# unit test and unit
HEADERS += \
    ut_launcherbuttonview.h \
    $$SRCDIR/launcherbuttonview.h

# base classes
HEADERS += \


# service classes
HEADERS += \
    $$SRCDIR/launcheraction.h \
    $$SRCDIR/launcherbutton.h \
    $$SRCDIR/windowinfo.h \
    $$SRCDIR/homeapplication.h

include(../common_bot.pri)
