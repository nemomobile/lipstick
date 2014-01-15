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

#include <QCoreApplication>
#include <QSettings>
#include <QProcess>
#include <QTimer>
#include <QDebug>
#include "devicelock.h"
#include <sys/time.h>
#include <QDBusConnection>
#include <QFile>

namespace {
const char * const settingsFile = "/usr/share/lipstick/devicelock/devicelock_settings.conf";
const char * const lockingKey = "/desktop/nemo/devicelock/automatic_locking";
}
/* ------------------------------------------------------------------------- *
 * struct timeval helpers
 * ------------------------------------------------------------------------- */
static void tv_get_monotime(struct timeval *tv)
{
#if defined(CLOCK_BOOTTIME) 
  struct timespec ts;
  if (clock_gettime(CLOCK_BOOTTIME, &ts) < 0)
      if (clock_gettime(CLOCK_MONOTONIC, &ts) < 0)
          qFatal("Can't clock_gettime!");
  TIMESPEC_TO_TIMEVAL(tv, &ts);
#endif
}

static int tv_diff_in_s(const struct timeval *tv1, const struct timeval *tv2)
{
    struct timeval tv;
    timersub(tv1, tv2, &tv);
    return tv.tv_sec;
}

DeviceLock::DeviceLock(QObject * parent) :
    QObject(parent),
    lockingDelay(-1),
    lockTimer(new QTimer(this)),
    qmActivity(new MeeGo::QmActivity(this)),
    qmLocks(new MeeGo::QmLocks(this)),
    qmDisplayState(new MeeGo::QmDisplayState(this)),
    deviceLockState(Undefined),
    isCallActive(false)
{
    monoTime.tv_sec = 0;
    connect(lockTimer, SIGNAL(timeout()), this, SLOT(lock()));
    connect(qmActivity, SIGNAL(activityChanged(MeeGo::QmActivity::Activity)), this, SLOT(setStateAndSetupLockTimer()));
    connect(qmLocks, SIGNAL(stateChanged(MeeGo::QmLocks::Lock,MeeGo::QmLocks::State)), this, SLOT(setStateAndSetupLockTimer()));
    connect(qmDisplayState, SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState)), this, SLOT(checkDisplayState(MeeGo::QmDisplayState::DisplayState)));
    connect(qApp, SIGNAL(homeReady()), this, SLOT(init()));

    QDBusConnection::systemBus().connect(QString(), "/com/nokia/mce/signal", "com.nokia.mce.signal", "sig_call_state_ind", this, SLOT(handleCallStateChange(QString, QString)));
}

void DeviceLock::handleCallStateChange(const QString &state, const QString &ignored)
{
    Q_UNUSED(ignored);
    if (state.contains("active")) {
        isCallActive = true;
    } else {
        isCallActive = false;
    }
}

void DeviceLock::init()
{
    if (QFile(settingsFile).exists() && watcher.addPath(settingsFile)) {
        readSettings();
        connect(&watcher, SIGNAL(fileChanged(QString)), this, SLOT(readSettings()));
    }

    setState(isSet() && lockingDelay >= 0 ? Locked : Unlocked);
}

void DeviceLock::setupLockTimer()
{
    if (deviceLockState == Locked) {
        // Device already locked: stop the timer
        lockTimer->stop();
        monoTime.tv_sec = 0;
    } else {
        if (lockingDelay <= 0 || (qmActivity->get() == MeeGo::QmActivity::Active && qmDisplayState->get() != MeeGo::QmDisplayState::DisplayState::Off)) {
            // Locking disabled or device active: stop the timer
            lockTimer->stop();
            monoTime.tv_sec = 0;
        } else if (!isCallActive && !monoTime.tv_sec) {
            // Locking in N minutes enabled and device inactive: start the timer
            lockTimer->start(lockingDelay * 60 * 1000);
            tv_get_monotime(&monoTime);
        }
    }
}

void DeviceLock::setStateAndSetupLockTimer()
{
    if (lockingDelay < 0) {
        // Locking disabled: unlock
        setState(Unlocked);
    }
    setupLockTimer();
}

void DeviceLock::checkDisplayState(MeeGo::QmDisplayState::DisplayState state)
{
    if (lockingDelay == 0 && state == MeeGo::QmDisplayState::DisplayState::Off
            && qmLocks->getState(MeeGo::QmLocks::TouchAndKeyboard) == MeeGo::QmLocks::Locked
            && !isCallActive) {
        // Immediate locking enabled and the display is off and not in call: lock
        setState(Locked);
    } else if (state == MeeGo::QmDisplayState::DisplayState::Off) {
        setStateAndSetupLockTimer();
    } else if (monoTime.tv_sec && lockingDelay > 0) {
        struct timeval compareTime;
        tv_get_monotime(&compareTime);
        if (lockingDelay*60 < tv_diff_in_s(&compareTime, &monoTime)) {
            setState(Locked);
        }
    }
}

void DeviceLock::lock()
{
    setState(Locked);
}

int DeviceLock::state() const
{
    return deviceLockState;
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

void DeviceLock::readSettings()
{
    QSettings settings(settingsFile, QSettings::IniFormat);
    lockingDelay = settings.value(lockingKey, "-1").toInt();
    if (deviceLockState != Undefined) {
        setStateAndSetupLockTimer();
    }
}
