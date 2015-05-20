/***************************************************************************
**
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

#include <linux/input.h>
#include <QtTest/QtTest>
#include <QDBusConnection>
#include "ut_volumecontrol.h"
#include "volumecontrol.h"
#include "pulseaudiocontrol_stub.h"
#include "closeeventeater_stub.h"
#include "mgconfitem_stub.h"
#include "lipstickqmlpath_stub.h"

extern "C"
{
#include <dbus/dbus.h>
#include <dbus/dbus-glib-lowlevel.h>
}

#include <policy/resource-set.h>
/*********************************************************************************
 * Stub for ResourcePolicy
 */

bool acquireCalled = false;

namespace ResourcePolicy
{
    ResourceSet::ResourceSet(const QString&, QObject*)
    {
    }

    bool ResourceSet::setAlwaysReply()
    {
        return true;
    }

    ScaleButtonResource::ScaleButtonResource()
    {
    }

    void ResourceSet::addResourceObject(ResourcePolicy::Resource* rsc)
    {
        Q_UNUSED(rsc);
        return;
    }

    bool ResourceSet::acquire()
    {
        acquireCalled = true;
        return true;
    }

    bool ResourceSet::release()
    {
        return true;
    }

    void ResourceSet::deleteResource(ResourcePolicy::ResourceType)
    {
        return;
    }
}

void QObject::installEventFilter(QObject *filterObj)
{
    Q_UNUSED(filterObj)
}

QMap<QTimer *, int> qTimerStartCounts;
void QTimer::start()
{
    id = 1;
    qTimerStartCounts[this]++;
}

QMap<QTimer *, int> qTimerStopCounts;
void QTimer::stop()
{
    id = -1;
    qTimerStopCounts[this]++;
}

QHash<QWindow *, bool> qWindowVisible;
void QWindow::show()
{
    qWindowVisible[this] = true;
}
void QWindow::hide()
{
    qWindowVisible[this] = false;
}

void Ut_VolumeControl::initTestCase()
{
}

void Ut_VolumeControl::cleanupTestCase()
{
}

void Ut_VolumeControl::init()
{
    gPulseAudioControlStub->stubReset();

    volumeControl = new VolumeControl;
    volumeControl->setVolume(5, 10);
}

void Ut_VolumeControl::cleanup()
{
    delete volumeControl;
    acquireCalled = false;
    qTimerStartCounts.clear();
    qTimerStopCounts.clear();
    disconnect(SIGNAL(timeout()));
}

void Ut_VolumeControl::testConnections()
{
    // Check that pulse audio and the volume bar are connected
    QCOMPARE(disconnect(volumeControl->pulseAudioControl, SIGNAL(volumeChanged(int,int)), volumeControl, SLOT(setVolume(int,int))), true);
    QCOMPARE(disconnect(volumeControl->pulseAudioControl, SIGNAL(callActiveChanged(bool)), volumeControl, SLOT(handleCallActive(bool))), true);
}

void Ut_VolumeControl::testKeyRepeatSetup()
{
    QCOMPARE(volumeControl->keyRepeatDelayTimer.interval(), 600);
    QCOMPARE(volumeControl->keyRepeatDelayTimer.isSingleShot(), true);
    QCOMPARE(volumeControl->keyRepeatTimer.interval(), 75);
    QCOMPARE(disconnect(&volumeControl->keyRepeatDelayTimer, SIGNAL(timeout()), &volumeControl->keyRepeatTimer, SLOT(start())), true);
    QCOMPARE(disconnect(&volumeControl->keyRepeatTimer, SIGNAL(timeout()), volumeControl, SLOT(changeVolume())), true);
}

Q_DECLARE_METATYPE(Qt::Key)
Q_DECLARE_METATYPE(QEvent::Type)

