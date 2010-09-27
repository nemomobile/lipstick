include(../common_top.pri)
TARGET = ut_pagedviewport

MODEL_HEADERS += $$SRCDIR/pagepositionindicatormodel.h

# unit test and unit
SOURCES += \
    ut_pagedviewport.cpp \
    $$SRCDIR/pagedviewport.cpp  \
    $$SRCDIR/pagedviewportview.cpp  \
    $$SRCDIR/pagepositionindicator.cpp \
    $$STUBSDIR/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_pagedviewport.h \
    $$SRCDIR/pagedviewport.h \
    $$SRCDIR/pagedviewportstyle.h \
    $$SRCDIR/pagedviewportview.h \
    $$SRCDIR/pagedpanning.h \
    $$SRCDIR/pagepositionindicator.h \
    $$SRCDIR/pagepositionindicatormodel.h \
    $$STUBSDIR/stubbase.h

STYLE_HEADERS += \
    $$SRCDIR/pagedviewportstyle.h

include(../common_bot.pri)
