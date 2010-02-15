include(../common_top.pri)
TARGET = ut_statusindicatorlabelview

STYLE_HEADERS += $$SRCDIR/statusindicatorlabelstyle.h
MODEL_HEADERS += $$SRCDIR/statusindicatormodel.h

# unit test and unit
SOURCES += \
    ut_statusindicatorlabelview.cpp \
    $$SRCDIR/statusindicatorlabelview.cpp \
    $$SRCDIR/statusindicator.cpp

# unit test and unit
HEADERS += \
    ut_statusindicatorlabelview.h \
    $$SRCDIR/statusindicatorlabelview.h \
    $$SRCDIR/statusindicator.h \
    $$STYLE_HEADERS \
    $$MODEL_HEADERS

include(../common_bot.pri)
