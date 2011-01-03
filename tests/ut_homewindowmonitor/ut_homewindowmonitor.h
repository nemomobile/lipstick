/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of mhome.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#ifndef UT_HOMEWINDOWMONITOR_H
#define UT_HOMEWINDOWMONITOR_H

#include <QObject>

#include "windowinfo.h"
class HomeWindowMonitor;
class MWindow;

class WindowStackingOrderListener : public QObject, public QList<QList<WindowInfo> >
{
    Q_OBJECT

public slots:
    void stackingOrderChanged(QList<WindowInfo> windowList);
};

class Ut_HomeWindowMonitor : public QObject
{
    Q_OBJECT

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    // Test cases
    void testWindowIdInWindowListIsRecognizedAsOwnWindow();
    void testWindowIdNotInWindowListIsNotRecognizedAsOwnWindow();

    void testReceivingNonStackingOrderXEventDoesNotEmitSignal();
    void testReceivingStackingOrderXEventEmitsStackingOrderSignal();
    void testReceivingStackingOrderXEventEmitsFullscreenWindowOnTopSignal();
    void testReceivingStackingOrderXEventEmitsAnyWindowOnTopSignal();

    void testNonFullscreenWindowOnTopDoesNotEmitFullscreenWindowOnTopSignal_data();
    void testNonFullscreenWindowOnTopDoesNotEmitFullscreenWindowOnTopSignal();

    void testNonFullscreenWindowOnTopEmitsAnyWindowOnTopSignal_data();
    void testNonFullscreenWindowOnTopEmitsAnyWindowOnTopSignal();

    void testOwnWindowOnTopDoesNotEmitFullscreenWindowOnTopSignal();
    void testOwnWindowOnTopDoesNotEmitAnyWindowOnTopSignal();

private:
    // The object being tested
    HomeWindowMonitor *m_subject;

    QList<MWindow*> windowList;

    XEvent clientListStackingEvent;
};

#endif
