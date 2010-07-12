include(../common_top.pri)
TARGET = ut_switcher

MODEL_HEADERS += $$SRCDIR/switchermodel.h \
    $$SRCDIR/switcherbuttonmodel.h

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
    $$SRCDIR/switcher.h

# service classes
HEADERS += \
    $$SRCDIR/switchermodel.h \
    $$SRCDIR/switcherbutton.h \
    $$SRCDIR/switcherbuttonmodel.h \
    $$SRCDIR/windowinfo.h \

include(../common_bot.pri)
