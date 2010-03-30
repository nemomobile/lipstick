include(../common_top.pri)
TARGET = ut_switcherview

STYLE_HEADERS += $$SRCDIR/switcherstyle.h
MODEL_HEADERS += $$SRCDIR/switchermodel.h \
                 $$SRCDIR/switcherbuttonmodel.h 

# unit test and unit
SOURCES += \
    ut_switcherview.cpp \
    $$SRCDIR/switcherview.cpp

# unit test and unit
HEADERS += \
    ut_switcherview.h \
    $$SRCDIR/switcherview.h \
    $$SRCDIR/switcher.h \
    $$SRCDIR/switcherstyle.h \
    $$SRCDIR/switcherbutton.h \
    $$SRCDIR/switcherbuttonmodel.h \
    $$SRCDIR/pagedpanning.h \
    $$SRCDIR/switchermodel.h \
    $$SRCDIR/mainwindow.h

# service classes
SOURCES += ../stubs/stubbase.cpp \
    $$SRCDIR/windowinfo.cpp

include(../common_bot.pri)
