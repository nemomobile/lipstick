include(../common_top.pri)
TARGET = ut_notificationparameter
INCLUDEPATH += $$SRCDIR/notifications

# unit test and unit
SOURCES += \
    ut_notificationparameter.cpp \
    $$SRCDIR/notifications/notificationparameter.cpp

# base classes
SOURCES += \


# unit test and unit
HEADERS += \
    ut_notificationparameter.h \
    $$SRCDIR/notifications/notificationparameter.h \

# base classes
HEADERS += \


# service classes
HEADERS += \


include(../common_bot.pri)
