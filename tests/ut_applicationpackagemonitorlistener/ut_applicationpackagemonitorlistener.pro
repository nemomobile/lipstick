include(../common_top.pri)
TARGET = ut_applicationpackagemonitorlistener

SOURCES += \
    ut_applicationpackagemonitorlistener.cpp \
    $$SRCDIR/applicationpackagemonitorlistener.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp

# unit test and unit classes
HEADERS += \
    ut_applicationpackagemonitorlistener.h \
    $$SRCDIR/applicationpackagemonitorlistener.h \
    $$SRCDIR/applicationpackagemonitor.h


# service classes
HEADERS += \

DEFINES += APPLICATIONS_DIRECTORY=\'$$quote(\"/tmp/ut_desktopview/applications/\")\'

include(../common_bot.pri)
