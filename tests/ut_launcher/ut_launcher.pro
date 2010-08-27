include(../common_top.pri)
TARGET = ut_launcher

MODEL_HEADERS += $$SRCDIR/launcherbuttonmodel.h \
    $$SRCDIR/launchermodel.h \
    $$SRCDIR/launcherpagemodel.h

SOURCES += \
    ut_launcher.cpp \
    $$SRCDIR/launcher.cpp \
    $$SRCDIR/launcheraction.cpp \
    $$SRCDIR/launcherpage.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp

# unit test and unit classes
HEADERS += \
    ut_launcher.h \
    $$SRCDIR/launcher.h \
    $$SRCDIR/launcheraction.h \
    $$SRCDIR/launcherpage.h \
    $$SRCDIR/launchermodel.h \
    $$SRCDIR/launcherdatastore.h \
    $$SRCDIR/launcherpagemodel.h \
    $$SRCDIR/launcherbuttonmodel.h \
    $$SRCDIR/applicationpackagemonitor.h \
    $$SRCDIR/applicationpackagemonitorlistener.h

# service classes
HEADERS += \
    $$SRCDIR/launcherbutton.h

DEFINES += APPLICATIONS_DIRECTORY=\'$$quote(\"/tmp/ut_launcher/applications/\")\'

include(../common_bot.pri)