void Ut_VolumeControl::testEventFilter_data()
{
    QTest::addColumn<bool>("hwKeysAcquired");
    QTest::addColumn<Qt::Key>("key");
    QTest::addColumn<QEvent::Type>("type");
    QTest::addColumn<int>("volumeChangedCount");
    QTest::addColumn<int>("volumeKeyPressedCount");
    QTest::addColumn<int>("volume");
    QTest::addColumn<bool>("keyRepeatDelayTimerActive");

    QTest::newRow("Pressing 'volume up' should produce a '+1' change request and start the delay timer") << true << Qt::Key_VolumeUp << QEvent::KeyPress << 1 << 1 << 6 << true;
    QTest::newRow("Pressing 'volume down' should produce a '-1' change request and start the delay timer") << true << Qt::Key_VolumeDown << QEvent::KeyPress << 1 << 1 << 4 << true;
    QTest::newRow("Pressing any other key should produce no change request") << true << Qt::Key_Camera << QEvent::KeyPress << 0 << 0 << 0 << false;
    QTest::newRow("Releasing a key should produce no change request") << true << Qt::Key_VolumeUp << QEvent::KeyRelease << 0 << 0 << 0 << false;
    QTest::newRow("When keys are not acquired pressing should do nothing") << false << Qt::Key_VolumeUp << QEvent::KeyPress << 0 << 0 << 0 << false;
}

void Ut_VolumeControl::testEventFilter()
{
    QFETCH(bool, hwKeysAcquired);
    QFETCH(Qt::Key, key);
    QFETCH(QEvent::Type, type);
    QFETCH(int, volumeChangedCount);
    QFETCH(int, volumeKeyPressedCount);
    QFETCH(int, volume);
    QFETCH(bool, keyRepeatDelayTimerActive);

    if (hwKeysAcquired) {
        volumeControl->hwKeyResourceAcquired();
    }

    QSignalSpy volumeChangedSpy(volumeControl, SIGNAL(volumeChanged()));
    QSignalSpy volumeKeyPressedSpy(volumeControl, SIGNAL(volumeKeyPressed()));
    QKeyEvent event(type, key, 0);
    volumeControl->eventFilter(0, &event);

    QCOMPARE(volumeChangedSpy.count(), volumeChangedCount);
    QCOMPARE(volumeKeyPressedSpy.count(), volumeKeyPressedCount);
    if (volumeChangedCount > 0) {
        QCOMPARE(volumeControl->volume(), volume);
    }
    QCOMPARE(qTimerStartCounts.value(&volumeControl->keyRepeatDelayTimer), keyRepeatDelayTimerActive ? 1 : 0);
}

void Ut_VolumeControl::testHwKeyEventWhenKeyRepeatDelayIsInProgress()
{
    volumeControl->hwKeyResourceAcquired();

    QSignalSpy volumeChangedSpy(volumeControl, SIGNAL(volumeChanged()));
    QSignalSpy volumeKeyPressedSpy(volumeControl, SIGNAL(volumeKeyPressed()));

    QKeyEvent upEvent(QEvent::KeyPress, Qt::Key_VolumeUp, 0);
    volumeControl->eventFilter(0, &upEvent);
    QCOMPARE(qTimerStartCounts.value(&volumeControl->keyRepeatDelayTimer), 1);
    QCOMPARE(qTimerStartCounts.value(&volumeControl->keyRepeatTimer), 0);
    QCOMPARE(volumeChangedSpy.count(), 1);
    QCOMPARE(volumeKeyPressedSpy.count(), 1);

    // Only the first press should cause the timer to be started and the volume change request to be made
    QKeyEvent downEvent(QEvent::KeyPress, Qt::Key_VolumeUp, 0);
    volumeControl->eventFilter(0, &downEvent);
    QCOMPARE(qTimerStartCounts.value(&volumeControl->keyRepeatDelayTimer), 1);
    QCOMPARE(qTimerStartCounts.value(&volumeControl->keyRepeatTimer), 0);
    QCOMPARE(volumeChangedSpy.count(), 1);
    QCOMPARE(volumeKeyPressedSpy.count(), 1);

    // A key repeat should use the new volume change
    volumeControl->changeVolume();
    QCOMPARE(volumeChangedSpy.count(), 2);
    QCOMPARE(volumeKeyPressedSpy.count(), 2);
}

void Ut_VolumeControl::testHwKeyEventWhenKeyRepeatIsInProgress()
{
    volumeControl->hwKeyResourceAcquired();

    QSignalSpy volumeChangedSpy(volumeControl, SIGNAL(volumeChanged()));
    QSignalSpy volumeKeyPressedSpy(volumeControl, SIGNAL(volumeKeyPressed()));

    // Start the key repeat timer by causing a timeout in the key repeat delay timer
    connect(this, SIGNAL(timeout()), &volumeControl->keyRepeatDelayTimer, SIGNAL(timeout()));
    emit timeout();
    // TODO figure out why the signal above won't do the trick on Qt5
    volumeControl->keyRepeatTimer.start();
    qApp->processEvents();
    QCOMPARE(qTimerStartCounts.value(&volumeControl->keyRepeatTimer), 1);

    // Further presses should not cause the timer to be started and the volume change request to be made
    QKeyEvent event(QEvent::KeyPress, Qt::Key_VolumeDown, 0);
    volumeControl->eventFilter(0, &event);
    QCOMPARE(qTimerStartCounts.value(&volumeControl->keyRepeatDelayTimer), 0);
    QCOMPARE(qTimerStartCounts.value(&volumeControl->keyRepeatTimer), 1);
    QCOMPARE(volumeChangedSpy.count(), 0);
    QCOMPARE(volumeKeyPressedSpy.count(), 0);
}

