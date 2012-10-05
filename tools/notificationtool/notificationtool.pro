system(qdbusxml2cpp ../../src/notifications/notificationmanager.xml -p notificationmanagerproxy -c NotificationManagerProxy)

TEMPLATE = app
TARGET = notificationtool

QT += core dbus sql

INSTALLS = target
target.path = /usr/bin

DEPENDPATH += "../../src"
INCLUDEPATH += "../../src" "../../src/notifications"
LIBS = -L"../../src" -llipstick

HEADERS += \
     notificationmanagerproxy.h
SOURCES += \
     notificationtool.cpp \
     notificationmanagerproxy.cpp

QMAKE_CXXFLAGS += \
    -Werror \
    -g \
    -std=c++0x \
    -fvisibility=hidden \
    -fvisibility-inlines-hidden
