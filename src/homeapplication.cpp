/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlEngine>
#include <QScreen>
#include <QTimer>
#include <QDBusMessage>
#include <QDBusConnection>
#include <QIcon>
#include <QTranslator>
#include <QDebug>
#include <QEvent>
#include <QtGui/qpa/qplatformnativeinterface.h>
#include <systemd/sd-daemon.h>

#include "notifications/notificationmanager.h"
#include "notifications/notificationpreviewpresenter.h"
#include "notifications/batterynotifier.h"
#include "notifications/diskspacenotifier.h"
#include "notifications/thermalnotifier.h"
#include "screenlock/screenlock.h"
#include "screenlock/screenlockadaptor.h"
#include "devicelock/devicelock.h"
#include "devicelock/devicelockadaptor.h"
#include "lipsticksettings.h"
#include "homeapplication.h"
#include "homeapplicationadaptor.h"
#include "homewindow.h"
#include "compositor/lipstickcompositor.h"
#include "compositor/lipstickcompositorwindow.h"
#include "lipstickdbus.h"

#include "volume/volumecontrol.h"
#include "usbmodeselector.h"
#include "shutdownscreen.h"
#include "shutdownscreenadaptor.h"
#include "connectionselector.h"
#include "screenshotservice.h"
#include "screenshotserviceadaptor.h"

void HomeApplication::quitSignalHandler(int)
{
    qApp->quit();
}

static void registerDBusObject(QDBusConnection &bus, const char *path, QObject *object)
{
    if (!bus.registerObject(path, object)) {
        qWarning("Unable to register object at path %s: %s", path, bus.lastError().message().toUtf8().constData());
    }
}

HomeApplication::HomeApplication(int &argc, char **argv, const QString &qmlPath)
    : QGuiApplication(argc, argv)
    , _mainWindowInstance(0)
    , _qmlPath(qmlPath)
    , originalSigIntHandler(signal(SIGINT, quitSignalHandler))
    , originalSigTermHandler(signal(SIGTERM, quitSignalHandler))
    , updatesEnabled(true)
    , homeReadySent(false)
    , onUpdatesDisabledUnfocusedWindowId(0)
{
    setApplicationName("Lipstick");
    // TODO: autogenerate this from tags
    setApplicationVersion(VERSION);

    QTranslator *engineeringEnglish = new QTranslator(this);
    engineeringEnglish->load("lipstick_eng_en", "/usr/share/translations");
    installTranslator(engineeringEnglish);
    QTranslator *translator = new QTranslator(this);
    translator->load(QLocale(), "lipstick", "-", "/usr/share/translations");
    installTranslator(translator);

    // Initialize the QML engine
    qmlEngine = new QQmlEngine(this);

    // Initialize the notification manager
    NotificationManager::instance();
    new NotificationPreviewPresenter(this);

    // Create screen lock logic - not parented to "this" since destruction happens too late in that case
    screenLock = new ScreenLock;
    LipstickSettings::instance()->setScreenLock(screenLock);
    new ScreenLockAdaptor(screenLock);

    deviceLock = new DeviceLock(this);
    new DeviceLockAdaptor(deviceLock);

    volumeControl = new VolumeControl;
    new BatteryNotifier(this);
    new DiskSpaceNotifier(this);
    new ThermalNotifier(this);
    usbModeSelector = new USBModeSelector(this);
    connect(usbModeSelector, SIGNAL(dialogShown()), screenLock, SLOT(unlockScreen()));
    shutdownScreen = new ShutdownScreen(this);
    new ShutdownScreenAdaptor(shutdownScreen);
    connectionSelector = new ConnectionSelector(this);

    // MCE and usb-moded expect services to be registered on the system bus
    QDBusConnection systemBus = QDBusConnection::systemBus();
    if (!systemBus.registerService(LIPSTICK_DBUS_SERVICE_NAME)) {
        qWarning("Unable to register D-Bus service %s: %s", LIPSTICK_DBUS_SERVICE_NAME, systemBus.lastError().message().toUtf8().constData());
    }

    new HomeApplicationAdaptor(this);

    registerDBusObject(systemBus, LIPSTICK_DBUS_PATH, this);
    registerDBusObject(systemBus, LIPSTICK_DBUS_SCREENLOCK_PATH, screenLock);
    registerDBusObject(systemBus, LIPSTICK_DBUS_DEVICELOCK_PATH, deviceLock);
    registerDBusObject(systemBus, LIPSTICK_DBUS_SHUTDOWN_PATH, shutdownScreen);

    ScreenshotService *screenshotService = new ScreenshotService(this);
    new ScreenshotServiceAdaptor(screenshotService);
    QDBusConnection sessionBus = QDBusConnection::sessionBus();

    registerDBusObject(sessionBus, LIPSTICK_DBUS_SCREENSHOT_PATH, screenshotService);

    // Setting up the context and engine things
    qmlEngine->rootContext()->setContextProperty("initialSize", QGuiApplication::primaryScreen()->size());
    qmlEngine->rootContext()->setContextProperty("lipstickSettings", LipstickSettings::instance());
    qmlEngine->rootContext()->setContextProperty("deviceLock", deviceLock);

    connect(this, SIGNAL(homeReady()), this, SLOT(sendStartupNotifications()));
}

