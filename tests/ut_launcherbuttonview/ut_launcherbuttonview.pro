include(../common_top.pri)
TARGET = ut_launcherbuttonview

STYLE_HEADERS += $$SRCDIR/launcherbuttonstyle.h
MODEL_HEADERS += $$SRCDIR/launcherbuttonmodel.h

# unit test and unit
SOURCES += \
    ut_launcherbuttonview.cpp \
    $$SRCDIR/launcherbuttonview.cpp \
    $$STUBSDIR/stubbase.cpp

HEADERS += \
    ut_launcherbuttonview.h \
    $$SRCDIR/launcherbuttonview.h \
    $$SRCDIR/launcherbuttonmodel.h \
    $$SRCDIR/launcherbuttonstyle.h \
    $$SRCDIR/launcheraction.h \
    $$SRCDIR/launcherbutton.h \
    $$SRCDIR/windowinfo.h \
    $$SRCDIR/launcherbuttonprogressindicator.h

include(../common_bot.pri)
