include(../common_top.pri)
TARGET = ut_notificationparameters
INCLUDEPATH += $$SRCDIR/notifications

# unit test and unit
SOURCES += \
    ut_notificationparameters.cpp \
    $$SRCDIR/notifications/notificationparameters.cpp \
    $$SRCDIR/notifications/notificationparameter.cpp

# base classes
SOURCES += \


# unit test and unit
HEADERS += \
    ut_notificationparameters.h \
    $$SRCDIR/notifications/notificationparameters.h \
    $$SRCDIR/notifications/notificationparameter.h

# base classes
HEADERS += \


# service classes
HEADERS += \


include(../common_bot.pri)
