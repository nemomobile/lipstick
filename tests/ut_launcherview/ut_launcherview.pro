include(../common_top.pri)
TARGET = ut_launcherview

PKGCONFIG += ContentManagerSearchIf

STYLE_HEADERS += \
    $$SRCDIR/launcherstyle.h

MODEL_HEADERS += \
    $$SRCDIR/launchermodel.h

# unit test and unit
SOURCES += \
    ut_launcherview.cpp \
    $$SRCDIR/launcherview.cpp \
    $$SRCDIR/launcher.cpp

# service classes
SOURCES += ../stubs/stubbase.cpp \
    $$SRCDIR/windowinfo.cpp


# unit test and unit
HEADERS += \
    ut_launcherview.h \
    $$SRCDIR/launcherview.h \
    $$SRCDIR/launcherstyle.h \
    $$SRCDIR/launchermodel.h \
    $$SRCDIR/launcher.h \
    $$SRCDIR/launcherbutton.h \
    $$SRCDIR/homeapplication.h \
    $$SRCDIR/mainwindow.h

DEFINES += APPLICATIONS_DIRECTORY=\'$$quote(\"/tmp/ut_launcherview/applications/\")\'
DEFINES += CATEGORIES_DIRECTORY=\'$$quote(\"/tmp/ut_launcherview/categories/\")\'

include(../common_bot.pri)
