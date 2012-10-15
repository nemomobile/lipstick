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
#ifndef UT_STATUSINDICATOR_H
#define UT_STATUSINDICATOR_H

#include <QObject>

class Ut_StatusIndicator : public QObject
{
    Q_OBJECT

private slots:
    void testConstruction();
    void testGettersAndSetters();
    void testPropertyNotificationSignals_data();
    void testPropertyNotificationSignals();
    void testSettingInitialValueIndexSetsValueAndStaysWithinBounds();
    void testSettingAnimationDurationSetsTimelineState();
    void testAnimationIsDisabledWhenNotOnDisplay();
    void testAnimationIsDisabledWhenNotEnoughValuesToAnimate();
    void testSettingInitialValueIndexSetsAnimationLength();
    void testContextPropertyCreation();
    void testSettingIsOnDisplaySubscribesContextProperties_data();
    void testSettingIsOnDisplaySubscribesContextProperties();
};

#endif
