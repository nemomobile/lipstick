/***************************************************************************
**
** Copyright (C) 2012 Jolla Ltd.
** Contact: Robin Burchell <robin.burchell@jollamobile.com>
**
** This file is part of lipstick.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include <QtTest/QtTest>
#include <QQuickView>
#include <QQmlContext>
#include <QScreen>
#include "lipstickcompositor.h"
#include "notificationmanager.h"
#include "ut_notificationpreviewpresenter.h"
#include "notificationpreviewpresenter.h"
#include "closeeventeater_stub.h"
#include "qmlocks_stub.h"
#include "qmdisplaystate_stub.h"

Q_DECLARE_METATYPE(NotificationPreviewPresenter*)
Q_DECLARE_METATYPE(LipstickNotification*)

LipstickCompositor *LipstickCompositor::instance()
{
    return (LipstickCompositor *)1;
}

QList<QQuickView *> qQuickViews;
void QQuickView::setSource(const QUrl &)
{
    qQuickViews.append(this);
}

QHash<QWindow *, bool> qWindowVisible;
void QWindow::show()
{
    qWindowVisible[this] = true;
}
void QWindow::hide()
{
    qWindowVisible[this] = false;
}

const char *NotificationManager::HINT_CATEGORY = "category";
const char *NotificationManager::HINT_URGENCY = "urgency";
const char *NotificationManager::HINT_ICON = "x-nemo-icon";
const char *NotificationManager::HINT_ITEM_COUNT = "x-nemo-item-count";
const char *NotificationManager::HINT_PRIORITY = "x-nemo-priority";
const char *NotificationManager::HINT_TIMESTAMP = "x-nemo-timestamp";
const char *NotificationManager::HINT_PREVIEW_ICON = "x-nemo-preview-icon";
const char *NotificationManager::HINT_PREVIEW_BODY = "x-nemo-preview-body";
const char *NotificationManager::HINT_PREVIEW_SUMMARY = "x-nemo-preview-summary";
const char *NotificationManager::HINT_HIDDEN = "x-nemo-hidden";

NotificationManager::NotificationManager(QObject *parent) : QObject(parent)
{
}

NotificationManager::~NotificationManager()
{
}

void NotificationManager::invokeAction(const QString &)
{
}

void NotificationManager::removeNotificationsWithCategory(const QString &)
{
}

void NotificationManager::updateNotificationsWithCategory(const QString &)
{
}

void NotificationManager::commit()
{
}

QList<uint> notificationManagerCloseNotificationIds;
void NotificationManager::CloseNotification(uint id, NotificationClosedReason)
{
    notificationManagerCloseNotificationIds.append(id);
}

NotificationManager *notificationManagerInstance = 0;
NotificationManager *NotificationManager::instance()
{
    if (notificationManagerInstance == 0) {
        notificationManagerInstance = new NotificationManager;
    }
    return notificationManagerInstance;
}

QHash<uint, LipstickNotification *> notificationManagerNotification;
LipstickNotification *NotificationManager::notification(uint id) const
{
    return notificationManagerNotification.value(id);
}

void NotificationManager::removeUserRemovableNotifications()
{
}

LipstickNotification *createNotification(uint id, int urgency = 0)
{
    LipstickNotification *notification = new LipstickNotification;
    QVariantHash hints;
    hints.insert(NotificationManager::HINT_PREVIEW_SUMMARY, "summary");
    hints.insert(NotificationManager::HINT_PREVIEW_BODY, "body");
    hints.insert(NotificationManager::HINT_URGENCY, urgency);
    notification->setHints(hints);
    notificationManagerNotification.insert(id, notification);
    return notification;
}

void Ut_NotificationPreviewPresenter::initTestCase()
{
    qRegisterMetaType<LipstickNotification *>();
    NotificationManager::instance()->setParent(this);
}

void Ut_NotificationPreviewPresenter::cleanup()
{
    qQuickViews.clear();
    qWindowVisible.clear();
    qDeleteAll(notificationManagerNotification);
    notificationManagerNotification.clear();
    notificationManagerCloseNotificationIds.clear();
    gQmLocksStub->stubReset();
    gQmDisplayStateStub->stubReset();
}

void Ut_NotificationPreviewPresenter::testSignalConnections()
{
    NotificationPreviewPresenter presenter;
    QCOMPARE(disconnect(NotificationManager::instance(), SIGNAL(notificationModified(uint)), &presenter, SLOT(updateNotification(uint))), true);
    QCOMPARE(disconnect(NotificationManager::instance(), SIGNAL(notificationRemoved(uint)), &presenter, SLOT(removeNotification(uint))), true);
}

void Ut_NotificationPreviewPresenter::testAddNotificationWhenWindowNotOpen()
{
    NotificationPreviewPresenter presenter;
    QSignalSpy spy(&presenter, SIGNAL(notificationChanged()));

    // Check that the window is not automatically created
    QCOMPARE(qQuickViews.isEmpty(), true);

    // Check that the window is created when a notification is added
    LipstickNotification *notification = createNotification(1);
    presenter.updateNotification(1);
    QCOMPARE(qQuickViews.count(), 1);

    // Check window properties
/*
    QCOMPARE(qDeclarativeViews.first()->testAttribute(Qt::WA_TranslucentBackground), true);
    QCOMPARE(qDeclarativeViews.first()->testAttribute(Qt::WA_X11DoNotAcceptFocus), true);
    QCOMPARE(qDeclarativeViews.first()->testAttribute(Qt::WA_X11NetWmWindowTypeNotification), true);
    QCOMPARE(qDeclarativeViews.first()->windowTitle(), QString("Notification"));
    QCOMPARE(qDeclarativeViews.first()->viewport()->autoFillBackground(), false);
    */
    QCOMPARE(qQuickViews.first()->resizeMode(), QQuickView::SizeRootObjectToView);
    QCOMPARE(qQuickViews.first()->rootContext()->contextProperty("initialSize").toSize(), QGuiApplication::primaryScreen()->size());
    QCOMPARE(qQuickViews.first()->rootContext()->contextProperty("notificationPreviewPresenter"), QVariant::fromValue(static_cast<QObject *>(&presenter)));

    // Check that the window was shown
    QCOMPARE(qWindowVisible[static_cast<QWindow *>(qQuickViews.first())], true);

    // Check that the expected notification is signaled onwards
    QCOMPARE(spy.count(), 1);
    QCOMPARE(presenter.notification(), notification);
}