void Ut_VolumeControl::testHwKeyEventWhenKeyReleaseIsInProgress()
{
    volumeControl->hwKeyResourceAcquired();

    QSignalSpy volumeChangedSpy(volumeControl, SIGNAL(volumeChanged()));
    QSignalSpy volumeKeyPressedSpy(volumeControl, SIGNAL(volumeKeyPressed()));

    // Start the key repeat timer by causing a timeout in the key repeat delay timer
    connect(this, SIGNAL(timeout()), &volumeControl->keyRepeatDelayTimer, SIGNAL(timeout()));
    emit timeout();
    // TODO figure out why the signal above won't do the trick on Qt5
    volumeControl->keyRepeatTimer.start();
    disconnect(this, SIGNAL(timeout()), &volumeControl->keyRepeatDelayTimer, SIGNAL(timeout()));

    // Key release should stop the repeat timer and the delay timer
    QKeyEvent event(QEvent::KeyRelease, Qt::Key_VolumeDown, 0);
    volumeControl->eventFilter(0, &event);
    QCOMPARE(qTimerStartCounts.value(&volumeControl->keyRepeatDelayTimer), 0);
    QCOMPARE(qTimerStartCounts.value(&volumeControl->keyRepeatTimer), 1);
    QCOMPARE(qTimerStopCounts.value(&volumeControl->keyRepeatDelayTimer), 1);
    QCOMPARE(qTimerStopCounts.value(&volumeControl->keyRepeatTimer), 1);
    QCOMPARE(volumeChangedSpy.count(), 0);
    QCOMPARE(volumeKeyPressedSpy.count(), 0);
}

void Ut_VolumeControl::testAcquireKeys()
{
    // ctor should aqcuire
    QCOMPARE(acquireCalled, true);
}

void Ut_VolumeControl::testMaximumVolume()
{
    volumeControl->hwKeyResourceAcquired();

    QSignalSpy volumeChangedSpy(volumeControl, SIGNAL(volumeChanged()));
    QSignalSpy maximumVolumeChangedSpy(volumeControl, SIGNAL(maximumVolumeChanged()));
    QSignalSpy volumeKeyPressedSpy(volumeControl, SIGNAL(volumeKeyPressed()));

    volumeControl->setVolume(2, 2);
    QCOMPARE(volumeChangedSpy.count(), 1);
    QCOMPARE(maximumVolumeChangedSpy.count(), 1);
    QCOMPARE(volumeKeyPressedSpy.count(), 0);

    volumeControl->setVolume(2, 2);
    QCOMPARE(volumeChangedSpy.count(), 1);
    QCOMPARE(maximumVolumeChangedSpy.count(), 1);
    QCOMPARE(volumeKeyPressedSpy.count(), 0);

    QKeyEvent upEvent(QEvent::KeyPress, Qt::Key_VolumeUp, 0);
    volumeControl->eventFilter(0, &upEvent);
    QCOMPARE(volumeChangedSpy.count(), 1);
    QCOMPARE(maximumVolumeChangedSpy.count(), 1);
    QCOMPARE(volumeKeyPressedSpy.count(), 1);
}

void Ut_VolumeControl::testCallActive()
{
    QSignalSpy signalSpy(volumeControl, SIGNAL(callActiveChanged()));

    volumeControl->handleCallActive(true);
    QCOMPARE(signalSpy.count(), 1);
    QCOMPARE(volumeControl->callActive(), true);

    volumeControl->handleCallActive(true);
    QCOMPARE(signalSpy.count(), 1);

    volumeControl->handleCallActive(false);
    QCOMPARE(signalSpy.count(), 2);
    QCOMPARE(volumeControl->callActive(), false);
}

QTEST_MAIN(Ut_VolumeControl)
