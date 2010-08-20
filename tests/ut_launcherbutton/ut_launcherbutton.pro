include(../common_top.pri)
TARGET = ut_launcherbutton

MODEL_HEADERS += $$SRCDIR/launcherbuttonmodel.h

# unit test and unit
SOURCES += \
    ut_launcherbutton.cpp \
    $$SRCDIR/launcherbutton.cpp \
    $$SRCDIR/windowinfo.cpp

# base classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_launcherbutton.h \
    $$SRCDIR/launcherbutton.h \
    $$SRCDIR/launcherbuttonmodel.h \
    $$SRCDIR/launcher.h \
    $$SRCDIR/homewindowmonitor.h \
    $$SRCDIR/windowinfo.h \
    $$SRCDIR/x11wrapper.h \
    $$SRCDIR/windowmonitor.h

include(../common_bot.pri)
