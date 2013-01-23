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
#include <QApplication>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QDesktopWidget>
#include <QX11Info>
#include <X11/extensions/shape.h>
#include "utilities/closeeventeater.h"
#include "xtools/x11wrapper.h"
#include "pulseaudiocontrol.h"
#include "volumecontrol.h"
#include "volumekeylistener.h"

VolumeControl::VolumeControl(QObject *parent) :
    QObject(parent),
    window(0),
    pulseAudioControl(new PulseAudioControl(this)),
    hwKeyResource(new ResourcePolicy::ResourceSet("event")),
    hwKeys(new VolumeKeyListener(this)),
    volume_(0),
    maximumVolume_(0)
{
    hwKeyResource->setAlwaysReply();
    hwKeyResource->addResourceObject(new ResourcePolicy::ScaleButtonResource);
    connect(hwKeyResource, SIGNAL(resourcesGranted(QList<ResourcePolicy::ResourceType>)), this, SLOT(hwKeyResourceAcquired()));
    connect(hwKeyResource, SIGNAL(lostResources()), this, SLOT(hwKeyResourceLost()));

    // Set up key repeat: initial delay and per-repeat delay
    keyReleaseTimer.setSingleShot(true);
    keyReleaseTimer.setInterval(100);
    keyRepeatDelayTimer.setSingleShot(true);
    keyRepeatDelayTimer.setInterval(600);
    keyRepeatTimer.setInterval(75);
    connect(&keyReleaseTimer, SIGNAL(timeout()), this, SLOT(stopKeyRepeat()));
    connect(&keyRepeatDelayTimer, SIGNAL(timeout()), &keyRepeatTimer, SLOT(start()));
    connect(&keyRepeatTimer, SIGNAL(timeout()), this, SLOT(changeVolume()));

    connect(pulseAudioControl, SIGNAL(currentVolumeSet(int)), this, SLOT(setVolume(int)));
    connect(pulseAudioControl, SIGNAL(maximumVolumeSet(int)), this, SLOT(setMaximumVolume(int)));
    pulseAudioControl->update();

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

void VolumeControl::setWindowVisible(bool visible)
{
    if (visible) {
        if (window == 0) {
            window = new QDeclarativeView();
            window->setAttribute(Qt::WA_TranslucentBackground);
            window->setAttribute(Qt::WA_X11DoNotAcceptFocus);
            window->setAttribute(Qt::WA_X11NetWmWindowTypeNotification);
            window->setWindowTitle("Volume");
            window->setResizeMode(QDeclarativeView::SizeRootObjectToView);
            window->viewport()->setAutoFillBackground(false);
            window->rootContext()->setContextProperty("initialSize", QApplication::desktop()->screenGeometry(window).size());
            window->rootContext()->setContextProperty("volumeControl", this);
            window->setSource(QUrl("qrc:/qml/VolumeControl.qml"));
            window->installEventFilter(new CloseEventEater(this));

            Display *dpy = QX11Info::display();
            XserverRegion shapeRegion = X11Wrapper::XFixesCreateRegion(dpy, NULL, 0);
            X11Wrapper::XFixesSetWindowShapeRegion(dpy, window->winId(), ShapeInput, 0, 0, shapeRegion);
            X11Wrapper::XFixesDestroyRegion(dpy, shapeRegion);
            X11Wrapper::XSync(dpy, False);
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

void VolumeControl::setVolume(int volume)
{
    if (volume_ != volume) {
        volume_ = volume;
        emit volumeChanged();
    }
}

void VolumeControl::setMaximumVolume(int maximumVolume)
{
    if (maximumVolume_ != maximumVolume) {
        maximumVolume_ = maximumVolume;
        emit maximumVolumeChanged();
    }
}

void VolumeControl::hwKeyEvent(unsigned int key, int eventType)
{
    if (!(key == KEY_VOLUMEUP || key == KEY_VOLUMEDOWN)) {
        // Do nothing when a non-volume key is pressed
        return;
    }

    switch (eventType) {
    case KeyPress:
        // Key down: set which way to change the volume on each repeat, start the repeat delay timer and change the volume once
        volumeChange = key == KEY_VOLUMEUP ? 1 : -1;
        if (!keyRepeatDelayTimer.isActive() && !keyRepeatTimer.isActive()) {
            keyRepeatDelayTimer.start();
            changeVolume();
        }
        break;
    case KeyRelease:
        // Key up: stop any key repeating in progress
        keyReleaseTimer.start();
        break;
    default:
        break;
    }
}

void VolumeControl::hwKeyResourceAcquired()
{
    hwKeys->disconnect();
    connect(hwKeys, SIGNAL(keyEvent(uint, int)), this, SLOT(hwKeyEvent(uint, int)));
}

void VolumeControl::hwKeyResourceLost()
{
    hwKeys->disconnect();
    stopKeyRepeat();
}

void VolumeControl::releaseKeys()
{
    hwKeys->disconnect();
    hwKeyResource->release();
    stopKeyRepeat();
}

void VolumeControl::acquireKeys()
{
    hwKeyResource->acquire();
}

void VolumeControl::changeVolume()
{
    volume_ += volumeChange;
    if (volume_ < 0) {
        volume_ = 0;
    } else if (volume_ >= maximumVolume_) {
        volume_ = maximumVolume_;
    }

    pulseAudioControl->setVolume(volume_);
    setWindowVisible(true);
    emit volumeChanged();
}

void VolumeControl::stopKeyRepeat()
{
    keyRepeatDelayTimer.stop();
    keyRepeatTimer.stop();
}
