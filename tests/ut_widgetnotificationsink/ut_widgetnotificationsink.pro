include(../common_top.pri)
TARGET = ut_widgetnotificationsink
INCLUDEPATH += $$SRCDIR/notifications
DEFINES += NOTIFICATIONS_EVENT_TYPES=\'$$quote(\"$$DUI_NOTIFICATIONS_EVENT_TYPES_DIR\")\'

# unit test and unit
SOURCES += \
    ut_widgetnotificationsink.cpp \
    $$SRCDIR/notifications/widgetnotificationsink.cpp \
    $$SRCDIR/notifications/notificationsink.cpp \
    $$SRCDIR/notifications/notificationparameters.cpp \
    $$SRCDIR/notifications/notificationparameter.cpp \
    $$SRCDIR/notifications/notification.cpp \
    $$SRCDIR/notifications/notificationgroup.cpp

# helper classes
SOURCES += \
    $$STUBSDIR\stubbase.cpp

# unit test and unit
HEADERS += \
    ut_widgetnotificationsink.h \
    $$SRCDIR/notifications/widgetnotificationsink.h \
    $$SRCDIR/notifications/notificationsink.h \
    $$SRCDIR/notifications/notification.h \
    $$SRCDIR/notifications/notificationgroup.h \
    $$SRCDIR/notifications/notificationparameters.h \
    $$SRCDIR/notifications/notificationparameter.h \
    $$SRCDIR/notifications/notificationmanager.h \
    $$SRCDIR/notifications/eventtypestore.h \
    $$SRCDIR/homeapplication.h

include(../common_bot.pri)
