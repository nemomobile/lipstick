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

#include "ut_homewindowmonitor.h"
#include "homewindowmonitor.h"
#include <QtTest/QtTest>


void Ut_HomeWindowMonitor::initTestCase()
{
}

void Ut_HomeWindowMonitor::cleanupTestCase()
{
}

void Ut_HomeWindowMonitor::init()
{
    m_subject = new HomeWindowMonitor();
}

void Ut_HomeWindowMonitor::cleanup()
{
    delete m_subject;
}

void Ut_HomeWindowMonitor::testRegisteredWindowIdIsRecognizedAsOwnWindow()
{
    const WId SOME_WINDOW_ID = 1001;
    QCOMPARE(m_subject->isOwnWindow(SOME_WINDOW_ID), false);
    m_subject->registerWindowId(SOME_WINDOW_ID);
    QCOMPARE(m_subject->isOwnWindow(SOME_WINDOW_ID), true);
}

QTEST_MAIN(Ut_HomeWindowMonitor)
