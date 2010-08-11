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
#include "mapplication_stub.h"
#include "mwindow_stub.h"

#include <QtTest/QtTest>

const WId OWN_WINDOW_ID = 2002;

WId QWidget::winId() const
{
    return OWN_WINDOW_ID;
}

void Ut_HomeWindowMonitor::initTestCase()
{
}

void Ut_HomeWindowMonitor::cleanupTestCase()
{
}

void Ut_HomeWindowMonitor::init()
{
    m_subject = new HomeWindowMonitor();

    gMApplicationStub->stubReset();

    windowList.append(new MWindow);
    gMApplicationStub->stubSetReturnValue<QList<MWindow*> >("windows", windowList);
}

void Ut_HomeWindowMonitor::cleanup()
{
    delete m_subject;

    qDeleteAll(windowList);
    windowList.clear();
}

void Ut_HomeWindowMonitor::testWindowIdInWindowListIsRecognizedAsOwnWindow()
{
    QCOMPARE(m_subject->isOwnWindow(OWN_WINDOW_ID), true);
}

void Ut_HomeWindowMonitor::testWindowIdNotInWindowListIsNotRecognizedAsOwnWindow()
{
    const WId SOME_WINDOW_ID = 1001;
    QCOMPARE(m_subject->isOwnWindow(SOME_WINDOW_ID), false);
}

QTEST_MAIN(Ut_HomeWindowMonitor)
