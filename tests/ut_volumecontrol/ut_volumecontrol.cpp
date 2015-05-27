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
}

void Ut_VolumeControl::testConnections()
{
    // Check that pulse audio and the volume bar are connected
    QCOMPARE(disconnect(volumeControl->pulseAudioControl, SIGNAL(volumeChanged(int,int)), volumeControl, SLOT(setVolume(int,int))), true);
    QCOMPARE(disconnect(volumeControl->pulseAudioControl, SIGNAL(callActiveChanged(bool)), volumeControl, SLOT(handleCallActive(bool))), true);
}

Q_DECLARE_METATYPE(Qt::Key)
Q_DECLARE_METATYPE(QEvent::Type)

void Ut_VolumeControl::testEventFilter()
{
    int pressCount = 0;
    int releaseCount = 0;
    QSignalSpy volumeKeyPressedSpy(volumeControl, SIGNAL(volumeKeyPressed(int)));
    QSignalSpy volumeKeyReleasedSpy(volumeControl, SIGNAL(volumeKeyReleased(int)));

    // no reaction if resources not acquired
    QKeyEvent event(QEvent::KeyPress, Qt::Key_VolumeUp, 0, QString(), false);
    volumeControl->eventFilter(0, &event);
    QCOMPARE(volumeKeyPressedSpy.count(), pressCount);
    QCOMPARE(volumeKeyReleasedSpy.count(), releaseCount);

    volumeControl->hwKeyResourceAcquired();

    // normal press results in press signal
    volumeControl->eventFilter(0, &event);
    pressCount++;
    QCOMPARE(volumeKeyPressedSpy.count(), pressCount);
    QCOMPARE(volumeKeyReleasedSpy.count(), releaseCount);

    // autorepeat doesn't do anything
    event = QKeyEvent(QEvent::KeyPress, Qt::Key_VolumeUp, 0, QString(), true);
    volumeControl->eventFilter(0, &event);
    QCOMPARE(volumeKeyPressedSpy.count(), pressCount);
    QCOMPARE(volumeKeyReleasedSpy.count(), releaseCount);

    // release results in release signal
    event = QKeyEvent(QEvent::KeyRelease, Qt::Key_VolumeUp, 0, QString(), false);
    volumeControl->eventFilter(0, &event);
    releaseCount++;
    QCOMPARE(volumeKeyPressedSpy.count(), pressCount);
    QCOMPARE(volumeKeyReleasedSpy.count(), releaseCount);

    // other keys don't do anything
    event = QKeyEvent(QEvent::KeyPress, Qt::Key_Camera, 0, QString(), false);
    volumeControl->eventFilter(0, &event);
    event = QKeyEvent(QEvent::KeyRelease, Qt::Key_Camera, 0, QString(), false);
    volumeControl->eventFilter(0, &event);
    QCOMPARE(volumeKeyPressedSpy.count(), pressCount);
    QCOMPARE(volumeKeyReleasedSpy.count(), releaseCount);
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

    volumeControl->setVolume(2, 2);
    QCOMPARE(volumeChangedSpy.count(), 1);
    QCOMPARE(maximumVolumeChangedSpy.count(), 1);

    volumeControl->setVolume(2, 2);
    QCOMPARE(volumeChangedSpy.count(), 1);
    QCOMPARE(maximumVolumeChangedSpy.count(), 1);

    // trying to set volume above maximum gets ignored
    volumeControl->setVolume(3);
    QCOMPARE(volumeChangedSpy.count(), 1);
    QCOMPARE(maximumVolumeChangedSpy.count(), 1);
    QCOMPARE(volumeControl->volume(), 2);
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
