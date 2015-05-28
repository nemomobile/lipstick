/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** Copyright (C) 2012-2015 Jolla Ltd.
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
#include <QtTest/QtTest>
#include <QQmlContext>
#include <QScreen>
#include <usbmodeselector.h>
#include <qusbmoded.h>

#include "ut_usbmodeselector.h"
#include "qmlocks_stub.h"
#include "notificationmanager_stub.h"
#include "closeeventeater_stub.h"
#include "homewindow.h"
#include "lipstickqmlpath_stub.h"

HomeWindow::HomeWindow()
{
}

HomeWindow::~HomeWindow()
{
}

QList<HomeWindow *> homeWindows;
void HomeWindow::setSource(const QUrl &)
{
    homeWindows.append(this);
}

QHash<HomeWindow *, QString> homeWindowCategory;
void HomeWindow::setCategory(const QString &category)
{
    homeWindowCategory[this] = category;
}

QHash<HomeWindow *, QString> homeWindowTitle;
void HomeWindow::setWindowTitle(const QString &title)
{
    homeWindowTitle[this] = title;
}

QHash<HomeWindow *, bool> homeWindowVisible;
void HomeWindow::show()
{
    homeWindowVisible[this] = true;
}

void HomeWindow::hide()
{
    homeWindowVisible[this] = false;
}

bool HomeWindow::isVisible() const
{
    return homeWindowVisible[const_cast<HomeWindow *>(this)];
}

QHash<HomeWindow *, QVariantMap> homeWindowContextProperties;
void HomeWindow::setContextProperty(const QString &key, const QVariant &value)
{
    homeWindowContextProperties[this].insert(key, value);
}

void HomeWindow::setContextProperty(const QString &key, QObject *value)
{
    homeWindowContextProperties[this].insert(key, QVariant::fromValue(static_cast<QObject *>(value)));
}

void HomeWindow::setGeometry(const QRect &)
{
}

int argc = 1;
char *argv[] = { (char *) "./ut_usbmodeselector", NULL };

void Ut_USBModeSelector::initTestCase()
{
}

void Ut_USBModeSelector::cleanupTestCase()
{
}

void Ut_USBModeSelector::init()
{
    usbModeSelector = new USBModeSelector;
    usbModeSelector->usbMode->setCurrentMode(QUsbModed::Mode::Undefined);

    gNotificationManagerStub->stubReset();
    gNotificationManagerStub->stubSetReturnValue("Notify", (uint)1);
}

void Ut_USBModeSelector::cleanup()
{
    delete usbModeSelector;
    homeWindows.clear();
    homeWindowVisible.clear();
    gNotificationManagerStub->stubReset();
}

void Ut_USBModeSelector::testConnections()
{
    QCOMPARE(disconnect(usbModeSelector->usbMode, SIGNAL(currentModeChanged()), usbModeSelector, SLOT(applyCurrentUSBMode())), true);
    QCOMPARE(disconnect(usbModeSelector->usbMode, SIGNAL(supportedModesChanged()), usbModeSelector, SIGNAL(supportedUSBModesChanged())), true);
}

void Ut_USBModeSelector::testShowDialog_data()
{
    QTest::addColumn<QString>("mode");

    QTest::newRow("Ask") << QUsbModed::Mode::Ask;
    QTest::newRow("Mode request") << QUsbModed::Mode::ModeRequest;
}

void Ut_USBModeSelector::testShowDialog()
{
    QFETCH(QString, mode);

    QSignalSpy spy(usbModeSelector, SIGNAL(dialogShown()));
    usbModeSelector->usbMode->setConfigMode(mode);
    usbModeSelector->applyUSBMode(mode);

    QCOMPARE(homeWindows.count(), 1);

    // Check window properties
    QCOMPARE(homeWindowCategory[homeWindows.first()], QLatin1String("dialog"));
    QCOMPARE(homeWindowTitle[homeWindows.first()], QString("USB Mode"));
    QCOMPARE(homeWindowContextProperties[homeWindows.first()].value("initialSize").toSize(), QGuiApplication::primaryScreen()->size());
    QCOMPARE(homeWindowContextProperties[homeWindows.first()].value("usbModeSelector"), QVariant::fromValue(static_cast<QObject *>(usbModeSelector)));

    // Check that the window was shown
    QCOMPARE(homeWindowVisible[homeWindows.first()], true);
    QCOMPARE(spy.count(), 1);
}

