include(../common_top.pri)
TARGET = ut_homenotificationsink
INCLUDEPATH += $$SRCDIR/notifications $$SRCDIR

# unit test and unit classes
SOURCES += \
    ut_homenotificationsink.cpp \
    $$SRCDIR/notifications/homenotificationsink.cpp \
    $$SRCDIR/notifications/widgetnotificationsink.cpp \
    $$SRCDIR/notifications/notificationsink.cpp \
    $$SRCDIR/notifications/notification.cpp \
    $$SRCDIR/notifications/notificationgroup.cpp \
    $$SRCDIR/notifications/notificationparameter.cpp \
    $$SRCDIR/notifications/notificationparameters.cpp \
    ../stubs/stubbase.cpp

# unit test and unit classes
HEADERS += \
    ut_homenotificationsink.h \
    $$SRCDIR/notifications/homenotificationsink.h \
    $$SRCDIR/notifications/widgetnotificationsink.h \
    $$SRCDIR/notifications/notificationsink.h \
    $$SRCDIR/notifications/notification.h \
    $$SRCDIR/notifications/notificationgroup.h \
    $$SRCDIR/notifications/notificationparameter.h \
    $$SRCDIR/notifications/notificationparameters.h \
    $$SRCDIR/notifications/notificationmanager.h \
    $$SRCDIR/homeapplication.h \
    $$SRCDIR/mainwindow.h

include(../common_bot.pri)
