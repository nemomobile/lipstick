include(../common_top.pri)
TARGET = ut_desktopview
INCLUDEPATH += $$SRCDIR/notifications

MODEL_HEADERS += $$SRCDIR/desktopmodel.h \
    $$SRCDIR/switchermodel.h
STYLE_HEADERS += $$SRCDIR/desktopstyle.h

# unit test and unit
SOURCES += \
    ut_desktopview.cpp \
    $$SRCDIR/desktopview.cpp

# base classes
SOURCES += \


# unit test and unit
HEADERS += \
    ut_desktopview.h \
    $$SRCDIR/desktopview.h

# base classes
HEADERS += \

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \
    $$SRCDIR/windowinfo.cpp

# service classes
HEADERS += \
    $$SRCDIR/windowinfo.h \
    $$SRCDIR/desktop.h \
    $$SRCDIR/desktopmodel.h \
    $$SRCDIR/switchermodel.h \
    $$SRCDIR/desktopstyle.h \
    $$SRCDIR/mainwindow.h \
    $$SRCDIR/switcher.h \
    $$SRCDIR/quicklaunchbar.h \
    $$SRCDIR/launcher.h \
    $$SRCDIR/launcherdatastore.h \
    $$SRCDIR/applicationpackagemonitor.h \
    $$SRCDIR/applicationpackagemonitorlistener.h \
    $$SRCDIR/windowmonitor.h \
    $$SRCDIR/homewindowmonitor.h

DEFINES += APPLICATIONS_DIRECTORY=\'$$quote(\"/tmp/ut_desktopview/applications/\")\'

include(../common_bot.pri)
