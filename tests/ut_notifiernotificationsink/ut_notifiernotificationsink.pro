include(../common_top.pri)
TARGET = ut_notifiernotificationsink
INCLUDEPATH += $$SRCDIR/notifications

# unit test and unit
SOURCES += \
    ut_notifiernotificationsink.cpp \
    $$SRCDIR/notifications/notifiernotificationsink.cpp

# base classes
SOURCES += \
    $$SRCDIR/notifications/notificationsink.cpp \
    $$SRCDIR/notifications/notification.cpp \
    $$SRCDIR/notifications/notificationparameter.cpp \
    $$SRCDIR/notifications/notificationparameters.cpp

# unit test and unit
HEADERS += \
    ut_notifiernotificationsink.h \
    $$SRCDIR/notifications/notifiernotificationsink.h \

# base classes
HEADERS += \
    $$SRCDIR/notifications/notificationsink.h \
    $$SRCDIR/notifications/notification.h \
    $$SRCDIR/notifications/notificationparameter.h \
    $$SRCDIR/notifications/notificationparameters.h

include(../common_bot.pri)
