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
#include <QDBusPendingCallWatcher>

class MGConfItem;
class QTimer;

class DeviceLock : public QObject, protected QDBusContext
{
    Q_OBJECT
    Q_ENUMS(LockState)
    Q_PROPERTY(int state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(bool blankingPause READ blankingPause NOTIFY blankingPauseChanged)
    Q_PROPERTY(bool blankingInhibit READ blankingInhibit NOTIFY blankingInhibitChanged)

public:
    DeviceLock(QObject *parent = 0);

    enum LockState
    {
        Unlocked = 0,           /*!< Unlocked - The lock is unlocked */
        Locked,                 /*!< Locked - The lock is being used */
        Undefined               /*!< Undefined - The state of the lock is unknown */
    };

    Q_INVOKABLE int state() const;
    Q_INVOKABLE void setState(int state);

    Q_INVOKABLE bool checkCode(const QString &code);
    Q_INVOKABLE bool setCode(const QString &oldCode, const QString &newCode);
    Q_INVOKABLE bool isSet();
    bool blankingPause() const;
    bool blankingInhibit() const;

signals:
    void stateChanged(int state);
    void blankingPauseChanged();
    void blankingInhibitChanged();

private slots:
    void init();
    void setupLockTimer();
    void setStateAndSetupLockTimer();
    void lock();
    void checkDisplayState(MeeGo::QmDisplayState::DisplayState state);
    void handleCallStateChange(const QString &state, const QString &ignored);
    void handleBlankingPauseChange(const QString &state);
    void handleBlankingInhibitChange(const QString &state);
    void readSettings();
    void sendInhibitFinished(QDBusPendingCallWatcher *call);
    void sendPauseFinished(QDBusPendingCallWatcher *call);

private:
    static bool runPlugin(const QStringList &args);
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
    bool m_blankingPause;
    bool m_blankingInhibit;
    struct timeval monoTime;

#ifdef UNIT_TEST
    friend class Ut_DeviceLock;
#endif
};

#endif // LOCKSERVICE_H
