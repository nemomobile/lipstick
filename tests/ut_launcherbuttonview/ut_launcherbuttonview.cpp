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
#include <MProgressIndicator>
#include "ut_launcherbuttonview.h"
#include "launcherbuttonview.h"
#include "launcherbutton_stub.h"
#include "launcheraction_stub.h"
#include "windowinfo_stub.h"

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

// QTimeLine stubs
bool qTimeLineStarted;
void QTimeLine::start()
{
    qTimeLineStarted = true;
}

void QTimeLine::stop()
{
    qTimeLineStarted = false;
}

QTimeLine::State QTimeLine::state() const
{
    return qTimeLineStarted ? QTimeLine::Running : QTimeLine::NotRunning;
}

// MButton stubs
QString mButtonText;
QString MButton::text() const
{
    return mButtonText;
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
    qTimeLineStarted = false;
    qTimerStarted = false;
    controller = new LauncherButton;
    controller->setModel(new LauncherButtonModel);
    m_subject = new LauncherButtonView(controller);
    controller->setView(m_subject);
    connect(this, SIGNAL(frameChanged(int)), m_subject, SLOT(setProgressIndicatorFrame(int)));
    mWidgetViewUpdateCalled = false;
    mButtonIconViewApplyStyleCalled = false;
}

void Ut_LauncherButtonView::cleanup()
{
    delete controller;
}

void Ut_LauncherButtonView::testInitialization()
{
    QVERIFY(disconnect(&m_subject->progressIndicatorTimer, SIGNAL(timeout()), m_subject, SLOT(showProgressIndicator())));
    QVERIFY(disconnect(&m_subject->progressIndicatorTimeLine, SIGNAL(frameChanged(int)), m_subject, SLOT(setProgressIndicatorFrame(int))));

    QVERIFY(m_subject->progressIndicatorTimer.isSingleShot());
    QCOMPARE(m_subject->progressIndicatorTimeLine.state(), QTimeLine::NotRunning);
    QCOMPARE(m_subject->progressIndicatorTimeLine.loopCount(), 0);
    QCOMPARE(m_subject->progressIndicatorTimeLine.curveShape(), QTimeLine::LinearCurve);
}

void Ut_LauncherButtonView::testApplyStyle()
{
    LauncherButtonStyle *style = const_cast<LauncherButtonStyle *>(m_subject->style().operator ->());
    style->setProgressIndicatorTimeout(12345);

    m_subject->applyStyle();
    QVERIFY(mButtonIconViewApplyStyleCalled);
    QCOMPARE(gLauncherButtonStub->stubLastCallTo("setProgressIndicatorTimeout").parameter<int>(0), m_subject->style()->progressIndicatorTimeout());
}

void Ut_LauncherButtonView::testUpdateData()
{
    controller->model()->setShowProgressIndicator(true);
    QCOMPARE(m_subject->progressIndicatorTimer.isActive(), true);

    controller->model()->setShowProgressIndicator(false);
    QCOMPARE(m_subject->progressIndicatorTimer.isActive(), false);
}

void Ut_LauncherButtonView::testShowProgressIndicator()
{
    m_subject->showProgressIndicator();
    QCOMPARE(m_subject->progressIndicatorTimeLine.state(), QTimeLine::Running);
    QVERIFY(mWidgetViewUpdateCalled);

    mWidgetViewUpdateCalled = false;
    m_subject->showProgressIndicator();
    QVERIFY(!mWidgetViewUpdateCalled);
}

void Ut_LauncherButtonView::testHideProgressIndicator()
{
    m_subject->showProgressIndicator();

    mWidgetViewUpdateCalled = false;
    m_subject->hideProgressIndicator();
    QCOMPARE(m_subject->progressIndicatorTimer.isActive(), false);
    QCOMPARE(m_subject->progressIndicatorTimeLine.state(), QTimeLine::NotRunning);
    QVERIFY(mWidgetViewUpdateCalled);
}

void Ut_LauncherButtonView::testSetProgressIndicatorFrame()
{
    emit frameChanged(5);
    QVERIFY(mWidgetViewUpdateCalled);
}

QTEST_APPLESS_MAIN(Ut_LauncherButtonView)
