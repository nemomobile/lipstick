include(../common_top.pri)
TARGET = ut_statusareawindow

# unit test and unit
SOURCES += \
    ut_statusareawindow.cpp \
    $$SRCDIR/statusareawindow.cpp

# base classes
SOURCES += \
    ../stubs/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_statusareawindow.h \
    $$SRCDIR/statusareawindow.h \

# base classes
HEADERS += \
    ../stubs/stubbase.h

# service classes
HEADERS += \
    $$SRCDIR/statusarea.h \
    $$SRCDIR/homeapplication.h \
    $$SRCDIR/mainwindow.h \
    $$SRCDIR/x11wrapper.h

include(../common_bot.pri)
