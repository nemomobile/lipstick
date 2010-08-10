include(../common_top.pri)
TARGET = ut_applicationpackagemonitor

# unit test and unit
SOURCES += \
    ut_applicationpackagemonitor.cpp \
    $$SRCDIR/applicationpackagemonitor.cpp

# base classes
SOURCES += \

# service classes
SOURCES += ../stubs/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_applicationpackagemonitor.h \
    $$SRCDIR/applicationpackagemonitor.h \
    $$SRCDIR/launcherdatastore.h

# base classes
HEADERS += \


# service classes
HEADERS += \

DEFINES += APPLICATIONS_DIRECTORY=\'$$quote(\"/tmp/ut_applicationpackagemonitor/applications/\")\'

include(../common_bot.pri)
