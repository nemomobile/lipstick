include(../common_top.pri)
TARGET = ut_homeapplication
INCLUDEPATH += $$SRCDIR/notifications

PKGCONFIG += ContentManagerSearchIf

# unit test and unit
SOURCES += \
    ut_homeapplication.cpp \
    $$SRCDIR/homeapplication.cpp

# helper classes
SOURCES += \
    windowinfo.cpp \
    $$STUBSDIR/stubbase.cpp \
    $$STUBSDIR/testcontextitem.cpp

# unit test and unit
HEADERS += \
    ut_homeapplication.h \
    $$SRCDIR/homeapplication.h \
    $$SRCDIR/notifications/notificationmanager.h \
    $$SRCDIR/notifications/notificationsink.h \
    $$SRCDIR/notifications/homenotificationsink.h \
    $$SRCDIR/notifications/widgetnotificationsink.h \
    $$SRCDIR/notifications/duicompositornotificationsink.h

# helper classes
HEADERS += \
    windowinfo.h \
    $$SRCDIR/mainwindow.h \
    $$SRCDIR/applicationcontext.h \
    $$STUBSDIR/testcontextitem.h \
    $$SRCDIR/contextframeworkcontext.h

SOURCES +=
include(../common_bot.pri)
