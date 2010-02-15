include(../common_top.pri)
TARGET = ut_statusindicatorimageview

STYLE_HEADERS += $$SRCDIR/statusindicatorimagestyle.h
MODEL_HEADERS += $$SRCDIR/statusindicatormodel.h

# unit test and unit
SOURCES += \
    ut_statusindicatorimageview.cpp \
    $$SRCDIR/statusindicatorimageview.cpp \
    $$SRCDIR/statusindicator.cpp

# unit test and unit
HEADERS += \
    ut_statusindicatorimageview.h \
    $$SRCDIR/statusindicatorimageview.h \
    $$SRCDIR/statusindicator.h \
    $$STYLE_HEADERS \
    $$MODEL_HEADERS

include(../common_bot.pri)
