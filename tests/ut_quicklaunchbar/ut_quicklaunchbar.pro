include(../common_top.pri)
TARGET = ut_quicklaunchbar

MODEL_HEADERS += $$SRCDIR/quicklaunchbarmodel.h \
    $$SRCDIR/launcherbuttonmodel.h

# unit test and unit
SOURCES += \
    ut_quicklaunchbar.cpp \
    $$SRCDIR/quicklaunchbar.cpp

# base classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_quicklaunchbar.h \
    $$SRCDIR/quicklaunchbar.h \
    $$SRCDIR/applicationpackagemonitor.h \
    $$SRCDIR/applicationpackagemonitorlistener.h

# service classes
HEADERS += \
    $$SRCDIR/quicklaunchbarmodel.h \
    $$SRCDIR/launcherbutton.h \
    $$SRCDIR/launcherdatastore.h \
    $$SRCDIR/windowinfo.h \
    $$SRCDIR/launcherbuttonmodel.h

DEFINES += APPLICATIONS_DIRECTORY=\'$$quote(\"/tmp/ut_launcher/applications/\")\'

include(../common_bot.pri)
