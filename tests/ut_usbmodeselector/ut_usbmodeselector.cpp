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
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QDesktopWidget>
#include <usbmodeselector.h>

#include "ut_usbmodeselector.h"
#include "qmlocks_stub.h"
#include "qmusbmode_stub.h"
#include "notificationmanager_stub.h"
#include "closeeventeater_stub.h"

QList<QDeclarativeView *> qDeclarativeViews;
void QDeclarativeView::setSource(const QUrl &)
{
    qDeclarativeViews.append(this);
}

QHash<QWidget *, bool> qWidgetVisible;
void QWidget::setVisible(bool visible)
{
    setAttribute(Qt::WA_WState_Visible, visible);
    qWidgetVisible[this] = visible;
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
    qDeclarativeViews.clear();
    qWidgetVisible.clear();
    gNotificationManagerStub->stubReset();
}

void Ut_USBModeSelector::testConnections()
{
    QCOMPARE(disconnect(usbModeSelector->usbMode, SIGNAL(modeChanged(MeeGo::QmUSBMode::Mode)), usbModeSelector, SLOT(applyUSBMode(MeeGo::QmUSBMode::Mode))), true);
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

    QCOMPARE(qDeclarativeViews.count(), 1);

    // Check window properties
    QCOMPARE(qDeclarativeViews.first()->testAttribute(Qt::WA_TranslucentBackground), true);
    QCOMPARE(qDeclarativeViews.first()->testAttribute(Qt::WA_X11DoNotAcceptFocus), true);
    QCOMPARE(qDeclarativeViews.first()->testAttribute(Qt::WA_X11NetWmWindowTypeMenu), true);
    QCOMPARE(qDeclarativeViews.first()->windowTitle(), QString("USB Mode"));
    QCOMPARE(qDeclarativeViews.first()->resizeMode(), QDeclarativeView::SizeRootObjectToView);
    QCOMPARE(qDeclarativeViews.first()->viewport()->autoFillBackground(), false);
    QCOMPARE(qDeclarativeViews.first()->rootContext()->contextProperty("initialSize").toSize(), QApplication::desktop()->screenGeometry(qDeclarativeViews.first()).size());
    QCOMPARE(qDeclarativeViews.first()->rootContext()->contextProperty("usbModeSelector"), QVariant::fromValue(static_cast<QObject *>(usbModeSelector)));

    // Check that the window was shown
    QCOMPARE(qWidgetVisible[static_cast<QWidget *>(qDeclarativeViews.first())], true);
    QCOMPARE(spy.count(), 1);
}

void Ut_USBModeSelector::testHideDialog_data()
{
    QTest::addColumn<MeeGo::QmUSBMode::Mode>("mode");

    QTest::newRow("Disconnected") << MeeGo::QmUSBMode::Disconnected;
    QTest::newRow("Ovi Suite") << MeeGo::QmUSBMode::OviSuite;
    QTest::newRow("Mass Storage") << MeeGo::QmUSBMode::MassStorage;
    QTest::newRow("SDK") << MeeGo::QmUSBMode::SDK;
}

void Ut_USBModeSelector::testHideDialog()
{
    QFETCH(MeeGo::QmUSBMode::Mode, mode);

    usbModeSelector->usbMode->setDefaultMode(MeeGo::QmUSBMode::Ask);
    usbModeSelector->applyUSBMode(MeeGo::QmUSBMode::Ask);
    usbModeSelector->applyUSBMode(mode);
    QCOMPARE(qWidgetVisible[static_cast<QWidget *>(qDeclarativeViews.first())], false);
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

QTEST_MAIN (Ut_USBModeSelector)
