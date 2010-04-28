include(../common_top.pri)
TARGET = ut_pagepositionindicatorview


STYLE_HEADERS += \
    $$SRCDIR/pagepositionindicatorstyle.h

MODEL_HEADERS += $$SRCDIR/pagepositionindicatormodel.h

# unit test and unit
SOURCES += \
    ut_pagepositionindicatorview.cpp \
    $$SRCDIR/pagepositionindicatorview.cpp

# unit test and unit
HEADERS += \
    ut_pagepositionindicatorview.h \
    $$SRCDIR/pagepositionindicatorview.h

# service classes
HEADERS += \
    $$SRCDIR/pagepositionindicator.h \
    $$SRCDIR/pagepositionindicatormodel.h \
    $$SRCDIR/pagepositionindicatorstyle.h

# service classes
SOURCES += ../stubs/stubbase.cpp \
    $$SRCDIR/pagepositionindicator.cpp

include(../common_bot.pri)
