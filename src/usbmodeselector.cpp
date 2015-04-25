/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** Copyright (C) 2012 Jolla Ltd.
** Contact: Robin Burchell <robin.burchell@jollamobile.com>
**
** This file is part of lipstick.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
#include <QGuiApplication>
#include "homewindow.h"
#include <QQmlContext>
#include <QScreen>
#include "utilities/closeeventeater.h"
#include <qmlocks.h>
#include "notifications/notificationmanager.h"
#include "usbmodeselector.h"
#include "lipstickqmlpath.h"

QMap<QString, QString> USBModeSelector::errorCodeToTranslationID;

USBModeSelector::USBModeSelector(QObject *parent) :
    QObject(parent),
    window(0)
    ,usbMode(new MeeGo::QmUSBMode(this)),
    locks(new MeeGo::QmLocks(this))
{
    if (errorCodeToTranslationID.isEmpty()) {
        errorCodeToTranslationID.insert("qtn_usb_filessystem_inuse", "qtn_usb_filessystem_inuse");
        errorCodeToTranslationID.insert("mount_failed", "qtn_usb_mount_failed");
    }

    connect(usbMode, SIGNAL(modeChanged(MeeGo::QmUSBMode::Mode)), this, SLOT(applyUSBMode(MeeGo::QmUSBMode::Mode)));
    connect(usbMode, SIGNAL(error(const QString &)), this, SLOT(showError(const QString &)));
    connect(usbMode, SIGNAL(supportedModesChanged(QList<MeeGo::QmUSBMode::Mode>)), this, SLOT(updateSupportedUSBModeList(QList<MeeGo::QmUSBMode::Mode>)));

    // Lazy initialize to improve startup time
    QTimer::singleShot(500, this, SLOT(applyCurrentUSBMode()));
}

void USBModeSelector::applyCurrentUSBMode()
{
    updateSupportedUSBModeList(usbMode->getSupportedModes());
    applyUSBMode(usbMode->getMode());
}

void USBModeSelector::setWindowVisible(bool visible)
{
    if (visible) {
        emit dialogShown();

        if (window == 0) {
            window = new HomeWindow();
            window->setGeometry(QRect(QPoint(), QGuiApplication::primaryScreen()->size()));
            window->setCategory(QLatin1String("dialog"));
            window->setWindowTitle("USB Mode");
            window->setContextProperty("initialSize", QGuiApplication::primaryScreen()->size());
            window->setContextProperty("usbModeSelector", this);
            window->setSource(QmlPath::to("connectivity/USBModeSelector.qml"));
            window->installEventFilter(new CloseEventEater(this));
        }

        if (!window->isVisible()) {
            window->show();
            emit windowVisibleChanged();
        }
    } else if (window != 0 && window->isVisible()) {
        window->hide();
        emit windowVisibleChanged();
    }
}

bool USBModeSelector::windowVisible() const
{
    return window != 0 && window->isVisible();
}

QList<int> USBModeSelector::supportedUSBModes() const
{
    return supportedUSBModeList;
}

