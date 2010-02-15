include(../common_top.pri)
TARGET = ut_statusarea

STYLE_HEADERS += $$SRCDIR/statusareastyle.h
MODEL_HEADERS += \
    $$SRCDIR/statusareamodel.h

# unit test and unit classes
SOURCES += \
    ut_statusarea.cpp \
    $$SRCDIR/statusarea.cpp

# unit test and unit classes
HEADERS += \
    ut_statusarea.h \
    $$SRCDIR/statusarea.h \
    $$SRCDIR/statusareastyle.h \
    $$SRCDIR/statusareamodel.h \
    $$SRCDIR/applicationcontext.h \
    $$SRCDIR/contextframeworkcontext.h

include(../common_bot.pri)
