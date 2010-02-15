include(../common_top.pri)
TARGET = ut_duihomesystemnotificationpluginbase
INCLUDEPATH += $$SRCDIR/notifications

# unit test and unit
SOURCES += \
    ut_duihomesystemnotificationpluginbase.cpp \
    $$SRCDIR/../libduihome/duihomesystemnotificationpluginbase.cpp \
    $$SRCDIR/notifications/notificationsource.cpp \
    $$SRCDIR/notifications/notificationparameters.cpp \
    $$SRCDIR/notifications/notificationparameter.cpp

HEADERS += \
    ut_duihomesystemnotificationpluginbase.h \
    $$SRCDIR/../libduihome/duihomesystemnotificationpluginbase.h \
    $$SRCDIR/notifications/systemnotificationsource.h \
    $$SRCDIR/notifications/notificationparameters.h \
    $$SRCDIR/notifications/notificationparameter.h

include(../common_bot.pri)