void Ut_NotificationPreviewPresenter::testAddNotificationWhenWindowAlreadyOpen()
{
    NotificationPreviewPresenter presenter;
    QSignalSpy spy(&presenter, SIGNAL(notificationChanged()));

    // Create a notification: this will create a window
    createNotification(1);
    presenter.updateNotification(1);

    // Reset stubs to see what happens next
    qQuickViews.clear();

    // Create another notification
    LipstickNotification *notification = createNotification(2);
    presenter.updateNotification(2);

    // The second notification should not be signaled onwards yet since the first one is being presented
    QCOMPARE(spy.count(), 1);

    // Show the next notification
    presenter.showNextNotification();

    // Check that the window was not unnecessarily created again
    QCOMPARE(qQuickViews.isEmpty(), true);

    // Check that the expected notification is signaled onwards
    QCOMPARE(spy.count(), 2);
    QCOMPARE(presenter.notification(), notification);
}

void Ut_NotificationPreviewPresenter::testUpdateNotification()
{
    NotificationPreviewPresenter presenter;

    // Create two notifications
    createNotification(1);
    createNotification(2);
    presenter.updateNotification(1);
    presenter.updateNotification(2);

    // Update both notifications
    QSignalSpy spy(&presenter, SIGNAL(notificationChanged()));
    presenter.updateNotification(1);
    presenter.updateNotification(2);

    // Check that no signals were sent
    QCOMPARE(spy.count(), 0);
}

