include(../common_top.pri)
TARGET = ut_switcher

MODEL_HEADERS += $$SRCDIR/switchermodel.h \
    $$SRCDIR/switcherbuttonmodel.h

STYLE_HEADERS += $$SRCDIR/switcherstyle.h \
                 $$SRCDIR/switcherbasestyle.h

# unit test and unit classes
SOURCES += \
    ut_switcher.cpp \
    $$SRCDIR/switcher.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \
    $$SRCDIR/windowinfo.cpp

# unit test and unit classes
HEADERS += \
    ut_switcher.h \
    $$SRCDIR/switcher.h \
    $$SRCDIR/switcherview.h \
    $$SRCDIR/switcherviewbase.h \
    $$SRCDIR/homewindowmonitor.h \
    $$SRCDIR/windowinfo.h \
    $$SRCDIR/windowmonitor.h

# service classes
HEADERS += \
    $$SRCDIR/switchermodel.h \
    $$SRCDIR/switcherstyle.h \
    $$SRCDIR/switcherbasestyle.h \
    $$SRCDIR/switcherbutton.h \
    $$SRCDIR/switcherbuttonmodel.h \
    $$SRCDIR/windowinfo.h

include(../common_bot.pri)
