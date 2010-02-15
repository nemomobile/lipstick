include(../common_top.pri)
TARGET = ut_notification
INCLUDEPATH += $$SRCDIR/notifications

# unit test and unit
SOURCES += \
    ut_notification.cpp \
    $$SRCDIR/notifications/notification.cpp \
    $$SRCDIR/notifications/notificationparameter.cpp \
    $$SRCDIR/notifications/notificationparameters.cpp

# unit test and unit
HEADERS += \
    ut_notification.h \
    $$SRCDIR/notifications/notification.h \
    $$SRCDIR/notifications/notificationparameter.h \
    $$SRCDIR/notifications/notificationparameters.h

include(../common_bot.pri)
