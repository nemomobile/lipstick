include(../common_top.pri)
TARGET = ut_statusindicator

STYLE_HEADERS += $$SRCDIR/statusindicatorimagestyle.h \
    $$SRCDIR/statusindicatorlabelstyle.h
MODEL_HEADERS += $$SRCDIR/statusindicatormodel.h

# unit test and unit classes
SOURCES += ut_statusindicator.cpp \
    $$SRCDIR/statusindicator.cpp \
    $$SRCDIR/statusindicatorimageview.cpp \
    $$SRCDIR/statusindicatorlabelview.cpp

# helper classes
SOURCES +=  \
    $$STUBSDIR/testcontextitem.cpp

# unit test and unit classes
HEADERS += ut_statusindicator.h \
    $$SRCDIR/statusindicator.h \
    $$SRCDIR/applicationcontext.h  \
    $$SRCDIR/statusindicatorimagestyle.h \
    $$SRCDIR/statusindicatorlabelstyle.h \
    $$SRCDIR/statusindicatormodel.h \

# helper classes
HEADERS +=  \
    $$STUBSDIR/testcontextitem.h

# base classes
# HEADERS += \
# service classes
HEADERS += $$SRCDIR/statusindicatorimageview.h \
    $$SRCDIR/statusindicatorlabelview.h
