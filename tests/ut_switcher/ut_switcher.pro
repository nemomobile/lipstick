include(../common_top.pri)
TARGET = ut_switcher

STYLE_HEADERS += $$SRCDIR/switcherstyle.h \
                 $$SRCDIR/switcherbuttonstyle.h \
                 $$SRCDIR/pagepositionindicatorstyle.h
MODEL_HEADERS += $$SRCDIR/switchermodel.h \
                 $$SRCDIR/switcherbuttonmodel.h \
                 $$SRCDIR/pagepositionindicatormodel.h

# unit test and unit classes
SOURCES += \
    ut_switcher.cpp \
    $$SRCDIR/switcher.cpp \
    $$SRCDIR/pagedviewport.cpp \
    $$SRCDIR/pagedpanning.cpp

# service classes
SOURCES += \
    $$SRCDIR/switcherview.cpp \
    $$SRCDIR/windowinfo.cpp \
    $$SRCDIR/mainwindow.cpp \
       ../stubs/stubbase.cpp \
    $$SRCDIR/pagepositionindicatorview.cpp \
    $$SRCDIR/pagepositionindicator.cpp

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
    $$SRCDIR/pagedviewport.h \
    $$SRCDIR/pagedpanning.h \
    $$SRCDIR/windowinfo.h \
    $$SRCDIR/mainwindow.h \
    $$SRCDIR/pagepositionindicatorview.h \
    $$SRCDIR/pagepositionindicatorstyle.h \
    $$SRCDIR/pagepositionindicator.h \
    $$SRCDIR/pagepositionindicatormodel.h

include(../common_bot.pri)