HomeApplication::~HomeApplication()
{
    emit aboutToDestroy();

    delete volumeControl;
    delete screenLock;
    delete _mainWindowInstance;
    delete qmlEngine;
}

HomeApplication *HomeApplication::instance()
{
    return qobject_cast<HomeApplication *>(qApp);
}

void HomeApplication::restoreSignalHandlers()
{
    signal(SIGINT, originalSigIntHandler);
    signal(SIGTERM, originalSigTermHandler);
}

void HomeApplication::sendHomeReadySignalIfNotAlreadySent()
{
    if (!homeReadySent) {
        homeReadySent = true;
        disconnect(LipstickCompositor::instance(), SIGNAL(frameSwapped()), this, SLOT(sendHomeReadySignalIfNotAlreadySent()));

        emit homeReady();
    }
}

void HomeApplication::sendStartupNotifications()
{
    static QDBusConnection systemBus = QDBusConnection::systemBus();
    QDBusMessage homeReadySignal =
        QDBusMessage::createSignal("/com/nokia/duihome",
                                   "com.nokia.duihome.readyNotifier",
                                   "ready");
    systemBus.send(homeReadySignal);

    /* Let systemd know that we are initialized */
    if (arguments().indexOf("--systemd") >= 0) {
        sd_notify(0, "READY=1");
    }

    /* Let timed know that the UI is up */
    systemBus.call(QDBusMessage::createSignal("/com/nokia/startup/signal", "com.nokia.startup.signal", "desktop_visible"), QDBus::NoBlock);
}

bool HomeApplication::homeActive() const
{
    LipstickCompositor *c = LipstickCompositor::instance();
    return c?c->homeActive():(QGuiApplication::focusWindow() != 0);
}

bool HomeApplication::event(QEvent *e)
{
    bool rv = QGuiApplication::event(e);
    if (LipstickCompositor::instance() == 0 &&
        (e->type() == QEvent::ApplicationActivate ||
         e->type() == QEvent::ApplicationDeactivate))
        emit homeActiveChanged();
    return rv;
}

const QString &HomeApplication::qmlPath() const
{
    return _qmlPath;
}

void HomeApplication::setQmlPath(const QString &path)
{
    _qmlPath = path;

    if (_mainWindowInstance) {
        _mainWindowInstance->setSource(path);
        if (_mainWindowInstance->hasErrors()) {
            qWarning() << "HomeApplication: Errors while loading" << path;
            qWarning() << _mainWindowInstance->errors();
        }
    }
}

const QString &HomeApplication::compositorPath() const
{
    return _compositorPath;
}

