include(../common_top.pri)
TARGET = ut_launcherpage

MODEL_HEADERS += $$SRCDIR/launcherpagemodel.h \
                 $$SRCDIR/launcherbuttonmodel.h

# unit test and unit
SOURCES += \
    ut_launcherpage.cpp \
    $$SRCDIR/launcherpage.cpp

# unit test and unit
HEADERS += \
    ut_launcherpage.h \
    $$SRCDIR/launcherpage.h

# service classes
HEADERS += $$SRCDIR/launcherpagemodel.h \
           $$SRCDIR/launcherbuttonmodel.h \
           $$SRCDIR/launcherbutton.h

# service sources
SOURCES += ../stubs/stubbase.cpp


include(../common_bot.pri)
