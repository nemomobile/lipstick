include(../common_top.pri)
TARGET = ut_test
QT += dbus

PKGCONFIG += xcomposite ContentManagerSearchIf

TEMPLATE += lib

DEFINES += APPLET_DATA=\'$$quote(\"$$DUI_APPLET_DATA_DIR\")\'
DEFINES += APPLICATIONS_DIRECTORY=\'$$quote(\"/usr/share/applications/\")\'
DEFINES += CATEGORIES_DIRECTORY=\'$$quote(\"/usr/share/desktop-directories\")\'
DEFINES += NOTIFICATIONS_EVENT_TYPES=\'$$quote(\"$$DUI_NOTIFICATIONS_EVENT_TYPES_DIR\")\'

INCLUDEPATH += $$SRCDIR/notifications
HOME_SRC_DIR = $$SRCDIR

include($$SRCDIR/files.pri)

# unit test and unit classes
SOURCES +=  ut_test.cpp \

# unit test and unit classes
HEADERS += ut_test.h \

include(../common_bot.pri)
