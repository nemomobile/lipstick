include(../common_top.pri)
TARGET = ut_duicompositornotificationsink
INCLUDEPATH += $$SRCDIR/notifications
DEFINES += NOTIFICATIONS_EVENT_TYPES=\'$$quote(\"$$DUI_NOTIFICATIONS_EVENT_TYPES_DIR\")\'

# unit test and unit classes
SOURCES += \
    ut_duicompositornotificationsink.cpp \
    $$SRCDIR/notifications/duicompositornotificationsink.cpp \
    $$SRCDIR/notifications/widgetnotificationsink.cpp \
    $$SRCDIR/notifications/notificationsink.cpp \
    $$SRCDIR/notifications/notification.cpp \
    $$SRCDIR/notifications/notificationgroup.cpp \
    $$SRCDIR/notifications/notificationparameter.cpp \
    $$SRCDIR/notifications/notificationparameters.cpp \
    ../stubs/stubbase.cpp

# unit test and unit classes
HEADERS += \
    ut_duicompositornotificationsink.h \
    $$SRCDIR/notifications/duicompositornotificationsink.h \
    $$SRCDIR/notifications/widgetnotificationsink.h \
    $$SRCDIR/notifications/notificationsink.h \
    $$SRCDIR/notifications/notification.h \
    $$SRCDIR/notifications/notificationgroup.h \
    $$SRCDIR/notifications/notificationparameter.h \
    $$SRCDIR/notifications/notificationparameters.h \
    $$SRCDIR/notifications/eventtypestore.h \
    $$SRCDIR/homeapplication.h \
    $$SRCDIR/mainwindow.h \
    ../stubs/stubbase.h

include(../common_bot.pri)
