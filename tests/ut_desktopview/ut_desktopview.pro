include(../common_top.pri)
TARGET = ut_desktopview
INCLUDEPATH += $$SRCDIR/notifications

MODEL_HEADERS += $$SRCDIR/desktopmodel.h
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
    $$SRCDIR/desktopview.h \

# base classes
HEADERS += \

# service classes
SOURCES += ../stubs/stubbase.cpp

# service classes
HEADERS += \
    $$SRCDIR/desktop.h \
    $$SRCDIR/desktopmodel.h \
    $$SRCDIR/desktopstyle.h \
    $$SRCDIR/applicationcontext.h \
    $$SRCDIR/homeapplication.h \
    $$SRCDIR/mainwindow.h \
    $$SRCDIR/switcher.h \
    $$SRCDIR/quicklaunchbar.h \
    $$SRCDIR/launcher.h \
    $$SRCDIR/statusindicator.h \
    $$SRCDIR/appletspace.h

include(../common_bot.pri)
