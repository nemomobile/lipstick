system(qdbusxml2cpp notifications/notificationmanager.xml -a notifications/notificationmanageradaptor -c NotificationManagerAdaptor -l NotificationManager -i notificationmanager.h)
system(qdbusxml2cpp screenlock/screenlock.xml -a screenlock/screenlockadaptor -c ScreenLockAdaptor -l ScreenLock -i screenlock.h)

TEMPLATE = lib
TARGET = lipstick-qt5
VERSION = 0.11.0

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x000000
DEFINES += LIPSTICK_BUILD_LIBRARY VERSION=\\\"$$VERSION\\\"

CONFIG += qt
INSTALLS = target ts_install engineering_english_install
target.path = $$[QT_INSTALL_LIBS]

QMAKE_STRIP = echo
OBJECTS_DIR = .obj
MOC_DIR = .moc

INCLUDEPATH += utilities components xtools

include(compositor/compositor.pri)

PUBLICHEADERS += \
    utilities/qobjectlistmodel.h \
    utilities/closeeventeater.h \
    homeapplication.h \
    homewindow.h \
    lipstickglobal.h \
    lipsticksettings.h \
    components/launcheritem.h \
    components/launchermodel.h \
    notifications/notificationmanager.h \
    notifications/notification.h \
    notifications/notificationlistmodel.h \
    notifications/notificationpreviewpresenter.h \
    usbmodeselector.h \
    shutdownscreen.h \
    connectionselector.h

INSTALLS += publicheaderfiles dbus_policy
publicheaderfiles.files = $$PUBLICHEADERS
publicheaderfiles.path = /usr/include/lipstick-qt5
dbus_policy.files += lipstick.conf
dbus_policy.path = /etc/dbus-1/system.d

HEADERS += \
    $$PUBLICHEADERS \
    notifications/notificationmanageradaptor.h \
    notifications/categorydefinitionstore.h \
    notifications/batterynotifier.h \
    notifications/lowbatterynotifier.h \
    notifications/notificationfeedbackplayer.h \
    screenlock/screenlock.h \
    screenlock/screenlockadaptor.h \
    volume/volumekeylistener.h \
    volume/volumecontrol.h \
    volume/pulseaudiocontrol.h \
    lipstickapi.h \


SOURCES += \
    homeapplication.cpp \
    homewindow.cpp \
    lipsticksettings.cpp \
    utilities/qobjectlistmodel.cpp \
    utilities/closeeventeater.cpp \
    components/launcheritem.cpp \
    components/launchermodel.cpp \
    notifications/notificationmanager.cpp \
    notifications/notificationmanageradaptor.cpp \
    notifications/notification.cpp \
    notifications/categorydefinitionstore.cpp \
    notifications/notificationlistmodel.cpp \
    notifications/notificationpreviewpresenter.cpp \
    notifications/batterynotifier.cpp \
    notifications/lowbatterynotifier.cpp \
    screenlock/screenlock.cpp \
    screenlock/screenlockadaptor.cpp \
    volume/volumekeylistener.cpp \
    volume/volumecontrol.cpp \
    volume/pulseaudiocontrol.cpp \
    notifications/notificationfeedbackplayer.cpp \
    usbmodeselector.cpp \
    shutdownscreen.cpp \
    connectionselector.cpp \
    lipstickapi.cpp \

CONFIG += link_pkgconfig mobility qt warn_on depend_includepath qmake_cache target_qt
MOBILITY += sensors systeminfo
PKGCONFIG += mlite5 mce dbus-1 dbus-glib-1 libresourceqt5 ngf-qt5 qmsystem2-qt5

packagesExist(contentaction5) {
    message("Using contentaction to launch applications")
    PKGCONFIG += contentaction5
    DEFINES += HAVE_CONTENTACTION
} else {
    warning("contentaction doesn't exist; falling back to exec - this may not work so great")
}

packagesExist(contextsubscriber-1.0) {
    PKGCONFIG += contextsubscriber-1.0
    DEFINES += HAVE_CONTEXTSUBSCRIBER
} else {
    warning("Contextsubscriber not found")
}

QT += dbus xml qml quick sql systeminfo gui

QMAKE_CXXFLAGS += \
    -Werror \
    -Wno-error=format-security \
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

# translations
TS_FILE = $$OUT_PWD/lipstick.ts
EE_QM = $$OUT_PWD/lipstick_eng_en.qm
ts.commands += lupdate $$PWD -ts $$TS_FILE
ts.CONFIG += no_check_exist
ts.output = $$TS_FILE
ts.input = .
ts_install.files = $$TS_FILE
ts_install.path = /usr/share/translations/source
ts_install.CONFIG += no_check_exist

# should add -markuntranslated "-" when proper translations are in place (or for testing)
engineering_english.commands += lrelease -idbased $$TS_FILE -qm $$EE_QM
engineering_english.CONFIG += no_check_exist
engineering_english.depends = ts
engineering_english.input = $$TS_FILE
engineering_english.output = $$EE_QM
engineering_english_install.path = /usr/share/translations
engineering_english_install.files = $$EE_QM
engineering_english_install.CONFIG += no_check_exist

QMAKE_EXTRA_TARGETS += ts engineering_english
PRE_TARGETDEPS += ts engineering_english

include(notificationcategories/notificationcategories.pri)