void Ut_USBModeSelector::testHideDialog_data()
{
    QTest::addColumn<QString>("mode");

    QTest::newRow("Disconnected") << QUsbModed::Mode::Disconnected;
    QTest::newRow("PC Suite") << QUsbModed::Mode::PCSuite;
    QTest::newRow("Mass Storage") << QUsbModed::Mode::MassStorage;
    QTest::newRow("MTP") << QUsbModed::Mode::MTP;
    QTest::newRow("Developer") << QUsbModed::Mode::Developer;
    QTest::newRow("Adb") << QUsbModed::Mode::Adb;
    QTest::newRow("Diag") << QUsbModed::Mode::Diag;
    QTest::newRow("Host") << QUsbModed::Mode::Host;
    QTest::newRow("Cellular connection sharing") << QUsbModed::Mode::ConnectionSharing;
}

void Ut_USBModeSelector::testHideDialog()
{
    QFETCH(QString, mode);

    usbModeSelector->usbMode->setConfigMode(QUsbModed::Mode::Ask);
    usbModeSelector->applyUSBMode(QUsbModed::Mode::Ask);
    usbModeSelector->applyUSBMode(mode);
    QCOMPARE(homeWindowVisible[homeWindows.first()], false);
}

void Ut_USBModeSelector::testUSBNotifications_data()
{
    QTest::addColumn<QString>("mode");
    QTest::addColumn<QString>("category");
    QTest::addColumn<QString>("body");

    QTest::newRow("Disconnected") << QUsbModed::Mode::Disconnected << "device.removed" << qtTrId("qtn_usb_disconnected");
    QTest::newRow("PC Suite") << QUsbModed::Mode::PCSuite << "device.added" << qtTrId("qtn_usb_sync_active");

    QTest::newRow("Mass Storage") << QUsbModed::Mode::MassStorage << "device.added" << qtTrId("qtn_usb_storage_active");
    QTest::newRow("Developer") << QUsbModed::Mode::Developer << "device.added" << qtTrId("qtn_usb_sdk_active");
    QTest::newRow("MTP") << QUsbModed::Mode::MTP << "device.added" << qtTrId("qtn_usb_mtp_active");
    QTest::newRow("Adb") << QUsbModed::Mode::Adb << "device.added" << qtTrId("qtn_usb_adb_active");
    QTest::newRow("Diag") << QUsbModed::Mode::Diag << "device.added" << qtTrId("qtn_usb_diag_active");
    QTest::newRow("Host") << QUsbModed::Mode::Host << "device.added" << qtTrId("qtn_usb_host_mode_active");
    QTest::newRow("Cellular connection sharing") << QUsbModed::Mode::ConnectionSharing << "device.added" << qtTrId("qtn_usb_connection_sharing_active");
}

void Ut_USBModeSelector::testUSBNotifications()
{
    QFETCH(QString, mode);
    QFETCH(QString, category);
    QFETCH(QString, body);

    usbModeSelector->applyUSBMode(mode);
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_CATEGORY).toString(), category);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), body);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QString>(2), QString());
}

Q_DECLARE_METATYPE(MeeGo::QmLocks::State)

void Ut_USBModeSelector::testConnectingUSBWhenDeviceIsLockedEmitsDialogShown_data()
{
    QTest::addColumn<MeeGo::QmLocks::State>("deviceLocked");
    QTest::addColumn<int>("dialogShownCount");
    QTest::newRow("Device locked") << MeeGo::QmLocks::Locked << 1;
    QTest::newRow("Device not locked") << MeeGo::QmLocks::Unlocked << 0;
}

void Ut_USBModeSelector::testConnectingUSBWhenDeviceIsLockedEmitsDialogShown()
{
    QFETCH(MeeGo::QmLocks::State, deviceLocked);
    QFETCH(int, dialogShownCount);

    QSignalSpy spy(usbModeSelector, SIGNAL(dialogShown()));
    gQmLocksStub->stubSetReturnValue("getState", deviceLocked);
    usbModeSelector->applyUSBMode(QUsbModed::Mode::Connected);
    QCOMPARE(spy.count(), dialogShownCount);
}

void Ut_USBModeSelector::testShowError()
{
    usbModeSelector->showError("test");
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 0);

    usbModeSelector->showError("qtn_usb_filessystem_inuse");
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 1);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_usb_filessystem_inuse"));

    usbModeSelector->showError("mount_failed");
    QCOMPARE(gNotificationManagerStub->stubCallCount("Notify"), 2);
    QCOMPARE(gNotificationManagerStub->stubLastCallTo("Notify").parameter<QVariantHash>(6).value(NotificationManager::HINT_PREVIEW_BODY).toString(), qtTrId("qtn_usb_mount_failed"));
}

void Ut_USBModeSelector::testSetUSBMode()
{
    usbModeSelector->setUSBMode(QUsbModed::Mode::Charging);
    QCOMPARE(usbModeSelector->usbMode->currentMode(), QUsbModed::Mode::Charging);
}

QTEST_MAIN (Ut_USBModeSelector)
