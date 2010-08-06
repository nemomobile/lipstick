include(../common_top.pri)
TARGET = ut_applicationpackagemonitor

# unit test and unit
SOURCES += \
    ut_applicationpackagemonitor.cpp \
    $$SRCDIR/applicationpackagemonitor.cpp

HEADERS += \
    ut_applicationpackagemonitor.h \
    $$SRCDIR/applicationpackagemonitor.h \


include(../common_bot.pri)
