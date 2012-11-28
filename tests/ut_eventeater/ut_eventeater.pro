include(../common.pri)
TARGET = ut_eventeater

INCLUDEPATH += $$SCREENLOCKSRCDIR $$SRCDIR/xtools

SOURCES += ut_eventeater.cpp \
    $$SCREENLOCKSRCDIR/eventeater.cpp \
    $$STUBSDIR/stubbase.cpp

HEADERS += ut_eventeater.h \
    $$SCREENLOCKSRCDIR/eventeater.h \
    $$SRCDIR/xtools/x11wrapper.h

