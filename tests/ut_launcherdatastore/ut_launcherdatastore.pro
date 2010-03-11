include(../common_top.pri)
TARGET = ut_launcherdatastore

MODEL_HEADERS += $$SRCDIR/launcherpagemodel.h \
                 $$SRCDIR/launcherbuttonmodel.h

# unit test and unit
SOURCES += \
    ut_launcherdatastore.cpp \
    $$SRCDIR/launcherdatastore.cpp \
    $$SRCDIR/launcherpage.cpp \
    $$SRCDIR/launcherbutton.cpp

# unit test and unit
HEADERS += \
    ut_launcherdatastore.h \
    $$SRCDIR/launcherdatastore.h \
    $$SRCDIR/launcherpagemodel.h \
    $$SRCDIR/launcherpage.h \
    $$SRCDIR/launcherbuttonmodel.h \
    $$SRCDIR/launcherbutton.h

include(../common_bot.pri)