void Ut_NotificationPreviewPresenter::testRemoveNotification()
{
    NotificationPreviewPresenter presenter;
    QSignalSpy spy(&presenter, SIGNAL(notificationChanged()));

    // Create two notifications
    createNotification(1);
    createNotification(2);
    presenter.updateNotification(1);
    presenter.updateNotification(2);

    // Remove the first one
    presenter.removeNotification(1);

    // Check that an empty notification is signaled onwards
    QCOMPARE(spy.count(), 2);
    QCOMPARE(presenter.notification(), (LipstickNotification *)0);

    // Show and remove the second one
    presenter.showNextNotification();
    presenter.removeNotification(2);

    // Check that an empty notification is signaled onwards
    QCOMPARE(spy.count(), 4);
    QCOMPARE(presenter.notification(), (LipstickNotification *)0);

    // Check that the window is not yet hidden
    QCOMPARE(qWindowVisible[static_cast<QWindow *>(qQuickViews.first())], true);

    // Check that the window is hidden when it's time to show the next notification (which doesn't exist)
    presenter.showNextNotification();
    QCOMPARE(qWindowVisible[static_cast<QWindow *>(qQuickViews.first())], false);
}

void Ut_NotificationPreviewPresenter::testNotificationNotShownIfNoSummaryOrBody_data()
{
    QTest::addColumn<QString>("previewSummary");
    QTest::addColumn<QString>("previewBody");
    QTest::addColumn<int>("signalCount");
    QTest::addColumn<bool>("windowVisible");
    QTest::newRow("No summary, no body") << "" << "" << 0 << false;
    QTest::newRow("Summary, no body") << "summary" << "" << 1 << true;
    QTest::newRow("No summary, body") << "" << "body" << 1 << true;
    QTest::newRow("Summary, body") << "summary" << "body" << 1 << true;
}

void Ut_NotificationPreviewPresenter::testNotificationNotShownIfNoSummaryOrBody()
{
    QFETCH(QString, previewSummary);
    QFETCH(QString, previewBody);
    QFETCH(int, signalCount);
    QFETCH(bool, windowVisible);

    NotificationPreviewPresenter presenter;
    QSignalSpy spy(&presenter, SIGNAL(notificationChanged()));

    // Create notification
    LipstickNotification *notification = new LipstickNotification;
    QVariantHash hints;
    hints.insert(NotificationManager::HINT_PREVIEW_SUMMARY, previewSummary);
    hints.insert(NotificationManager::HINT_PREVIEW_BODY, previewBody);
    notification->setHints(hints);
    notificationManagerNotification.insert(1, notification);
    presenter.updateNotification(1);

    // Check whether the expected notification is signaled onwards
    QCOMPARE(spy.count(), signalCount);

    QCOMPARE(qWindowVisible.isEmpty(), !windowVisible);
    if (windowVisible) {
        // Check whether the window was shown
        QCOMPARE(qWindowVisible[static_cast<QWindow *>(qQuickViews.first())], windowVisible);
    }
}

void Ut_NotificationPreviewPresenter::testNotificationNotShownIfHidden()
{
    NotificationPreviewPresenter presenter;
    QSignalSpy spy(&presenter, SIGNAL(notificationChanged()));

    // Create notification
    LipstickNotification *notification = new LipstickNotification;
    QVariantHash hints;
    hints.insert(NotificationManager::HINT_PREVIEW_SUMMARY, "previewSummary");
    hints.insert(NotificationManager::HINT_PREVIEW_BODY, "previewBody");
    hints.insert(NotificationManager::HINT_HIDDEN, true);
    notification->setHints(hints);
    notificationManagerNotification.insert(1, notification);
    presenter.updateNotification(1);

    QCOMPARE(spy.count(), 0);
    QCOMPARE(qWindowVisible.isEmpty(), true);
}

void Ut_NotificationPreviewPresenter::testShowingOnlyCriticalNotifications()
{
    NotificationPreviewPresenter presenter;
    QSignalSpy spy(&presenter, SIGNAL(notificationChanged()));

    // Create normal urgency notification
    LipstickNotification *notification = new LipstickNotification;
    QVariantHash hints;
    hints.insert(NotificationManager::HINT_PREVIEW_SUMMARY, "previewSummary");
    hints.insert(NotificationManager::HINT_PREVIEW_BODY, "previewBody");
    hints.insert(NotificationManager::HINT_URGENCY, 1);
    notification->setHints(hints);
    notificationManagerNotification.insert(1, notification);
    QCOMPARE(qWindowVisible.isEmpty(), true);

    // When the screen or device is locked and the urgency is not high enough, so the notification shouldn't be shown
    gQmLocksStub->stubSetReturnValue("getState", MeeGo::QmLocks::Locked);
    presenter.updateNotification(1);
    QCOMPARE(spy.count(), 0);
    QCOMPARE(qWindowVisible.isEmpty(), true);

    // Urgency set to critical, so the notification should be shown
    hints.insert(NotificationManager::HINT_URGENCY, 2);
    notification->setHints(hints);
    presenter.updateNotification(1);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(qWindowVisible.isEmpty(), false);
    QCOMPARE(qWindowVisible[static_cast<QWindow *>(qQuickViews.first())], true);
}

