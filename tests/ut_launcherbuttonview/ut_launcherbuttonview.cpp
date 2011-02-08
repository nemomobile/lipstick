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
#include "ut_launcherbuttonview.h"
#include "homewindowmonitor.h"
#include "launcherbutton_stub.h"
#include "launcheraction_stub.h"
#include "windowinfo_stub.h"
#include "launcherbuttonprogressindicator_stub.h"
#include "mprogressindicator_stub.h"
#include "mdesktopentry_stub.h"

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

// QIcon stubs
QString qIconFileName;
QIcon::QIcon(const QString &fileName)
{
    qIconFileName = fileName;
}

QIcon& QIcon::operator=(QIcon const &)
{
    return *this;
}

QIcon::~QIcon()
{
}

bool qIconHasThemeIcon = false;
bool QIcon::hasThemeIcon(const QString &name)
{
    Q_UNUSED(name);
    return qIconHasThemeIcon;
}

QString qIconName;
QIcon QIcon::fromTheme(const QString &name, const QIcon &fallback)
{
    qIconName = name;
    return fallback;
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
    controller = new LauncherButton("");
    controller->setModel(new LauncherButtonModel);
    m_subject = new TestLauncherButtonView(controller);
    controller->setView(m_subject);
    mWidgetViewUpdateCalled = false;
    mButtonIconViewApplyStyleCalled = false;
    qTimerStarted = false;
    qIconHasThemeIcon = false;
    qIconFileName.clear();
    qIconName.clear();

    m_subject->modifiableStyle()->setShowLaunchProgress(true);
    gMProgressIndicatorStub->stubReset();
}

void Ut_LauncherButtonView::cleanup()
{
    delete controller;
    gMDesktopEntryStub->stubReset();
}

void Ut_LauncherButtonView::testInitialization()
{
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

     QTest::newRow("Installed") << LauncherButtonModel::Installed << false << 0 << false;

     QTest::newRow("Launching") << LauncherButtonModel::Launching << true << 2 << true;
     QTest::newRow("Installing") << LauncherButtonModel::Installing << true << 2 << true;
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
    QCOMPARE(gLauncherButtonProgressIndicatorStub->stubLastCallTo("setIndicatorState").parameter<LauncherButtonModel::State>(0), state);
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

void Ut_LauncherButtonView::testUpdatingIconFromDesktopEntry()
{
    gMDesktopEntryStub->stubSetReturnValue("icon", QString("icon"));
    m_subject->model()->setDesktopEntry(QSharedPointer<MDesktopEntry>(new MDesktopEntry("/dev/null")));
    QCOMPARE(m_subject->model()->iconID(), QString("icon"));
}

void Ut_LauncherButtonView::testUpdatingAbsoluteIconFromDesktopEntry()
{
    gMDesktopEntryStub->stubSetReturnValue("icon", QString("/icon"));
    m_subject->model()->setDesktopEntry(QSharedPointer<MDesktopEntry>(new MDesktopEntry("/dev/null")));
    QCOMPARE(qIconFileName, QString("/icon"));
}

void Ut_LauncherButtonView::testUpdatingFreeDesktopIconFromDesktopEntry()
{
    qIconHasThemeIcon = true;
    gMDesktopEntryStub->stubSetReturnValue("icon", QString("icon"));
    m_subject->model()->setDesktopEntry(QSharedPointer<MDesktopEntry>(new MDesktopEntry("/dev/null")));
    QCOMPARE(qIconName, QString("icon"));
}

void Ut_LauncherButtonView::testUpdatingPlaceholderIcons_data()
{
     QTest::addColumn<LauncherButtonModel::State>("state");
     QTest::addColumn<QString>("iconId");

     QTest::newRow("Downloading") << LauncherButtonModel::Downloading << "Downloading";
     QTest::newRow("Installing") << LauncherButtonModel::Installing << "Installing";
     QTest::newRow("Broken") << LauncherButtonModel::Broken << "Broken";
}

void Ut_LauncherButtonView::testUpdatingPlaceholderIcons()
{
    QFETCH(LauncherButtonModel::State, state);
    QFETCH(QString, iconId);

    m_subject->modifiableStyle()->setDownloadingPlaceholderIcon("Downloading");
    m_subject->modifiableStyle()->setInstallingPlaceholderIcon("Installing");
    m_subject->modifiableStyle()->setBrokenPlaceholderIcon("Broken");

    m_subject->model()->setButtonState(state);

    QCOMPARE(m_subject->model()->iconID(), iconId);
}

void Ut_LauncherButtonView::testUpdatingIconFromDesktopEntryAfterPlaceholderIcon()
{
    gMDesktopEntryStub->stubSetReturnValue("icon", QString("icon"));
    m_subject->model()->setDesktopEntry(QSharedPointer<MDesktopEntry>(new MDesktopEntry("/dev/null")));

    // change to a state with placeholder icon
    QString placeholderIconId = "Installing";
    m_subject->modifiableStyle()->setInstallingPlaceholderIcon(placeholderIconId);
    m_subject->model()->setButtonState(LauncherButtonModel::Installing);
    QCOMPARE(m_subject->model()->iconID(), placeholderIconId);

    // change back to a installed state with icon from content action
    m_subject->model()->setButtonState(LauncherButtonModel::Installed);
    QCOMPARE(m_subject->model()->iconID(), QString("icon"));
}

void Ut_LauncherButtonView::testEnablingAccordingToState_data()
{
     QTest::addColumn<LauncherButtonModel::State>("state");
     QTest::addColumn<bool>("enabled");
     QTest::newRow("Installed") << LauncherButtonModel::Installed << true;
     QTest::newRow("Launching") << LauncherButtonModel::Launching << false;
     QTest::newRow("Installing") << LauncherButtonModel::Installing << false;
     QTest::newRow("Downloading") << LauncherButtonModel::Downloading << false;
     QTest::newRow("Broken") << LauncherButtonModel::Broken << true;
}

void Ut_LauncherButtonView::testEnablingAccordingToState()
{
    QFETCH(LauncherButtonModel::State, state);
    QFETCH(bool, enabled);
    m_subject->model()->setButtonState(state);
    QCOMPARE(m_subject->controller->isEnabled(), enabled);
}

void Ut_LauncherButtonView::testWhenStateIsChangedToLaunchingThenProgressIndicatorIsShownAccordingToStyle_data()
{
    QTest::addColumn<bool>("showLaunchProgress");
    QTest::addColumn<LauncherButtonModel::State> ("state");
    QTest::addColumn<bool>("visibility");

    QTest::newRow("Visible") << true << LauncherButtonModel::Launching << true;
    QTest::newRow("Not visible") << false << LauncherButtonModel::Launching << false;
}

void Ut_LauncherButtonView::testWhenStateIsChangedToLaunchingThenProgressIndicatorIsShownAccordingToStyle()
{
    QFETCH(bool, showLaunchProgress);
    QFETCH(LauncherButtonModel::State, state);
    QFETCH(bool, visibility);

    m_subject->modifiableStyle()->setShowLaunchProgress(showLaunchProgress);
    m_subject->model()->setButtonState(state);
    QCOMPARE(m_subject->progressIndicator->isVisible(), visibility);
}


QTEST_APPLESS_MAIN(Ut_LauncherButtonView)
