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

#include <QtTest/QtTest>
#include <MApplication>
#include "ut_launcherbutton.h"
#include "launcherbutton.h"
#include "launcher_stub.h"

void Ut_LauncherButton::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_launcherbutton" };
    app = new MApplication(argc, app_name);
}

void Ut_LauncherButton::cleanupTestCase()
{
    delete app;
}

void Ut_LauncherButton::init()
{
    m_subject = new LauncherButton();
    connect(this, SIGNAL(clicked()), m_subject, SLOT(launch()));
}

void Ut_LauncherButton::cleanup()
{
    delete m_subject;
}

void Ut_LauncherButton::testLaunchApplication()
{
    m_subject->setTargetType("Application");
    m_subject->setTarget("testApplication");
    emit clicked();
    QCOMPARE(gLauncherStub->stubCallCount("startApplication"), 1);
    QCOMPARE(gLauncherStub->stubLastCallTo("startApplication").parameter<QString>(0), m_subject->target());
}

void Ut_LauncherButton::testLaunchMApplication()
{
    m_subject->setTargetType("Service");
    m_subject->setTarget("testService");
    emit clicked();
    QCOMPARE(gLauncherStub->stubCallCount("startMApplication"), 1);
    QCOMPARE(gLauncherStub->stubLastCallTo("startMApplication").parameter<QString>(0), m_subject->target());
}

QTEST_APPLESS_MAIN(Ut_LauncherButton)
