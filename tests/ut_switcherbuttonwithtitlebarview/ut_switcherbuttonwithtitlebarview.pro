include(../common_top.pri)
TARGET = ut_switcherbuttonwithtitlebarview

STYLE_HEADERS += $$SRCDIR/switcherbuttonstyle.h $$SRCDIR/switcherbuttonwithtitlebarstyle.h
MODEL_HEADERS += $$SRCDIR/switcherbuttonmodel.h

SOURCES += \
    ut_switcherbuttonwithtitlebarview.cpp \
    $$SRCDIR/switcherbutton.cpp \
    $$SRCDIR/switcherbuttonview.cpp \
    $$SRCDIR/switcherbuttonwithtitlebarview.cpp \
    ../stubs/stubbase.cpp

# unit test and unit classes
HEADERS += \
    ut_switcherbuttonwithtitlebarview.h \
    $$SRCDIR/switcherbutton.h \
    $$SRCDIR/switcherbuttonmodel.h \
    $$SRCDIR/switcherbuttonstyle.h \
    $$SRCDIR/switcherbuttonwithtitlebarstyle.h \
    $$SRCDIR/switcherbuttonview.h \
    $$SRCDIR/switcherbuttonwithtitlebarview.h \
    $$SRCDIR/homeapplication.h \
    $$SRCDIR/mainwindow.h \
    $$SRCDIR/switcher.h 

include(../common_bot.pri)
