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

#include <MApplication>
#include <MPannableWidget>
#include "panningwatcher.h"
#include "ut_panningwatcher.h"

QPointF gPannableWidgetPosition;
QPointF MPannableWidget::position() const
{
    return gPannableWidgetPosition;
}

bool gTimerStarted = false;
void QTimer::start(int)
{
    gTimerStarted = true;
}

void Ut_PanningWatcher::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_panningwatcher";
    app = new MApplication(argc, &app_name);
}

void Ut_PanningWatcher::cleanupTestCase()
{
    delete app;
}

void Ut_PanningWatcher::init()
{
    pannableWidget = new MPannableWidget();
    m_subject = new PanningWatcher(*pannableWidget);
    gPannableWidgetPosition = QPointF();
    gTimerStarted = false;
}

void Ut_PanningWatcher::cleanup()
{
    delete m_subject;
    delete pannableWidget;
}

void Ut_PanningWatcher::testInitialState()
{
    QSignalSpy panningStateSpy(m_subject, SIGNAL(panningStateChanged(bool)));
    m_subject->updatePanningState();
    QCOMPARE(panningStateSpy.count(), 0);

    QVERIFY(disconnect(&m_subject->pannableWidget, SIGNAL(positionChanged(QPointF)), m_subject, SLOT(updatePanningState())));
    QVERIFY(disconnect(&m_subject->movementDetectorTimer, SIGNAL(timeout()), m_subject, SLOT(updatePanningState())));
}

void Ut_PanningWatcher::testWhenPannableWidgetMovesThenStateSignalsAreSent()
{
    QSignalSpy panningStateSpy(m_subject, SIGNAL(panningStateChanged(bool)));

    gPannableWidgetPosition = QPointF(0, 10);
    m_subject->updatePanningState();
    QCOMPARE(panningStateSpy.count(), 1);
    QVERIFY(panningStateSpy.at(0).at(0).toBool());
    QVERIFY(gTimerStarted);

    panningStateSpy.clear();
    gTimerStarted = false;

    gPannableWidgetPosition = QPointF(0, 15);
    m_subject->updatePanningState();
    QVERIFY(gTimerStarted);
    QCOMPARE(panningStateSpy.count(), 0);

    gTimerStarted = false;

    gPannableWidgetPosition = QPointF(0, 15);
    m_subject->updatePanningState();
    QCOMPARE(panningStateSpy.count(), 1);
    QVERIFY(!panningStateSpy.at(0).at(0).toBool());
    QVERIFY(!gTimerStarted);
}

QTEST_APPLESS_MAIN(Ut_PanningWatcher)
