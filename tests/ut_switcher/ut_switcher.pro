include(../common_top.pri)
TARGET = ut_switcher

STYLE_HEADERS += $$SRCDIR/switcherstyle.h
MODEL_HEADERS += $$SRCDIR/switchermodel.h

# unit test and unit classes
SOURCES += \
    ut_switcher.cpp \
    $$SRCDIR/switcher.cpp

# service classes
SOURCES += \
    $$SRCDIR/switcherview.cpp \
    $$SRCDIR/windowinfo.cpp

# unit test and unit classes
HEADERS += \
    ut_switcher.h \
    $$SRCDIR/switchermodel.h \
    $$SRCDIR/switcherstyle.h \
    $$SRCDIR/switcher.h

# service classes
HEADERS += \
    $$SRCDIR/switcherview.h \
    $$SRCDIR/switcherbutton.h \
    $$SRCDIR/windowinfo.h

include(../common_bot.pri)
