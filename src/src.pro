system(qdbusxml2cpp notifications/notificationmanager.xml -a notifications/notificationmanageradaptor -c NotificationManagerAdaptor -l NotificationManager -i notificationmanager.h)

TEMPLATE = lib
TARGET = lipstick
VERSION = 0.1

DEFINES += LIPSTICK_BUILD_LIBRARY DEBUG_NOTIFICATIONS

CONFIG += qt
INSTALLS = target
target.path = $$[QT_INSTALL_LIBS]

QMAKE_STRIP = echo
OBJECTS_DIR = .obj
MOC_DIR = .moc

PUBLICHEADERS += \
    homeapplication.h \
    lipstickglobal.h \
    components/windowinfo.h \
    components/launcheritem.h \
    components/launchermodel.h \
    components/switchermodel.h \
    components/switcherpixmapitem.h \
    components/statusbar.h \
    components/windowmanager.h \
    notifications/notificationmanager.h \
    notifications/notificationhints.h \
    notifications/notification.h

INSTALLS += publicheaderfiles
publicheaderfiles.files = $$PUBLICHEADERS
publicheaderfiles.path = /usr/include/lipstick

HEADERS += \
    $$PUBLICHEADERS \
    utilities/qobjectlistmodel.h \
    xtools/homewindowmonitor.h \
    xtools/windowmonitor.h \
    xtools/xeventlistener.h \
    xtools/xatomcache.h \
    xtools/xwindowmanager.h \
    lipstickdbusinterface.h \
    lipsticksettings.h \
    notifications/notificationmanager.h \
    notifications/notificationmanageradaptor.h \
    notifications/notificationhints.h \
    notifications/notification.h \
    notifications/categorydefinitionstore.h

SOURCES += \
    homeapplication.cpp \
    lipstickdbusinterface.cpp \
    lipsticksettings.cpp \
    utilities/qobjectlistmodel.cpp \
    xtools/homewindowmonitor.cpp \
    xtools/xeventlistener.cpp \
    xtools/xatomcache.cpp \
    xtools/xwindowmanager.cpp \
    components/windowinfo.cpp \
    components/launcheritem.cpp \
    components/launchermodel.cpp \
    components/switchermodel.cpp \
    components/switcherpixmapitem.cpp \
    components/statusbar.cpp \
    components/windowmanager.cpp \
    notifications/notificationmanager.cpp \
    notifications/notificationmanageradaptor.cpp \
    notifications/notificationhints.cpp \
    notifications/notification.cpp \
    notifications/categorydefinitionstore.cpp

CONFIG += link_pkgconfig mobility qt warn_on depend_includepath qmake_cache target_qt
MOBILITY += sensors
PKGCONFIG += xcomposite mlite xdamage x11

packagesExist(contentaction-0.1) {
    message("Using contentaction to launch applications")
    PKGCONFIG += contentaction-0.1
    DEFINES += HAVE_CONTENTACTION
}
else {
    warning("contentaction doesn't exist; falling back to exec - this may not work so great")
}

QT += network \
    svg \
    dbus \
    xml \
    declarative \
    opengl

QMAKE_CXXFLAGS += \
    -Werror \
    -g \
    -std=c++0x \
    -fPIC \
    -fvisibility=hidden \
    -fvisibility-inlines-hidden

QMAKE_LFLAGS += \
    -pie \
    -rdynamic

QMAKE_CLEAN += \
    *.gcov \
    ./.obj/*.gcno

CONFIG += create_pc create_prl
QMAKE_PKGCONFIG_NAME = lib$$TARGET
QMAKE_PKGCONFIG_DESCRIPTION = Library for creating QML desktops
QMAKE_PKGCONFIG_LIBDIR = $$target.path
QMAKE_PKGCONFIG_INCDIR = $$publicheaderfiles.path
QMAKE_PKGCONFIG_DESTDIR = pkgconfig

pkgconfig.files = $$TARGET.pc
pkgconfig.path = $$target.path/pkgconfig
INSTALLS += pkgconfig
