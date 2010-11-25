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
    gPannableWidgetPosition = QPointF();
    gTimerStarted = false;
    pannableWidget = new MPannableWidget();
    m_subject = new PanningWatcher(*pannableWidget);
}

void Ut_PanningWatcher::cleanup()
{
    delete m_subject;
    delete pannableWidget;
}

void Ut_PanningWatcher::testInitialState()
{
    QSignalSpy panningStateSpy(m_subject, SIGNAL(panningStateChanged(bool)));
    m_subject->enablePanningIfPositionChanged();
    QCOMPARE(panningStateSpy.count(), 0);

    QVERIFY(disconnect(&m_subject->pannableWidget, SIGNAL(positionChanged(QPointF)), m_subject, SLOT(enablePanningIfPositionChanged())));
    QVERIFY(disconnect(&m_subject->movementDetectorTimer, SIGNAL(timeout()), m_subject, SLOT(disablePanningIfPositionNotChanged())));
}

void Ut_PanningWatcher::testStateSignalIsSentWhenPannableWidgetStartsMoving()
{
    QSignalSpy panningStateSpy(m_subject, SIGNAL(panningStateChanged(bool)));

    gPannableWidgetPosition = QPointF(0, 10);
    m_subject->enablePanningIfPositionChanged();
    QCOMPARE(panningStateSpy.count(), 1);
    QVERIFY(panningStateSpy.at(0).at(0).toBool());
    QVERIFY(gTimerStarted);
}

void Ut_PanningWatcher::testStateSignalIsNotSentWhenPannableWidgetMovesWhenAlreadyPanning()
{
    QSignalSpy panningStateSpy(m_subject, SIGNAL(panningStateChanged(bool)));

    gPannableWidgetPosition = QPointF(0, 10);
    m_subject->enablePanningIfPositionChanged();
    panningStateSpy.clear();
    gTimerStarted = false;

    // Position change when already panning should not cause signaling but timer should be restarted
    gPannableWidgetPosition = QPointF(0, 15);
    m_subject->enablePanningIfPositionChanged();
    QVERIFY(gTimerStarted);
    QCOMPARE(panningStateSpy.count(), 0);
}

void Ut_PanningWatcher::testStateSignalIsNotSentWhenPositionChangeReceivedButPannableWidgetDoesNotMove()
{
    QSignalSpy panningStateSpy(m_subject, SIGNAL(panningStateChanged(bool)));

    gPannableWidgetPosition = QPointF(0, 10);
    m_subject->enablePanningIfPositionChanged();
    panningStateSpy.clear();
    gTimerStarted = false;

    // Position change signal without an actual position change should not cause the panning to be considered stopped
    m_subject->enablePanningIfPositionChanged();
    QCOMPARE(panningStateSpy.count(), 0);
    QVERIFY(!gTimerStarted);
}

void Ut_PanningWatcher::testStateSignalIsSentWhenMovementTimeoutOccursAndPannableWidgetDoesNotMove()
{
    QSignalSpy panningStateSpy(m_subject, SIGNAL(panningStateChanged(bool)));

    gPannableWidgetPosition = QPointF(0, 10);
    m_subject->enablePanningIfPositionChanged();
    panningStateSpy.clear();
    gTimerStarted = false;

    // Timer timeout without a position change should cause the panning to be considered stopped
    m_subject->disablePanningIfPositionNotChanged();
    QCOMPARE(panningStateSpy.count(), 1);
    QVERIFY(!panningStateSpy.at(0).at(0).toBool());
    QVERIFY(!gTimerStarted);
}

void Ut_PanningWatcher::testStateSignalIsNotSentWhenMovementTimeoutOccursAndPannableWidgetDoesNotMoveWhenAlreadyNotPanning()
{
    QSignalSpy panningStateSpy(m_subject, SIGNAL(panningStateChanged(bool)));

    gPannableWidgetPosition = QPointF(0, 10);
    m_subject->enablePanningIfPositionChanged();
    m_subject->disablePanningIfPositionNotChanged();
    panningStateSpy.clear();
    gTimerStarted = false;

    // Timer timeout without a position change when the panning has already stopped should do nothing
    m_subject->disablePanningIfPositionNotChanged();
    QCOMPARE(panningStateSpy.count(), 0);
    QVERIFY(!gTimerStarted);
}

void Ut_PanningWatcher::testStateSignalIsSentWhenMovementTimeoutOccursAndPannableWidgetDoesMoveWhenNotPanning()
{
    QSignalSpy panningStateSpy(m_subject, SIGNAL(panningStateChanged(bool)));

    gPannableWidgetPosition = QPointF(0, 10);
    m_subject->enablePanningIfPositionChanged();
    m_subject->disablePanningIfPositionNotChanged();
    panningStateSpy.clear();
    gTimerStarted = false;

    // Timer timeout with a position change when not panning should send a signal and restart the timer
    gPannableWidgetPosition = QPointF(0, 15);
    m_subject->disablePanningIfPositionNotChanged();
    QCOMPARE(panningStateSpy.count(), 1);
    QVERIFY(panningStateSpy.at(0).at(0).toBool());
    QVERIFY(gTimerStarted);
}

void Ut_PanningWatcher::testStateSignalIsNotSentWhenMovementTimeoutOccursAndPannableWidgetDoesMoveWhenAlreadyPanning()
{
    QSignalSpy panningStateSpy(m_subject, SIGNAL(panningStateChanged(bool)));

    gPannableWidgetPosition = QPointF(0, 10);
    m_subject->enablePanningIfPositionChanged();
    panningStateSpy.clear();
    gTimerStarted = false;

    // Timer timeout with a position change when already panning should only restart the timer
    gPannableWidgetPosition = QPointF(0, 15);
    m_subject->disablePanningIfPositionNotChanged();
    QCOMPARE(panningStateSpy.count(), 0);
    QVERIFY(gTimerStarted);
}

QTEST_APPLESS_MAIN(Ut_PanningWatcher)
