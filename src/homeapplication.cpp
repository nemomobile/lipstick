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

#include "notifications/notificationmanager.h"
#include "notifications/notificationpreviewpresenter.h"
#include "notifications/notificationfeedbackplayer.h"
#include "notifications/batterynotifier.h"
#include "notifications/diskspacenotifier.h"
#include "screenlock/screenlock.h"
#include "screenlock/screenlockadaptor.h"
#include "devicelock/devicelock.h"
#include "devicelock/devicelockadaptor.h"
#include "lipsticksettings.h"
#include "homeapplication.h"
#include "homeapplicationadaptor.h"
#include "homewindow.h"
#include "compositor/lipstickcompositor.h"

#include "volume/volumecontrol.h"
#include "usbmodeselector.h"
#include "shutdownscreen.h"
#include "connectionselector.h"

// Define this if you'd like to see debug messages from the home app
#ifdef DEBUG_HOME
#define HOME_DEBUG(things) qDebug() << Q_FUNC_INFO << things
#else
#define HOME_DEBUG(things)
#endif

void HomeApplication::quitSignalHandler(int)
{
    qApp->quit();
}

HomeApplication::HomeApplication(int &argc, char **argv, const QString &qmlPath)
    : QGuiApplication(argc, argv)
    , _mainWindowInstance(0)
    , _qmlPath(qmlPath)
    , originalSigIntHandler(signal(SIGINT, quitSignalHandler))
    , originalSigTermHandler(signal(SIGTERM, quitSignalHandler))
    , updatesEnabled(true)
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

    // launch a timer for sending a dbus-signal upstart when basic construct is done
    QTimer::singleShot(0, this, SLOT(sendStartupNotifications()));

    // Initialize the QML engine
    qmlEngine = new QQmlEngine(this);

    // Initialize the notification manager
    NotificationManager::instance();
    new NotificationFeedbackPlayer(new NotificationPreviewPresenter(this));

    // Create screen lock logic - not parented to "this" since destruction happens too late in that case
    screenLock = new ScreenLock;
    LipstickSettings::instance()->setScreenLock(screenLock);
    new ScreenLockAdaptor(screenLock);

    deviceLock = new DeviceLock(this);
    new DeviceLockAdaptor(deviceLock);

    volumeControl = new VolumeControl;
    new BatteryNotifier(this);
    new DiskSpaceNotifier(this);
    usbModeSelector = new USBModeSelector(this);
    connect(usbModeSelector, SIGNAL(dialogShown()), screenLock, SLOT(unlockScreen()));
    shutdownScreen = new ShutdownScreen(this);
    connectionSelector = new ConnectionSelector(this);

    // MCE and usb-moded expect services to be registered on the system bus
    QDBusConnection systemBus = QDBusConnection::systemBus();
    static const char *SYSTEM_DBUS_SERVICE = "org.nemomobile.lipstick";
    if (!systemBus.registerService(SYSTEM_DBUS_SERVICE)) {
        qWarning("Unable to register D-Bus service %s: %s", SYSTEM_DBUS_SERVICE, systemBus.lastError().message().toUtf8().constData());
    }

    new HomeApplicationAdaptor(this);
    static const char *LIPSTICK_DBUS_PATH = "/";
    if (!systemBus.registerObject(LIPSTICK_DBUS_PATH, this)) {
        qWarning("Unable to register lipstick object at path %s: %s", LIPSTICK_DBUS_PATH, systemBus.lastError().message().toUtf8().constData());
    }

    static const char *SCREENLOCK_DBUS_PATH = "/screenlock";
    if (!systemBus.registerObject(SCREENLOCK_DBUS_PATH, screenLock)) {
        qWarning("Unable to register screen lock object at path %s: %s", SCREENLOCK_DBUS_PATH, systemBus.lastError().message().toUtf8().constData());
    }

    static const char *DEVICELOCK_DBUS_PATH = "/devicelock";
    if (!systemBus.registerObject(DEVICELOCK_DBUS_PATH, deviceLock)) {
        qWarning("Unable to register device lock object at path %s: %s", DEVICELOCK_DBUS_PATH, systemBus.lastError().message().toUtf8().constData());
    }
}

HomeApplication::~HomeApplication()
{
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

void HomeApplication::sendStartupNotifications()
{
    static QDBusConnection systemBus = QDBusConnection::systemBus();
    QDBusMessage homeReadySignal =
        QDBusMessage::createSignal("/com/nokia/duihome",
                                   "com.nokia.duihome.readyNotifier",
                                   "ready");
    systemBus.send(homeReadySignal);

    // For device boot performance reasons initializing Home scene window must be done
    // only after ready signal is sent.
    mainWindowInstance()->showFullScreen();
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

    // Setting up the context and engine things
    _mainWindowInstance->setContextProperty("initialSize", QGuiApplication::primaryScreen()->size());
    _mainWindowInstance->setContextProperty("LipstickSettings", LipstickSettings::instance());
    _mainWindowInstance->setContextProperty("deviceLock", deviceLock);

    QObject::connect(_mainWindowInstance->engine(), SIGNAL(quit()), qApp, SLOT(quit()));

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
            LipstickCompositor::instance()->hide();
            QGuiApplication::platformNativeInterface()->nativeResourceForIntegration("DisplayOff");
        } else {
            QGuiApplication::platformNativeInterface()->nativeResourceForIntegration("DisplayOn");
            LipstickCompositor::instance()->showFullScreen();
        }
    }
}