void USBModeSelector::applyUSBMode(MeeGo::QmUSBMode::Mode mode)
{
    switch (mode) {
    case MeeGo::QmUSBMode::Connected:
        if (locks->getState(MeeGo::QmLocks::Device) == MeeGo::QmLocks::Locked) {
            // When the device lock is on and USB is connected, always pretend that the USB mode selection dialog is shown to unlock the touch screen lock
            emit dialogShown();

            // Show a notification instead
            NotificationManager *manager = NotificationManager::instance();
            QVariantHash hints;
            hints.insert(NotificationManager::HINT_CATEGORY, "x-nemo.device.locked");
            //% "Unlock device first"
            hints.insert(NotificationManager::HINT_PREVIEW_BODY, qtTrId("qtn_usb_device_locked"));
            manager->Notify(qApp->applicationName(), 0, QString(), QString(), QString(), QStringList(), hints, -1);
        }
        break;
    case MeeGo::QmUSBMode::Ask:
    case MeeGo::QmUSBMode::ModeRequest:
        setWindowVisible(true);
        break;
    case MeeGo::QmUSBMode::Disconnected:
    case MeeGo::QmUSBMode::MassStorage:
    case MeeGo::QmUSBMode::MTP:
    case MeeGo::QmUSBMode::Developer:
    case MeeGo::QmUSBMode::Adb:
    case MeeGo::QmUSBMode::Diag:
    case MeeGo::QmUSBMode::Host:
    case MeeGo::QmUSBMode::ConnectionSharing:
        // Hide the mode selection dialog and show a mode notification
        setWindowVisible(false);
        showNotification(mode);
        break;
    case MeeGo::QmUSBMode::ChargingOnly:
    case MeeGo::QmUSBMode::Undefined:
        // no-op
        break;
    default:
        // Hide the mode selection dialog
        setWindowVisible(false);
        break;
    }
}

void USBModeSelector::showNotification(MeeGo::QmUSBMode::Mode mode)
{
    QString category = "device.added";
    QString body;
    switch (mode) {
    case MeeGo::QmUSBMode::MassStorage:
        //% "Mass storage in use"
        body = qtTrId("qtn_usb_storage_active");
        break;
    case MeeGo::QmUSBMode::Developer:
        //% "SDK mode in use"
        body = qtTrId("qtn_usb_sdk_active");
        break;
    case MeeGo::QmUSBMode::MTP:
        //% "MTP mode in use"
        body = qtTrId("qtn_usb_mtp_active");
        break;
    case MeeGo::QmUSBMode::Adb:
        //% "Adb mode in use"
        body = qtTrId("qtn_usb_adb_active");
        break;
    case MeeGo::QmUSBMode::Diag:
        //% "Diag mode in use"
        body = qtTrId("qtn_usb_diag_active");
        break;
    case MeeGo::QmUSBMode::Disconnected:
        category = "device.removed";
        //% "USB cable disconnected"
        body = qtTrId("qtn_usb_disconnected");
        break;
    case MeeGo::QmUSBMode::Host:
        //% "USB switched to host mode (OTG)"
        body = qtTrId("qtn_usb_host_mode_active");
        break;
    case MeeGo::QmUSBMode::ConnectionSharing:
        //% "USB tethering in use"
        body = qtTrId("qtn_usb_connection_sharing_active");
        break;
    default:
        return;
    }

    NotificationManager *manager = NotificationManager::instance();
    QVariantHash hints;
    hints.insert(NotificationManager::HINT_CATEGORY, category);
    hints.insert(NotificationManager::HINT_PREVIEW_BODY, body);
    manager->Notify(qApp->applicationName(), 0, QString(), QString(), QString(), QStringList(), hints, -1);
}

void USBModeSelector::showError(const QString &errorCode)
{
    if (errorCodeToTranslationID.contains(errorCode)) {
        NotificationManager *manager = NotificationManager::instance();
        QVariantHash hints;
        hints.insert(NotificationManager::HINT_CATEGORY, "device.error");
        //% "USB connection error occurred"
        hints.insert(NotificationManager::HINT_PREVIEW_BODY, qtTrId(errorCodeToTranslationID.value(errorCode).toUtf8().constData()));
        manager->Notify(qApp->applicationName(), 0, QString(), QString(), QString(), QStringList(), hints, -1);
    }
}

void USBModeSelector::setUSBMode(int mode)
{
    usbMode->setMode((MeeGo::QmUSBMode::Mode)mode);
}

void USBModeSelector::updateSupportedUSBModeList(const QList<MeeGo::QmUSBMode::Mode> &supportedModes)
{
    supportedUSBModeList.clear();
    foreach (MeeGo::QmUSBMode::Mode supportedMode, supportedModes) {
        supportedUSBModeList.append(supportedMode);
    }
    emit supportedUSBModesChanged();
}
