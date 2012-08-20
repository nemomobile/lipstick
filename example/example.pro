
TEMPLATE = app
TARGET = lipstick
VERSION = 0.1

target.path += /usr/bin
INSTALLS = target

QT += network \
    svg \
    dbus \
    xml \
    declarative \
    opengl

HEADERS +=

SOURCES += \
    main.cpp

RESOURCES += \
    res.qrc

OTHER_FILES += \
    images/clarity_03_by_pr09studio.jpg \
    qml/main.qml \
    qml/Launcher.qml \
    qml/Switcher.qml \
    qml/SwitcherItem.qml

DEPENDPATH += "../src"
INCLUDEPATH += "../src"
LIBS += -L"../src" -llipstick
