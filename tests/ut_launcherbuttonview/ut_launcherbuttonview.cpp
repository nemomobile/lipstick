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
#include "mprogressindicator_stub.h"
#include "ut_launcherbuttonview.h"
#include "launcherbuttonview.h"
#include "launcherbuttonmodel.h"
#include "launcherbutton_stub.h"
#include "launcheraction_stub.h"
#include "windowinfo_stub.h"

// MButton stubs
QString mButtonText;
QString MButton::text() const
{
    return mButtonText;
}

// QTimer stubs
bool qTimerStarted;
void QTimer::start()
{
    qTimerStarted = true;
    id = 0;
}

void QTimer::stop()
{
    qTimerStarted = false;
    id = -1;
}

// MButtonIconView stubs
bool mButtonIconViewApplyStyleCalled;
void MButtonIconView::applyStyle()
{
    mButtonIconViewApplyStyleCalled = true;
}

bool mWidgetViewUpdateCalled;
void MWidgetView::update(const QRectF &)
{
    mWidgetViewUpdateCalled = true;
}

void Ut_LauncherButtonView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_launcherbuttonview" };
    app = new MApplication(argc, app_name);
    mButtonText = "test";
}

void Ut_LauncherButtonView::cleanupTestCase()
{
    delete app;
}

void Ut_LauncherButtonView::init()
{
    controller = new LauncherButton;
    controller->setModel(new LauncherButtonModel);
    m_subject = new LauncherButtonView(controller);
    controller->setView(m_subject);
    mWidgetViewUpdateCalled = false;
    mButtonIconViewApplyStyleCalled = false;
    qTimerStarted = false;

    gMProgressIndicatorStub->stubReset();
}

void Ut_LauncherButtonView::cleanup()
{
    delete controller;
}

void Ut_LauncherButtonView::testInitialization()
{
    controller->setObjectName("LauncherButton");
    QVERIFY(m_subject->progressIndicator->objectName() == "LauncherButtonProgressIndicator");

    controller->setObjectName("QuickLaunchBarButton");
    QVERIFY(m_subject->progressIndicator->objectName() == "QuickLaunchBarButtonProgressIndicator");

    QCOMPARE(m_subject->model()->buttonState(), LauncherButtonModel::Installed);
    QVERIFY(disconnect(&m_subject->launchProgressTimeoutTimer, SIGNAL(timeout()), controller, SLOT(stopLaunchProgress())));
    QVERIFY(m_subject->launchProgressTimeoutTimer.isSingleShot());
    QVERIFY(!qTimerStarted);
}

void Ut_LauncherButtonView::testApplyStyle()
{
    m_subject->applyStyle();
    QVERIFY(mButtonIconViewApplyStyleCalled);
}

void Ut_LauncherButtonView::testResetProgressIndicator_data()
{
     QTest::addColumn<LauncherButtonModel::State>("state");
     QTest::addColumn<bool>("verifyVisible");
     QTest::addColumn<int>("verifySetUnknownDurationCallCount");
     QTest::addColumn<bool>("verifySetUnknownDurationParam");

     QTest::newRow("installed") << LauncherButtonModel::Installed << false << 0 << false;

     QTest::newRow("Launching") << LauncherButtonModel::Launching << true << 2 << true;
     QTest::newRow("Installing") << LauncherButtonModel::Installing << true << 2 << true;
     // reset calls already setUnknowDuration once with false parameter
     QTest::newRow("Downloading") << LauncherButtonModel::Downloading << true << 1 << false;
}

void Ut_LauncherButtonView::testResetProgressIndicator()
{
    QFETCH(LauncherButtonModel::State, state);
    QFETCH(bool, verifyVisible);
    QFETCH(int, verifySetUnknownDurationCallCount);
    QFETCH(bool, verifySetUnknownDurationParam);

    m_subject->model()->setButtonState(state);

    QCOMPARE(m_subject->progressIndicator->isVisible(), verifyVisible);
    QCOMPARE(gMProgressIndicatorStub->stubCallCount("setUnknownDuration"), verifySetUnknownDurationCallCount);
    if (verifySetUnknownDurationCallCount > 0) {
        QCOMPARE(gMProgressIndicatorStub->stubLastCallTo("setUnknownDuration").parameter<bool>(0), verifySetUnknownDurationParam);
    }
}

void Ut_LauncherButtonView::testLaunchingProgress()
{
    m_subject->model()->setButtonState(LauncherButtonModel::Launching);
    QVERIFY(qTimerStarted);
    // one for reset and one for button state change
    QCOMPARE(gMProgressIndicatorStub->stubCallCount("setUnknownDuration"), 2);
    QCOMPARE(gMProgressIndicatorStub->stubLastCallTo("setUnknownDuration").parameter<bool>(0), true);

    m_subject->model()->setButtonState(LauncherButtonModel::Installed);
    QVERIFY(!qTimerStarted);
}

void Ut_LauncherButtonView::testUpdateProgressWhenDownloading()
{
    m_subject->model()->setButtonState(LauncherButtonModel::Downloading);

    m_subject->model()->setOperationProgress(50);
    QCOMPARE(gMProgressIndicatorStub->stubLastCallTo("setValue").parameter<int>(0), 50);
}

void Ut_LauncherButtonView::testUpdateProgressWhenNotDownloading()
{
    m_subject->model()->setButtonState(LauncherButtonModel::Installing);

    m_subject->model()->setOperationProgress(50);
    QCOMPARE(gMProgressIndicatorStub->stubLastCallTo("setValue").parameter<int>(0), 0);
}

QTEST_APPLESS_MAIN(Ut_LauncherButtonView)
