system(qdbusxml2cpp ../../src/components/notificationmanager.xml -p notificationmanagerproxy -c NotificationManagerProxy -i notificationhints.h)

TEMPLATE = app
TARGET = notificationtool

QT += core dbus

INSTALLS = target
target.path = /usr/bin

DEPENDPATH += "../../src"
INCLUDEPATH += "../../src" "../../src/components"
LIBS += -L"../../src" -llipstick

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
