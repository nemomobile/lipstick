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

#ifndef DEVICELOCK_H
#define DEVICELOCK_H

#include <qmactivity.h>
#include <qmlocks.h>
#include <qmdisplaystate.h>
#include <sys/time.h>
#include <QFileSystemWatcher>
#include <QDBusContext>
#include "lipstickglobal.h"

class MGConfItem;
class QTimer;

class LIPSTICK_EXPORT DeviceLock : public QObject, protected QDBusContext
{
    Q_OBJECT

    Q_ENUMS(LockState)
    Q_ENUMS(ReturnValue)
    Q_PROPERTY(int state READ state WRITE setState NOTIFY stateChanged)

public:
    DeviceLock(QObject *parent = 0);

    enum LockState
    {
        Unlocked = 0,           /*!< Unlocked - The lock is unlocked */
        Locked,                 /*!< Locked - The lock is being used */
        Undefined               /*!< Undefined - The state of the lock is unknown */
    };

    /*!< note: encryption binary returns 0==OK and 1==Failed, switching is done in runPlugin for the QML */
    enum ReturnValue
    {
        Failed = 0,             /*!< Failed - syscall returned with default error */
        OK,                     /*!< OK - syscall returned without errors */
        Expired,                /*!< Expired - lockcode creation date is over the settings limit */
        InHistory               /*!< InHistory - lockcode was found in history file. */
    };

    Q_INVOKABLE int state() const;
    Q_INVOKABLE void setState(int state);

    Q_INVOKABLE int checkCode(const QString &code);
    Q_INVOKABLE int setCode(const QString &oldCode, const QString &newCode);
    Q_INVOKABLE bool isSet();

signals:
    void stateChanged(int state);

private slots:
    void init();
    void setupLockTimer();
    void setStateAndSetupLockTimer();
    void lock();
    void checkDisplayState(MeeGo::QmDisplayState::DisplayState state);
    void handleCallStateChange(const QString &state, const QString &ignored);
    void readSettings();

private:
    static int runPlugin(const QStringList &args);
    void setupTimer();
    bool isPrivileged();

    int lockingDelay;
    QFileSystemWatcher watcher;
    QTimer *lockTimer;
    MeeGo::QmActivity *qmActivity;
    MeeGo::QmLocks *qmLocks;
    MeeGo::QmDisplayState *qmDisplayState;
    LockState deviceLockState;
    bool isCallActive;
    struct timeval monoTime;

#ifdef UNIT_TEST
    friend class Ut_DeviceLock;
#endif
};

#endif // LOCKSERVICE_H
