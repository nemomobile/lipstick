
// This file is part of lipstick, a QML desktop library
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License version 2.1 as published by the Free Software Foundation
// and appearing in the file LICENSE.LGPL included in the packaging
// of this file.
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// Copyright (c) 2012, Robin Burchell <robin+nemo@viroteck.net>

#include <QDebug>

#include "lipstickdbusinterface.h"
#include "lipsticksettings.h"

// Define this if you'd like to see debug messages from the dbus interface class
#ifdef DEBUG_DBUSINTERFACE
#define DBUSINTERFACE_DEBUG(things) qDebug() << Q_FUNC_INFO << things
#else
#define DBUSINTERFACE_DEBUG(things)
#endif

LipstickDBusInterface::LipstickDBusInterface(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    LipstickSettings *settings = LipstickSettings::instance();
    connect(settings, SIGNAL(lockscreenVisibleChanged()), this, SLOT(lockscreenVisibilityChanged()));
}

void LipstickDBusInterface::showLockScreen()
{
    DBUSINTERFACE_DEBUG("Received signal to show lockscreen");

    LipstickSettings *settings = LipstickSettings::instance();
    settings->setLockscreenVisible(true, true);
}

void LipstickDBusInterface::hideLockScreen()
{
    DBUSINTERFACE_DEBUG("Received signal to hide lockscreen");

    LipstickSettings *settings = LipstickSettings::instance();
    settings->setLockscreenVisible(false, true);
}

void LipstickDBusInterface::lockscreenVisibilityChanged()
{
    LipstickSettings *settings = LipstickSettings::instance();

    if (!settings->lockscreenVisible())
    {
        DBUSINTERFACE_DEBUG("Lockscreen unlocked, emitting signal");

        emit screenUnlocked();
    }
}
