include(../common_top.pri)
TARGET = ut_launcherbuttonview

STYLE_HEADERS += $$SRCDIR/launcherbuttonstyle.h
MODEL_HEADERS += $$SRCDIR/launcherbuttonmodel.h

SOURCES += \
    ut_launcherbuttonview.cpp

# service classes
SOURCES += \
    $$SRCDIR/launcherbuttonview.cpp \
    $$SRCDIR/launcherbutton.cpp \
   ../stubs/stubbase.cpp

# unit test and unit classes
HEADERS += \
    ut_launcherbuttonview.h \
    $$SRCDIR/launcherbuttonview.h \
    $$SRCDIR/launcherbutton.h \
    $$SRCDIR/launcherbuttonmodel.h \
    $$SRCDIR/launcherbuttonstyle.h \
    ../stubs/stubbase.h

include(../common_bot.pri)
