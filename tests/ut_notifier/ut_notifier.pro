include(../common_top.pri)
TARGET = ut_notifier
INCLUDEPATH += $$SRCDIR/notifications

# unit test and unit
SOURCES += \
    ut_notifier.cpp \
    $$SRCDIR/notifier.cpp

# base classes
SOURCES += \

# helper classes
SOURCES += \
    $$STUBSDIR\stubbase.cpp

# unit test and unit
HEADERS += \
    ut_notifier.h \
    $$SRCDIR/notifier.h \

# base classes
HEADERS += \
    $$SRCDIR/notifications/notificationsink.h \
    $$SRCDIR/notifications/notifiernotificationsink.h \
    $$SRCDIR/homeapplication.h

include(../common_bot.pri)
