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
#include "ut_homescreenservice.h"
#include "homescreenservice.h"

static const QString TEST_DESKTOP_FILE = "test.desktop";

void Ut_HomeScreenService::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_homescreenservice" };
    app = new MApplication(argc, app_name);
}

void Ut_HomeScreenService::cleanupTestCase()
{
    delete app;
}

void Ut_HomeScreenService::init()
{
    m_subject = new HomeScreenService();
}

void Ut_HomeScreenService::cleanup()
{
    delete m_subject;
}

void Ut_HomeScreenService::testShowLauncher()
{
    const int EXPECTED_COUNT_OF_EMITTED_SIGNALS = 1;
    const QString EXPECTED_DESKTOP_FILE = TEST_DESKTOP_FILE;

    QSignalSpy spy(m_subject, SIGNAL(focusToLauncherApp(const QString &)));
    m_subject->showLauncher(TEST_DESKTOP_FILE);

    QCOMPARE(spy.count(), EXPECTED_COUNT_OF_EMITTED_SIGNALS);

    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), EXPECTED_DESKTOP_FILE);
}

QTEST_APPLESS_MAIN(Ut_HomeScreenService)
