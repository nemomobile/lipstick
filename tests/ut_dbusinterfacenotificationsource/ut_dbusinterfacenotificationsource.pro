include(../common_top.pri)
TARGET = ut_dbusinterfacenotificationsource
INCLUDEPATH += $$SRCDIR/notifications

# unit test and unit
SOURCES += \
    ut_dbusinterfacenotificationsource.cpp \
    $$SRCDIR/notifications/dbusinterfacenotificationsource.cpp \
    $$SRCDIR/notifications/notificationsource.cpp \
    $$SRCDIR/notifications/notificationparameters.cpp \
    $$SRCDIR/notifications/notificationparameter.cpp

# base classes
SOURCES += \


# unit test and unit
HEADERS += \
    ut_dbusinterfacenotificationsource.h \
    $$SRCDIR/notifications/dbusinterfacenotificationsource.h \
    $$SRCDIR/notifications/dbusinterfacenotificationsourceadaptor.h \
    $$SRCDIR/notifications/notificationparameters.h \
    $$SRCDIR/notifications/notificationparameter.h

# base classes
HEADERS += \


# service classes
HEADERS += \


include(../common_bot.pri)
