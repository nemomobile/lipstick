include(../common_top.pri)
TARGET = ut_quicklaunchbarview

STYLE_HEADERS += $$SRCDIR/quicklaunchbarstyle.h
MODEL_HEADERS += $$SRCDIR/quicklaunchbarmodel.h \
    $$SRCDIR/launcherbuttonmodel.h

# unit test and unit
SOURCES += \
    ut_quicklaunchbarview.cpp \
    $$SRCDIR/quicklaunchbarview.cpp \
    $$SRCDIR/quicklaunchbar.cpp

# base classes
SOURCES += ../stubs/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_quicklaunchbarview.h \
    $$SRCDIR/quicklaunchbarview.h \

# service classes
HEADERS += \
    $$SRCDIR/quicklaunchbarmodel.h \
    $$SRCDIR/quicklaunchbarstyle.h \
    $$SRCDIR/quicklaunchbar.h \
    $$SRCDIR/launcherbutton.h \
    $$SRCDIR/launcherdatastore.h \
    $$SRCDIR/windowinfo.h \
    $$SRCDIR/launcherbuttonmodel.h

DEFINES += APPLICATIONS_DIRECTORY=\'$$quote(\"/tmp/ut_launcher/applications/\")\'

include(../common_bot.pri)
