include(../common_top.pri)
TARGET = ut_statusareaview

STYLE_HEADERS += $$SRCDIR/statusareastyle.h
MODEL_HEADERS += $$SRCDIR/statusareamodel.h

# unit test and unit
SOURCES += \
    ut_statusareaview.cpp \
    $$SRCDIR/statusareaview.cpp \
    $$SRCDIR/statusarea.cpp

# service classes
SOURCES += ../stubs/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_statusareaview.h \
    $$SRCDIR/statusareastyle.h \
    $$SRCDIR/statusareamodel.h \
    $$SRCDIR/statusareaview.h \
    $$SRCDIR/statusarea.h \
    $$SRCDIR/statusindicator.h \
    $$SRCDIR/notifier.h \
    $$SRCDIR/clock.h

include(../common_bot.pri)
