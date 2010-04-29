include(../common_top.pri)
TARGET = ut_switcherbutton

MODEL_HEADERS += $$SRCDIR/switcherbuttonmodel.h

SOURCES += \
    ut_switcherbutton.cpp \
    $$SRCDIR/switcherbutton.cpp \
    ../stubs/stubbase.cpp

# unit test and unit classes
HEADERS += \
    ut_switcherbutton.h \
    $$SRCDIR/switcherbutton.h \
    $$SRCDIR/switcherbuttonmodel.h \
    $$SRCDIR/homeapplication.h \
    $$SRCDIR/mainwindow.h

include(../common_bot.pri)
