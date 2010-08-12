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
#include "ut_launcherbuttonprogressindicatorview.h"

#include "launcherbuttonprogressindicator.h"

void Ut_LauncherButtonProgressIndicatorView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_launcherbuttonprogressindicatorview" };
    app = new MApplication(argc, app_name);
}

void Ut_LauncherButtonProgressIndicatorView::cleanupTestCase()
{
    delete app;
}

void Ut_LauncherButtonProgressIndicatorView::init()
{
    controller = new LauncherButtonProgressIndicator;
    controller->setModel(new LauncherButtonProgressIndicatorModel);
    m_subject = new TestLauncherButtonProgressIndicatorView(controller);
    controller->setView(m_subject);
}

void Ut_LauncherButtonProgressIndicatorView::cleanup()
{
    delete controller;
}

void Ut_LauncherButtonProgressIndicatorView::testUpdateDataForStyleModes_data()
{
     QTest::addColumn<LauncherButtonModel::State>("state");
     QTest::addColumn<QString>("mode");

     QTest::newRow("Downloading") << LauncherButtonModel::Downloading << "downloading";
     QTest::newRow("Installing") << LauncherButtonModel::Installing << "installing";
     QTest::newRow("Launching") << LauncherButtonModel::Launching << "launching";
}

void Ut_LauncherButtonProgressIndicatorView::testUpdateDataForStyleModes()
{
    QFETCH(LauncherButtonModel::State, state);
    QFETCH(QString, mode);

    m_subject->model()->setIndicatorState(state);

    QCOMPARE(m_subject->style().currentMode(), mode);
}

QTEST_APPLESS_MAIN(Ut_LauncherButtonProgressIndicatorView)
