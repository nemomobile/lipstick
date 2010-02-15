include(../common_top.pri)
TARGET = ut_notificationmanager
INCLUDEPATH += $$SRCDIR/notifications
DEFINES += NOTIFICATIONS_EVENT_TYPES=\'$$quote(\"$$DUI_NOTIFICATIONS_EVENT_TYPES_DIR\")\'
LIBS += -L../../lib -lduihome
# unit test and unit classes
SOURCES += \
    ut_notificationmanager.cpp \
    $$SRCDIR/notifications/notificationmanager.cpp \
    $$SRCDIR/contextframeworkcontext.cpp \
    $$SRCDIR/notifications/notificationsource.cpp \
    $$SRCDIR/notifications/notification.cpp \
    $$SRCDIR/notifications/notificationgroup.cpp \
    $$SRCDIR/notifications/notificationparameter.cpp \
    $$SRCDIR/notifications/notificationparameters.cpp \
    $$SRCDIR/notifications/eventtypestore.cpp \
    $$SRCDIR/notifications/systemnotificationsource.cpp \
    $$SRCDIR/../libduihome/duihomesystemnotificationpluginbase.cpp

# unit test and unit classes
HEADERS += \
    ut_notificationmanager.h \
    $$SRCDIR/notifications/notificationmanager.h \
    $$SRCDIR/notifications/dbusinterfacenotificationsource.h \
    $$SRCDIR/applicationcontext.h \
    $$SRCDIR/contextframeworkcontext.h \
    $$SRCDIR/notifications/notificationsource.h \
    $$SRCDIR/notifications/notification.h \
    $$SRCDIR/notifications/notificationgroup.h \
    $$SRCDIR/notifications/notificationparameter.h \
    $$SRCDIR/notifications/notificationparameters.h \
    $$SRCDIR/notifications/eventtypestore.h \
    $$SRCDIR/notifications/systemnotificationsource.h \
    $$SRCDIR/../libduihome/duihomesystemnotificationpluginbase.h

include(../common_bot.pri)
