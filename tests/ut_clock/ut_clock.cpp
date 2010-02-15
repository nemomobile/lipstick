/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of duihome.
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

#include <DuiOnDisplayChangeEvent>
#include "ut_clock.h"
#include "clock.h"

int QObject::startTimer(int)
{
    int id = 1;
    if (!Ut_Clock::timerIds.isEmpty()) {
        id = Ut_Clock::timerIds.last() + 1;
    }
    Ut_Clock::timerIds.append(id);
    return id;
}

void QObject::killTimer(int id)
{
    Ut_Clock::timerIds.removeAll(id);
}

QList<int> Ut_Clock::timerIds;

// Called before the first testfunction is executed
void Ut_Clock::initTestCase()
{
}

// Called after the last testfunction was executed
void Ut_Clock::cleanupTestCase()
{
}

// Called before each testfunction is executed
void Ut_Clock::init()
{
    m_subject = new Clock();
}

// Called after every testfunction
void Ut_Clock::cleanup()
{
    delete m_subject;
}

void Ut_Clock::testModelUpdates()
{
    DuiOnDisplayChangeEvent exitDisplayEvent(DuiOnDisplayChangeEvent::FullyOffDisplay, QRectF());
    DuiOnDisplayChangeEvent enterDisplayEvent(DuiOnDisplayChangeEvent::FullyOnDisplay, QRectF());

    QCOMPARE(timerIds.count(), 1);
    QCOMPARE(timerIds.at(0), 1);

    // When the application becomes invisible the timer should stop
    qApp->sendEvent(m_subject, &exitDisplayEvent);
    QCOMPARE(timerIds.count(), 0);

    // When the application becomes visible the timer should start
    qApp->sendEvent(m_subject, &enterDisplayEvent);
    QCOMPARE(timerIds.count(), 1);
    QCOMPARE(timerIds.at(0), 1);

    // Only one timer should be running
    qApp->sendEvent(m_subject, &enterDisplayEvent);
    QCOMPARE(timerIds.count(), 1);
    QCOMPARE(timerIds.at(0), 1);
}

QTEST_MAIN(Ut_Clock)