void HomeApplication::setCompositorPath(const QString &path)
{
    if (path.isEmpty()) {
        qWarning() << "HomeApplication: Invalid empty compositor path";
        return;
    }

    if (!_compositorPath.isEmpty()) {
        qWarning() << "HomeApplication: Compositor already set";
        return;
    }

    _compositorPath = path;
    QQmlComponent component(qmlEngine, QUrl(path));
    if (component.isError()) {
        qWarning() << "HomeApplication: Errors while loading compositor from" << path;
        qWarning() << component.errors();
        return;
    } 

    QObject *compositor = component.beginCreate(qmlEngine->rootContext());
    if (compositor) {
        compositor->setParent(this);
        if (LipstickCompositor::instance())
            LipstickCompositor::instance()->setGeometry(QRect(QPoint(0, 0), QGuiApplication::primaryScreen()->size()));

        component.completeCreate();

        if (!qmlEngine->incubationController() && LipstickCompositor::instance()) {
            // install default incubation controller
            qmlEngine->setIncubationController(LipstickCompositor::instance()->incubationController());
        }

        if (LipstickCompositor::instance())
            LipstickCompositor::instance()->show();
    } else {
        qWarning() << "HomeApplication: Error creating compositor from" << path;
        qWarning() << component.errors();
    }
}

HomeWindow *HomeApplication::mainWindowInstance()
{
    if (_mainWindowInstance)
        return _mainWindowInstance;

    _mainWindowInstance = new HomeWindow();
    _mainWindowInstance->setGeometry(QRect(QPoint(), QGuiApplication::primaryScreen()->size()));
    _mainWindowInstance->setWindowTitle("Home");
    QObject::connect(_mainWindowInstance->engine(), SIGNAL(quit()), qApp, SLOT(quit()));
    QObject::connect(_mainWindowInstance, SIGNAL(visibleChanged(bool)), this, SLOT(connectFrameSwappedSignal(bool)));

    // Setting the source, if present
    if (!_qmlPath.isEmpty())
        _mainWindowInstance->setSource(_qmlPath);

    return _mainWindowInstance;
}

QQmlEngine *HomeApplication::engine() const
{
    return qmlEngine;
}

void HomeApplication::setUpdatesEnabled(bool enabled)
{
    if (updatesEnabled != enabled) {
        updatesEnabled = enabled;

        if (!updatesEnabled) {
            LipstickCompositorWindow *topmostWindow = qobject_cast<LipstickCompositorWindow *>(LipstickCompositor::instance()->windowForId(LipstickCompositor::instance()->topmostWindowId()));
            if (topmostWindow != 0 && topmostWindow->hasFocus()) {
                onUpdatesDisabledUnfocusedWindowId = topmostWindow->windowId();
                LipstickCompositor::instance()->clearKeyboardFocus();
            }
            LipstickCompositor::instance()->hide();
            QGuiApplication::platformNativeInterface()->nativeResourceForIntegration("DisplayOff");
        } else {
            QGuiApplication::platformNativeInterface()->nativeResourceForIntegration("DisplayOn");
            emit LipstickCompositor::instance()->displayAboutToBeOn();
            LipstickCompositor::instance()->showFullScreen();
            if (onUpdatesDisabledUnfocusedWindowId > 0) {
                if (!screenLock->isScreenLocked()) {
                    LipstickCompositorWindow *topmostWindow = qobject_cast<LipstickCompositorWindow *>(LipstickCompositor::instance()->windowForId(LipstickCompositor::instance()->topmostWindowId()));
                    if (topmostWindow != 0 && topmostWindow->windowId() == onUpdatesDisabledUnfocusedWindowId) {
                        topmostWindow->takeFocus();
                    }
                }
                onUpdatesDisabledUnfocusedWindowId = 0;
            }
        }
    }
}

void HomeApplication::connectFrameSwappedSignal(bool mainWindowVisible)
{
    if (!homeReadySent && mainWindowVisible) {
        connect(LipstickCompositor::instance(), SIGNAL(frameSwapped()), this, SLOT(sendHomeReadySignalIfNotAlreadySent()));
    }
}
