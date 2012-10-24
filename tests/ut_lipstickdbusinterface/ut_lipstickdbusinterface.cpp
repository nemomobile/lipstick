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

#include <QtTest/QtTest>
#include "ut_lipstickdbusinterface.h"
#include "lipstickdbusinterface.h"
#include "lipsticksettings.h"

LipstickSettings *LipstickSettings::instance()
{
    return 0;
}

bool lipstickSettingsLockScreenVisible = false;
bool lipstickSettingsExternallyChanged = false;
void LipstickSettings::setLockscreenVisible(bool lockscreenVisible, bool externallyChanged)
{
    lipstickSettingsLockScreenVisible = lockscreenVisible;
    lipstickSettingsExternallyChanged = externallyChanged;
}

bool LipstickSettings::lockscreenVisible() const
{
    return lipstickSettingsLockScreenVisible;
}

void Ut_LipstickDBusInterface::testShowAndHideLockScreen()
{
    QObject parent;
    LipstickDBusInterface interface(&parent);
    interface.showLockScreen();
    QCOMPARE(lipstickSettingsLockScreenVisible, true);
    QCOMPARE(lipstickSettingsExternallyChanged, true);
    interface.hideLockScreen();
    QCOMPARE(lipstickSettingsLockScreenVisible, false);
    QCOMPARE(lipstickSettingsExternallyChanged, true);
}

QTEST_MAIN(Ut_LipstickDBusInterface)
