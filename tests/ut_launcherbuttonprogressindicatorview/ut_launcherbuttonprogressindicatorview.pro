include(../common_top.pri)
TARGET = ut_launcherbuttonprogressindicatorview

STYLE_HEADERS += $$SRCDIR/launcherbuttonprogressindicatorstyle.h
MODEL_HEADERS += $$SRCDIR/launcherbuttonprogressindicatormodel.h \
    $$SRCDIR/launcherbuttonmodel.h

# unit test and unit
SOURCES += \
    ut_launcherbuttonprogressindicatorview.cpp \
    $$SRCDIR/launcherbuttonprogressindicatorview.cpp \
    $$SRCDIR/launcherbuttonprogressindicator.cpp \
    $$SRCDIR/launcheraction.cpp \
    $$STUBSDIR/stubbase.cpp

HEADERS += \
    ut_launcherbuttonprogressindicatorview.h \
    $$SRCDIR/launcherbuttonprogressindicatorview.h \
    $$SRCDIR/launcherbuttonprogressindicator.h \
    $$SRCDIR/launcherbuttonprogressindicatormodel.h \
    $$SRCDIR/launcherbuttonprogressindicatorstyle.h \
    $$SRCDIR/launcherbuttonmodel.h \
    $$SRCDIR/launcheraction.h

include(../common_bot.pri)
