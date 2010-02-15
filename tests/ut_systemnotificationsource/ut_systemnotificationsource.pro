include(../common_top.pri)
TARGET = ut_systemnotificationsource
INCLUDEPATH += $$SRCDIR/notifications

# unit test and unit
SOURCES += \
    ut_systemnotificationsource.cpp \
    $$SRCDIR/notifications/systemnotificationsource.cpp \
    $$SRCDIR/notifications/notificationsource.cpp \
    $$SRCDIR/notifications/notificationparameters.cpp \
    $$SRCDIR/notifications/notificationparameter.cpp

HEADERS += \
    ut_systemnotificationsource.h \
    $$SRCDIR/notifications/notificationmanagerinterface.h \
    $$SRCDIR/notifications/systemnotificationsource.h \
    $$SRCDIR/notifications/notificationparameters.h \
    $$SRCDIR/notifications/notificationparameter.h \
    $$SRCDIR/../libduihome/duihomesystemnotificationpluginbase.h
include(../common_bot.pri)
