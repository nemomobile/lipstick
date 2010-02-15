include(../common_top.pri)
TARGET = ut_notificationarea
INCLUDEPATH += $$SRCDIR/notifications

STYLE_HEADERS += $$SRCDIR/notificationareastyle.h
MODEL_HEADERS += $$SRCDIR/notificationareamodel.h

# unit test and unit
SOURCES += \
    ut_notificationarea.cpp \
    $$SRCDIR/notificationarea.cpp

# base classes
SOURCES += \

# service classes
SOURCES += \
    $$SRCDIR/notificationareaview.cpp \
    $$STUBSDIR/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_notificationarea.h \
    $$SRCDIR/notificationarea.h \
    $$SRCDIR/notificationareamodel.h

# base classes
HEADERS += \


# service classes
HEADERS += \
    $$SRCDIR/notifications/notificationsink.h \
    $$SRCDIR/notifications/widgetnotificationsink.h \
    $$SRCDIR/notifications/notificationareasink.h \
    $$SRCDIR/notificationareaview.h \
    $$SRCDIR/notificationareastyle.h \
    $$SRCDIR/homeapplication.h

include(../common_bot.pri)
