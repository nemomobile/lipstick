include(../common_top.pri)
TARGET = ut_duifeedbacknotificationsink
INCLUDEPATH += $$SRCDIR/notifications
DEFINES += NOTIFICATIONS_EVENT_TYPES=\'$$quote(\"$$DUI_NOTIFICATIONS_EVENT_TYPES_DIR\")\'

# unit test and unit
SOURCES += \
    ut_duifeedbacknotificationsink.cpp \
    $$SRCDIR/notifications/duifeedbacknotificationsink.cpp \
    $$SRCDIR/notifications/notification.cpp \
    $$SRCDIR/notifications/notificationgroup.cpp \
    $$SRCDIR/notifications/notificationparameters.cpp \
    $$SRCDIR/notifications/notificationparameter.cpp \
    $$SRCDIR/../libduihome/duihomesystemnotificationpluginbase.cpp \

# base classes
SOURCES += \
    $$SRCDIR/notifications/notificationsink.cpp \
    ../stubs/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_duifeedbacknotificationsink.h \
    $$SRCDIR/notifications/duifeedbacknotificationsink.h \

# base classes
HEADERS += \
    $$SRCDIR/notifications/notificationsink.h \

# service classes
HEADERS += \
    $$SRCDIR/notifications/notification.h \
    $$SRCDIR/notifications/notificationparameters.h \
    $$SRCDIR/notifications/notificationparameter.h \
    $$SRCDIR/notifications/notification.h \
    $$SRCDIR/notifications/notificationgroup.h \
    $$SRCDIR/notifications/notificationmanager.h \
    $$SRCDIR/homeapplication.h \
    $$SRCDIR/../libduihome/duihomesystemnotificationpluginbase.h \

include(../common_bot.pri)
