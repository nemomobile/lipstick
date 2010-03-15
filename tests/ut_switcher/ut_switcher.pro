include(../common_top.pri)
TARGET = ut_switcher

STYLE_HEADERS += $$SRCDIR/switcherstyle.h \
                 $$SRCDIR/switcherbuttonstyle.h
MODEL_HEADERS += $$SRCDIR/switchermodel.h \
                 $$SRCDIR/switcherbuttonmodel.h 

# unit test and unit classes
SOURCES += \
    ut_switcher.cpp \
    $$SRCDIR/switcher.cpp \
    $$SRCDIR/switcherphysicsintegrationstrategy.cpp

# service classes
SOURCES += \
    $$SRCDIR/switcherview.cpp \
    $$SRCDIR/windowinfo.cpp \
    $$SRCDIR/mainwindow.cpp \
       ../stubs/stubbase.cpp

# unit test and unit classes
HEADERS += \
    ut_switcher.h \
    $$SRCDIR/switchermodel.h \
    $$SRCDIR/switcherstyle.h \
    $$SRCDIR/switcher.h

# service classes
HEADERS += \
    $$SRCDIR/switcherview.h \
    $$SRCDIR/switcherbutton.h \
    $$SRCDIR/switcherbuttonstyle.h \
    $$SRCDIR/switcherbuttonmodel.h \
    $$SRCDIR/switcherphysicsintegrationstrategy.h \
    $$SRCDIR/windowinfo.h \
    $$SRCDIR/mainwindow.h

include(../common_bot.pri)
