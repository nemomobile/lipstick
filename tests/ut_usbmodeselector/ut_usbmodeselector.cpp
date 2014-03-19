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
#include <QtTest/QtTest>
#include <QQmlContext>
#include <QScreen>
#include <usbmodeselector.h>

#include "ut_usbmodeselector.h"
#include "qmlocks_stub.h"
#include "qmusbmode_stub.h"
#include "notificationmanager_stub.h"
#include "closeeventeater_stub.h"
#include "homewindow.h"

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
    usbModeSelector->usbMode->setMode(MeeGo::QmUSBMode::Undefined);

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
    QCOMPARE(disconnect(usbModeSelector->usbMode, SIGNAL(modeChanged(MeeGo::QmUSBMode::Mode)), usbModeSelector, SLOT(applyUSBMode(MeeGo::QmUSBMode::Mode))), true);
    QCOMPARE(disconnect(usbModeSelector->usbMode, SIGNAL(supportedModesChanged(QList<MeeGo::QmUSBMode::Mode>)), usbModeSelector, SLOT(updateSupportedUSBModeList(QList<MeeGo::QmUSBMode::Mode>))), true);
}

Q_DECLARE_METATYPE(MeeGo::QmUSBMode::Mode)

void Ut_USBModeSelector::testShowDialog_data()
{
    QTest::addColumn<MeeGo::QmUSBMode::Mode>("mode");

    QTest::newRow("Ask") << MeeGo::QmUSBMode::Ask;
    QTest::newRow("Mode request") << MeeGo::QmUSBMode::ModeRequest;
}

void Ut_USBModeSelector::testShowDialog()
{
    QFETCH(MeeGo::QmUSBMode::Mode, mode);

    QSignalSpy spy(usbModeSelector, SIGNAL(dialogShown()));
    usbModeSelector->usbMode->setDefaultMode(mode);
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
    QTest::addColumn<MeeGo::QmUSBMode::Mode>("mode");

    QTest::newRow("Disconnected") << MeeGo::QmUSBMode::Disconnected;
    QTest::newRow("Ovi Suite") << MeeGo::QmUSBMode::OviSuite;
    QTest::newRow("Mass Storage") << MeeGo::QmUSBMode::MassStorage;
    QTest::newRow("SDK") << MeeGo::QmUSBMode::SDK;
    QTest::newRow("MTP") << MeeGo::QmUSBMode::MTP;
    QTest::newRow("Developer") << MeeGo::QmUSBMode::Developer;
    QTest::newRow("Adb") << MeeGo::QmUSBMode::Adb;
    QTest::newRow("Diag") << MeeGo::QmUSBMode::Diag;
    QTest::newRow("Cellular connection sharing") << MeeGo::QmUSBMode::ConnectionSharing;
}

void Ut_USBModeSelector::testHideDialog()
{
    QFETCH(MeeGo::QmUSBMode::Mode, mode);

    usbModeSelector->usbMode->setDefaultMode(MeeGo::QmUSBMode::Ask);
    usbModeSelector->applyUSBMode(MeeGo::QmUSBMode::Ask);
    usbModeSelector->applyUSBMode(mode);
    QCOMPARE(homeWindowVisible[homeWindows.first()], false);
}

void Ut_USBModeSelector::testUSBNotifications_data()
{
    QTest::addColumn<MeeGo::QmUSBMode::Mode>("mode");
    QTest::addColumn<QString>("category");
    QTest::addColumn<QString>("body");

    QTest::newRow("Disconnected") << MeeGo::QmUSBMode::Disconnected << "device.removed" << qtTrId("qtn_usb_disconnected");
    QTest::newRow("Ovi Suite") << MeeGo::QmUSBMode::OviSuite << "device.added" << qtTrId("qtn_usb_sync_active");
    QTest::newRow("Mass Storage") << MeeGo::QmUSBMode::MassStorage << "device.added" << qtTrId("qtn_usb_storage_active");
    QTest::newRow("SDK") << MeeGo::QmUSBMode::SDK << "device.added" << qtTrId("qtn_usb_sdk_active");
    QTest::newRow("Developer") << MeeGo::QmUSBMode::Developer << "device.added" << qtTrId("qtn_usb_sdk_active");
    QTest::newRow("MTP") << MeeGo::QmUSBMode::MTP << "device.added" << qtTrId("qtn_usb_mtp_active");
    QTest::newRow("Adb") << MeeGo::QmUSBMode::Adb << "device.added" << qtTrId("qtn_usb_adb_active");
    QTest::newRow("Diag") << MeeGo::QmUSBMode::Diag << "device.added" << qtTrId("qtn_usb_diag_active");
    QTest::newRow("Cellular connection sharing") << MeeGo::QmUSBMode::ConnectionSharing << "device.added" << qtTrId("qtn_usb_connection_sharing_active");
}

void Ut_USBModeSelector::testUSBNotifications()
{
    QFETCH(MeeGo::QmUSBMode::Mode, mode);
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
    usbModeSelector->applyUSBMode(MeeGo::QmUSBMode::Connected);
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
    usbModeSelector->setUSBMode(5);
    QCOMPARE(testMode, (MeeGo::QmUSBMode::Mode)5);
}

void Ut_USBModeSelector::testSupportedUSBModes()
{
    QSignalSpy spy(usbModeSelector, SIGNAL(supportedUSBModesChanged()));
    testSupportedModes << MeeGo::QmUSBMode::OviSuite << MeeGo::QmUSBMode::MassStorage;
    usbModeSelector->applyCurrentUSBMode();
    QCOMPARE(usbModeSelector->supportedUSBModes().count(), testSupportedModes.count());
    for (int i = 0; i < testSupportedModes.count(); i++) {
        QCOMPARE(usbModeSelector->supportedUSBModes().at(i), (int)testSupportedModes.at(i));
    }
    QCOMPARE(spy.count(), 1);

    QList<MeeGo::QmUSBMode::Mode> modeList;
    modeList << MeeGo::QmUSBMode::MTP << MeeGo::QmUSBMode::Developer;
    usbModeSelector->updateSupportedUSBModeList(modeList);
    QCOMPARE(usbModeSelector->supportedUSBModes().count(), modeList.count());
    for (int i = 0; i < modeList.count(); i++) {
        QCOMPARE(usbModeSelector->supportedUSBModes().at(i), (int)modeList.at(i));
    }
    QCOMPARE(spy.count(), 2);
}

QTEST_MAIN (Ut_USBModeSelector)
