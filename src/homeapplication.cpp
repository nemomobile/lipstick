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

#include "notifications/notificationmanager.h"
#include "notifications/notificationpreviewpresenter.h"
#include "notifications/notificationfeedbackplayer.h"
#include "notifications/batterynotifier.h"
#include "screenlock/screenlock.h"
#include "screenlock/screenlockadaptor.h"
#include "lipsticksettings.h"
#include "homeapplication.h"

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

    // Initialize the notification manager
    NotificationManager::instance();
//    new NotificationPreviewPresenter(this);
    new NotificationFeedbackPlayer(this);

    // Create screen lock logic - not parented to "this" since destruction happens too late in that case
    screenLock = new ScreenLock;
    LipstickSettings::instance()->setScreenLock(screenLock);
    new ScreenLockAdaptor(screenLock);

    /*
    volumeControl = new VolumeControl;
    batteryNotifier = new BatteryNotifier(this);
    usbModeSelector = new USBModeSelector(this);
    connect(usbModeSelector, SIGNAL(dialogShown()), screenLock, SLOT(unlockScreen()));
    shutdownScreen = new ShutdownScreen(this);
    connectionSelector = new ConnectionSelector(this);
*/

    // MCE expects the service to be registered on the system bus
    static const char *SCREENLOCK_DBUS_SERVICE = "org.nemomobile.lipstick";
    static const char *SCREENLOCK_DBUS_PATH = "/screenlock";
    QDBusConnection systemBus = QDBusConnection::systemBus();
    if (!systemBus.registerService(SCREENLOCK_DBUS_SERVICE)) {
        qWarning("Unable to register screen lock D-Bus service %s: %s", SCREENLOCK_DBUS_SERVICE, systemBus.lastError().message().toUtf8().constData());
    }
    if (!systemBus.registerObject(SCREENLOCK_DBUS_PATH, screenLock)) {
        qWarning("Unable to register screen lock object at path %s: %s", SCREENLOCK_DBUS_PATH, systemBus.lastError().message().toUtf8().constData());
    }
}

HomeApplication::~HomeApplication()
{
    delete volumeControl;
    delete screenLock;
    delete _mainWindowInstance;
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

const QString &HomeApplication::qmlPath() const
{
    return _qmlPath;
}

void HomeApplication::setQmlPath(const QString &path)
{
    _qmlPath = path;

    if (_mainWindowInstance)
        _mainWindowInstance->setSource(path);
}

QQuickView *HomeApplication::mainWindowInstance()
{
    if (_mainWindowInstance)
        return _mainWindowInstance;

    _mainWindowInstance = new QQuickView();
    _mainWindowInstance->setGeometry(QRect(QPoint(), QGuiApplication::primaryScreen()->size()));
    _mainWindowInstance->setResizeMode(QQuickView::SizeRootObjectToView);
    /*
    // Setting optimalization flags
    _mainWindowInstance->setAttribute(Qt::WA_X11NetWmWindowTypeDesktop);
    _mainWindowInstance->setOptimizationFlag(QGraphicsView::DontSavePainterState);
    _mainWindowInstance->setAutoFillBackground(false);
    _mainWindowInstance->setAttribute(Qt::WA_OpaquePaintEvent);
    _mainWindowInstance->setAttribute(Qt::WA_NoSystemBackground);
    _mainWindowInstance->viewport()->setAutoFillBackground(false);
    _mainWindowInstance->viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
    _mainWindowInstance->viewport()->setAttribute(Qt::WA_NoSystemBackground);
*/

    // Setting up the context and engine things
    QObject::connect(_mainWindowInstance->engine(), SIGNAL(quit()), qApp, SLOT(quit()));
    _mainWindowInstance->rootContext()->setContextProperty("initialSize", QGuiApplication::primaryScreen()->size());
    _mainWindowInstance->rootContext()->setContextProperty("LipstickSettings", LipstickSettings::instance());

    // Setting the source, if present
    if (!_qmlPath.isEmpty())
        _mainWindowInstance->setSource(_qmlPath);

    _mainWindowInstance->installEventFilter(this);

    return _mainWindowInstance;
}
