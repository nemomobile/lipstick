
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

LipstickDBusInterface::LipstickDBusInterface(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    LipstickSettings *settings = LipstickSettings::instance();
    connect(settings, SIGNAL(lockscreenVisibleChanged()), this, SLOT(lockscreenVisibilityChanged()));
}

void LipstickDBusInterface::showLockScreen()
{
    qDebug() << Q_FUNC_INFO << "Show lockscreen";

    LipstickSettings *settings = LipstickSettings::instance();
    settings->setLockscreenVisible(true);
}

void LipstickDBusInterface::hideLockScreen()
{
    qDebug() << Q_FUNC_INFO << "Hide lockscreen";
    LipstickSettings *settings = LipstickSettings::instance();
    settings->setLockscreenVisible(false);
}

void LipstickDBusInterface::lockscreenVisibilityChanged()
{
    LipstickSettings *settings = LipstickSettings::instance();
    if (!settings->lockscreenVisible())
        emit screenUnlocked();
}
