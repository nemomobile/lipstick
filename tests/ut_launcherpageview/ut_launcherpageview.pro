include(../common_top.pri)
TARGET = ut_launcherpageview

STYLE_HEADERS += \
    $$SRCDIR/launcherpagestyle.h

MODEL_HEADERS += \
    $$SRCDIR/launcherpagemodel.h \
    $$SRCDIR/launcherbuttonmodel.h

# unit test and unit
SOURCES += \
    ut_launcherpageview.cpp \
    $$SRCDIR/launcherpageview.cpp \
    $$SRCDIR/launcherpage.cpp


# service classes
SOURCES += ../stubs/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_launcherpageview.h \
    $$SRCDIR/launcherpageview.h \
    $$SRCDIR/launcherpage.h \
    $$SRCDIR/launcherbutton.h \
    $$SRCDIR/launcherbuttonmodel.h \
    $$SRCDIR/launcherpagestyle.h \
    $$SRCDIR/launcherpagemodel.h \
    $$SRCDIR/homeapplication.h \
    $$SRCDIR/mainwindow.h

include(../common_bot.pri)