void Ut_NotificationPreviewPresenter::testUpdateNotificationRemovesNotificationFromQueueIfNotShowable()
{
    NotificationPreviewPresenter presenter;

    // Create two notifications
    LipstickNotification *notification1 = createNotification(1);
    LipstickNotification *notification2 = createNotification(2);
    presenter.updateNotification(1);
    presenter.updateNotification(2);

    // Update the notifications to have no summary or body
    QSignalSpy spy(&presenter, SIGNAL(notificationChanged()));
    notification1->setHints(QVariantHash());
    notification2->setHints(QVariantHash());
    presenter.updateNotification(1);
    presenter.updateNotification(2);

    // Check that the current notification is not removed
    QCOMPARE(spy.count(), 0);

    // Check that the other notification is removed from the queue
    presenter.showNextNotification();
    QCOMPARE(spy.count(), 1);
    QCOMPARE(presenter.notification(), (LipstickNotification *)0);
}

Q_DECLARE_METATYPE(MeeGo::QmDisplayState::DisplayState)
Q_DECLARE_METATYPE(MeeGo::QmLocks::State)

void Ut_NotificationPreviewPresenter::testNotificationNotShownIfTouchScreenIsLockedAndDisplayIsOff_data()
{
    QTest::addColumn<MeeGo::QmDisplayState::DisplayState>("displayState");
    QTest::addColumn<MeeGo::QmLocks::State>("lockState");
    QTest::addColumn<int>("notifications");
    QTest::newRow("Display on, touch screen not locked") << MeeGo::QmDisplayState::On << MeeGo::QmLocks::Unlocked << 1;
    QTest::newRow("Display on, touch screen locked") << MeeGo::QmDisplayState::On << MeeGo::QmLocks::Locked << 1;
    QTest::newRow("Display off, touch screen not locked") << MeeGo::QmDisplayState::Off << MeeGo::QmLocks::Unlocked << 1;
    QTest::newRow("Display off, touch screen locked") << MeeGo::QmDisplayState::Off << MeeGo::QmLocks::Locked << 0;
}

void Ut_NotificationPreviewPresenter::testNotificationNotShownIfTouchScreenIsLockedAndDisplayIsOff()
{
    QFETCH(MeeGo::QmDisplayState::DisplayState, displayState);
    QFETCH(MeeGo::QmLocks::State, lockState);
    QFETCH(int, notifications);

    gQmDisplayStateStub->stubSetReturnValue("get", displayState);
    gQmLocksStub->stubSetReturnValue("getState", lockState);

    NotificationPreviewPresenter presenter;
    QSignalSpy spy(&presenter, SIGNAL(notificationChanged()));

    createNotification(1, 2);
    presenter.updateNotification(1);
    QCOMPARE(qQuickViews.count(), notifications);
    QCOMPARE(spy.count(), notifications);
}

void Ut_NotificationPreviewPresenter::testCriticalNotificationIsClosedAfterShowing()
{
    NotificationPreviewPresenter presenter;
    createNotification(1, 2);
    createNotification(2);
    createNotification(3);
    presenter.updateNotification(1);
    presenter.updateNotification(2);
    presenter.updateNotification(3);
    QCOMPARE(notificationManagerCloseNotificationIds.count(), 0);

    presenter.showNextNotification();
    QCOMPARE(notificationManagerCloseNotificationIds.count(), 1);
    QCOMPARE(notificationManagerCloseNotificationIds.at(0), (uint)1);

    presenter.showNextNotification();
    QCOMPARE(notificationManagerCloseNotificationIds.count(), 1);
}

QTEST_MAIN(Ut_NotificationPreviewPresenter)
