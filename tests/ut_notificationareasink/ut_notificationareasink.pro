include(../common_top.pri)
TARGET = ut_notificationareasink
INCLUDEPATH += $$SRCDIR/notifications $$SRCDIR
DEFINES += NOTIFICATIONS_EVENT_TYPES=\'$$quote(\"$$DUI_NOTIFICATIONS_EVENT_TYPES_DIR\")\'

# unit test and unit classes
SOURCES += ut_notificationareasink.cpp \
    $$SRCDIR/notifications/notificationareasink.cpp \
    $$SRCDIR/notifications/widgetnotificationsink.cpp \
    $$SRCDIR/notifications/notificationsink.cpp \
    $$SRCDIR/notifications/notification.cpp \
    $$SRCDIR/notifications/notificationgroup.cpp \
    $$SRCDIR/notifications/notificationparameter.cpp \
    $$SRCDIR/notifications/notificationparameters.cpp \
    $$SRCDIR/notifications/dbusinterfacenotificationsource.cpp \
    $$SRCDIR/notifications/dbusinterfacenotificationsourceadaptor.cpp \
    $$SRCDIR/contextframeworkcontext.cpp \
    $$SRCDIR/notifications/notificationsource.cpp

# helper classes
SOURCES += \
    $$STUBSDIR\stubbase.cpp


# unit test and unit classes
HEADERS += ut_notificationareasink.h \
    $$SRCDIR/notifications/notificationareasink.h \
    $$SRCDIR/notifications/widgetnotificationsink.h \
    $$SRCDIR/notifications/notificationsink.h \
    $$SRCDIR/notifications/notification.h \
    $$SRCDIR/notifications/notificationgroup.h \
    $$SRCDIR/notifications/notificationparameter.h \
    $$SRCDIR/notifications/notificationparameters.h \
    $$SRCDIR/homeapplication.h \
    $$SRCDIR/notifications/dbusinterfacenotificationsource.h \
    $$SRCDIR/notifications/dbusinterfacenotificationsourceadaptor.h \
    $$SRCDIR/notifications/eventtypestore.h \
    $$SRCDIR/contextframeworkcontext.h \
    $$SRCDIR/applicationcontext.h \
    $$SRCDIR/notifications/notificationsource.h \
    $$SRCDIR/notifications/notificationsource.h \
    $$SRCDIR/notifications/notificationmanager.h

include(../common_bot.pri)
