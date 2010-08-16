include(../common_top.pri)
TARGET = ut_test
QT += dbus

TEMPLATE += lib

DEFINES += APPLICATIONS_DIRECTORY=\'$$quote(\"/usr/share/applications/\")\'
DEFINES += NOTIFICATIONS_EVENT_TYPES=\'$$quote(\"$$M_NOTIFICATIONS_EVENT_TYPES_DIR\")\'

INCLUDEPATH += $$SRCDIR/notifications

include($$SRCDIR/files.pri)

# unit test and unit classes
SOURCES +=  ut_test.cpp \

# unit test and unit classes
HEADERS += ut_test.h \

include(../common_bot.pri)
