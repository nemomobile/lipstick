include(../common_top.pri)
TARGET = ut_transformlayoutanimation


STYLE_HEADERS += $$SRCDIR/transformlayoutanimationstyle.h

# unit test and unit
SOURCES += \
    ut_transformlayoutanimation.cpp \
    $$SRCDIR/transformlayoutanimation.cpp

# unit test and unit
HEADERS += \
    ut_transformlayoutanimation.h \
    $$SRCDIR/transformlayoutanimation.h \
    $$SRCDIR/transformlayoutanimationstyle.h


# service classes
SOURCES += ../stubs/stubbase.cpp \

include(../common_bot.pri)
