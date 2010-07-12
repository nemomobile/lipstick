include(../common_top.pri)
TARGET = ut_switcherbuttonview

STYLE_HEADERS += $$SRCDIR/switcherbuttonstyle.h
MODEL_HEADERS += $$SRCDIR/switcherbuttonmodel.h

SOURCES += \
    ut_switcherbuttonview.cpp \
    $$SRCDIR/switcherbutton.cpp \
    $$SRCDIR/switcherbuttonview.cpp \
    ../stubs/stubbase.cpp

# unit test and unit classes
HEADERS += \
    ut_switcherbuttonview.h \
    $$SRCDIR/switcherbutton.h \
    $$SRCDIR/switcherbuttonmodel.h \
    $$SRCDIR/switcherbuttonstyle.h \
    $$SRCDIR/switcherbuttonview.h \
    $$SRCDIR/homeapplication.h \
    $$SRCDIR/mainwindow.h \
    $$SRCDIR/switcher.h 

include(../common_bot.pri)
