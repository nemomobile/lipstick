system(qdbusxml2cpp ../../src/notifications/notificationmanager.xml -p notificationmanagerproxy -c NotificationManagerProxy -i notification.h)

TEMPLATE = app
TARGET = notificationtool

QT += core dbus
CONFIG += link_pkgconfig
equals(QT_MAJOR_VERSION, 4): PKGCONFIG += mlite
equals(QT_MAJOR_VERSION, 5): PKGCONFIG += mlite5

INSTALLS = target
target.path = /usr/bin

DEPENDPATH += "../../src"
INCLUDEPATH += "../../src" "../../src/notifications"
QMAKE_LIBDIR = ../../src
equals(QT_MAJOR_VERSION, 4): LIBS = -llipstick
equals(QT_MAJOR_VERSION, 5): LIBS = -llipstick-qt5

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
