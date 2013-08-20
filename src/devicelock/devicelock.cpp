/***************************************************************************
**
** Copyright (C) 2013 Jolla Ltd.
** Contact: Jonni Rainisto <jonni.rainisto@jollamobile.com>
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

#include <QSettings>
#include <QProcess>
#include <QTimer>
#include <MGConfItem>
#include <QDebug>
#include "devicelock.h"

DeviceLock::DeviceLock(QObject * parent) :
    QObject(parent),
    lockingGConfItem(new MGConfItem("/desktop/nemo/devicelock/automatic_locking", this)),
    peekingGConfItem(new MGConfItem("/desktop/nemo/devicelock/peeking_allowed", this)),
    lockTimer(new QTimer(this)),
    qmActivity(new MeeGo::QmActivity(this)),
    qmLocks(new MeeGo::QmLocks(this)),
    deviceLockState(Undefined)
{
    connect(lockingGConfItem, SIGNAL(valueChanged()), this, SLOT(setStateAndSetupLockTimer()));
    connect(peekingGConfItem, SIGNAL(valueChanged()), this, SLOT(updatePeek()));
    connect(lockTimer, SIGNAL(timeout()), this, SLOT(lock()));
    connect(qmActivity, SIGNAL(activityChanged(MeeGo::QmActivity::Activity)), this, SLOT(setStateAndSetupLockTimer()));
    connect(qmLocks, SIGNAL(stateChanged(MeeGo::QmLocks::Lock,MeeGo::QmLocks::State)), this, SLOT(setStateAndSetupLockTimer()));

    setState(isSet() && lockingGConfItem->value(-1).toInt() >= 0 ? Locked : Unlocked);
    updatePeek();
}

void DeviceLock::setupLockTimer()
{
    if (deviceLockState == Locked) {
        // Device already locked: stop the timer
        lockTimer->stop();
    } else {
        int lockingDelay = lockingGConfItem->value(-1).toInt();
        if (lockingDelay <= 0 || qmActivity->get() == MeeGo::QmActivity::Active) {
            // Locking disabled or device active: stop the timer
            lockTimer->stop();
        } else {
            // Locking in N minutes enabled and device inactive: start the timer
            lockTimer->start(lockingDelay * 60 * 1000);
        }
    }
}

void DeviceLock::setStateAndSetupLockTimer()
{
    int lockingDelay = lockingGConfItem->value(-1).toInt();
    if (lockingDelay < 0) {
        // Locking disabled: unlock
        setState(Unlocked);
    } else if (lockingDelay == 0 && qmLocks->getState(MeeGo::QmLocks::TouchAndKeyboard) == MeeGo::QmLocks::Locked) {
        // Immediate locking enabled and the touch screen is locked: lock
        setState(Locked);
    }
    setupLockTimer();
}

void DeviceLock::updatePeek()
{
    peekAllowed = peekingGConfItem->value(-1).toInt();
    emit peekChanged(peekAllowed);
}

void DeviceLock::lock()
{
    setState(Locked);
}

int DeviceLock::state() const
{
    return deviceLockState;
}

int DeviceLock::peek() const
{
    return peekAllowed;
}


void DeviceLock::setState(int state)
{
    if (deviceLockState != (LockState)state) {
        deviceLockState = (LockState)state;
        emit stateChanged(state);

        setupLockTimer();
    }
}

bool DeviceLock::checkCode(const QString &code)
{
    return runPlugin(QStringList() << "--check-code" << code);
}

bool DeviceLock::setCode(const QString &oldCode, const QString &newCode)
{
    return runPlugin(QStringList() << "--set-code" << oldCode << newCode);
}

bool DeviceLock::isSet() {
    return runPlugin(QStringList() << "--is-set" << "lockcode");
}

bool DeviceLock::runPlugin(const QStringList &args)
{
    QSettings settings("/usr/share/lipstick/devicelock/devicelock.conf", QSettings::IniFormat);
    QString pluginName = settings.value("DeviceLock/pluginName").toString();

    if (pluginName.isEmpty()) {
        qWarning("No plugin configuration set in /usr/share/lipstick/devicelock/devicelock.conf");
        return false;
    }

    QProcess process;
    process.start(pluginName, args);
    if (!process.waitForFinished()) {
        qWarning("Plugin did not finish in time");
        return false;
    }

    qDebug() << process.readAllStandardOutput();
    qWarning() << process.readAllStandardError();
    return process.exitCode() == 0;
}
