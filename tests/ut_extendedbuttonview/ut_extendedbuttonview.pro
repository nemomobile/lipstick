include(../common_top.pri)
TARGET = ut_extendedbuttonview

STYLE_HEADERS += $$SRCDIR/extendedbuttonstyle.h

# unit test and unit
SOURCES += \
    ut_extendedbuttonview.cpp \
    $$SRCDIR/extendedbuttonview.cpp

# unit test and unit
HEADERS += \
    ut_extendedbuttonview.h \
    $$SRCDIR/extendedbuttonview.h \
    $$SRCDIR/extendedbuttonstyle.h \

include(../common_bot.pri)
