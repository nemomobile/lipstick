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

#include <dbus/dbus.h>
#include <policy/resource-set.h>
#include <linux/input.h>
#include <QGuiApplication>
#include "homewindow.h"
#include <QQmlContext>
#include <QScreen>
#include <QKeyEvent>
#include <MGConfItem>
#include "utilities/closeeventeater.h"
#include "pulseaudiocontrol.h"
#include "volumecontrol.h"
#include "lipstickqmlpath.h"

VolumeControl::VolumeControl(QObject *parent) :
    QObject(parent),
    window(0),
    pulseAudioControl(new PulseAudioControl(this)),
    hwKeyResource(new ResourcePolicy::ResourceSet("event")),
    hwKeysAcquired(false),
    volume_(0),
    maximumVolume_(0),
    audioWarning(new MGConfItem("/desktop/nemo/audiowarning", this)),
    safeVolume_(0),
    callActive_(false)
{
    hwKeyResource->setAlwaysReply();
    hwKeyResource->addResourceObject(new ResourcePolicy::ScaleButtonResource);
    connect(hwKeyResource, SIGNAL(resourcesGranted(QList<ResourcePolicy::ResourceType>)), this, SLOT(hwKeyResourceAcquired()));
    connect(hwKeyResource, SIGNAL(lostResources()), this, SLOT(hwKeyResourceLost()));

    // Set up key repeat: initial delay and per-repeat delay
    keyRepeatDelayTimer.setSingleShot(true);
    keyRepeatDelayTimer.setInterval(600);
    keyRepeatTimer.setInterval(75);
    connect(&keyRepeatDelayTimer, SIGNAL(timeout()), &keyRepeatTimer, SLOT(start()));
    connect(&keyRepeatTimer, SIGNAL(timeout()), this, SLOT(changeVolume()));

    connect(pulseAudioControl, SIGNAL(volumeChanged(int,int)), this, SLOT(setVolume(int,int)));
    connect(pulseAudioControl, SIGNAL(highVolume(int)), SLOT(handleHighVolume(int)));
    connect(pulseAudioControl, SIGNAL(longListeningTime(int)), SLOT(handleLongListeningTime(int)));
    connect(pulseAudioControl, SIGNAL(callActiveChanged(bool)), SLOT(handleCallActive(bool)));
    pulseAudioControl->update();

    qApp->installEventFilter(this);

    acquireKeys();
}

VolumeControl::~VolumeControl()
{
    hwKeyResource->deleteResource(ResourcePolicy::ScaleButtonType);
    delete window;
}

int VolumeControl::volume() const
{
    return volume_;
}

int VolumeControl::maximumVolume() const
{
    return maximumVolume_;
}

int VolumeControl::safeVolume() const
{
    return safeVolume_ == 0 ? maximumVolume() : safeVolume_;
}

void VolumeControl::setWindowVisible(bool visible)
{
    if (visible) {
        if (window == 0) {
            window = new HomeWindow();
            window->setGeometry(QRect(QPoint(), QGuiApplication::primaryScreen()->size()));
            window->setCategory(QLatin1String("notification"));
            window->setWindowTitle("Volume");
            window->setContextProperty("initialSize", QGuiApplication::primaryScreen()->size());
            window->setContextProperty("volumeControl", this);
            window->setSource(QmlPath::to("volumecontrol/VolumeControl.qml"));
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

bool VolumeControl::windowVisible() const
{
    return window != 0 && window->isVisible();
}

bool VolumeControl::warningAcknowledged() const
{
    return audioWarning->value(false).toBool();
}

void VolumeControl::setWarningAcknowledged(bool acknowledged)
{
    if (audioWarning->value(false).toBool() != acknowledged) {
        audioWarning->set(acknowledged);
    }
}

bool VolumeControl::callActive() const
{
    return callActive_;
}

void VolumeControl::setVolume(int volume, int maximumVolume)
{
    int clampedMaxVolume = maximumVolume < 0 ? 0 : maximumVolume;
    int clampedVolume = qBound(0, volume, maximumVolume);

    bool volumeUpdated = false;
    bool maxVolumeUpdated = false;

    if (maximumVolume_ != clampedMaxVolume) {
        maximumVolume_ = clampedMaxVolume;
        maxVolumeUpdated = true;
    }

    if (volume_ != clampedVolume) {
        volume_ = clampedVolume;
        volumeUpdated = true;
    }

    if (maxVolumeUpdated) {
        emit maximumVolumeChanged();
    }

    if (volumeUpdated) {
        emit volumeChanged();
    }
}

void VolumeControl::hwKeyResourceAcquired()
{
    hwKeysAcquired = true;
}

void VolumeControl::hwKeyResourceLost()
{
    hwKeysAcquired = false;
    stopKeyRepeat();
}

void VolumeControl::releaseKeys()
{
    hwKeyResource->release();
    stopKeyRepeat();
}

void VolumeControl::acquireKeys()
{
    hwKeyResource->acquire();
}

void VolumeControl::changeVolume()
{
    int newVolume = qBound(0, volume_ + volumeChange, warningAcknowledged() ? maximumVolume() : safeVolume());
    if (newVolume != volume_) {
        volume_ = newVolume;
        pulseAudioControl->setVolume(volume_);
        emit volumeChanged();
    }

    setWindowVisible(true);

    if (!warningAcknowledged() && safeVolume_ != 0 && volume_ >= safeVolume_) {
        emit showAudioWarning(false);
    }

    emit volumeKeyPressed();
}

void VolumeControl::stopKeyRepeat()
{
    keyRepeatDelayTimer.stop();
    keyRepeatTimer.stop();
}

void VolumeControl::handleHighVolume(int safeLevel)
{
    if (safeVolume_ != safeLevel) {
        safeVolume_ = safeLevel;
        emit safeVolumeChanged();
    }

    int newVolume = qBound(0, volume_, warningAcknowledged() ? maximumVolume() : safeVolume());
    if (newVolume != volume_) {
        pulseAudioControl->setVolume(volume_);
        emit volumeChanged();
    }

    if (!warningAcknowledged() && safeVolume_ != 0 && volume_ >= safeVolume_) {
        setWindowVisible(true);
        emit showAudioWarning(false);
    }
}

void VolumeControl::handleLongListeningTime(int listeningTime)
{
    setWarningAcknowledged(false);
    setWindowVisible(true);

    int newVolume = qBound(0, volume_, listeningTime == 0 ? maximumVolume() : safeVolume());
    if (newVolume != volume_) {
        volume_ = newVolume;
        pulseAudioControl->setVolume(volume_);
        emit volumeChanged();
    }

    emit showAudioWarning(listeningTime == 0);
}

void VolumeControl::handleCallActive(bool callActive)
{
    if (callActive_ != callActive) {
        callActive_ = callActive;
        emit callActiveChanged();
    }
}

bool VolumeControl::eventFilter(QObject *, QEvent *event)
{
    if (hwKeysAcquired && (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease)) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_VolumeUp || keyEvent->key() == Qt::Key_VolumeDown) {
            if (event->type() == QEvent::KeyPress) {
                // Key down: set which way to change the volume on each repeat, start the repeat delay timer and change the volume once
                volumeChange = keyEvent->key() == Qt::Key_VolumeUp ? 1 : -1;
                if (!keyRepeatDelayTimer.isActive() && !keyRepeatTimer.isActive()) {
                    keyRepeatDelayTimer.start();
                    changeVolume();
                }
            } else {
                // Key up: stop any key repeating in progress and the repeat delay timer
                stopKeyRepeat();
            }
            return true;
        }
    }
    return false;
}
