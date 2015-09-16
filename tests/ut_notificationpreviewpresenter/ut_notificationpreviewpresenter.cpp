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
#include "notificationmanager.h"
#include "ut_notificationpreviewpresenter.h"
#include "notificationpreviewpresenter.h"
#include "notificationfeedbackplayer_stub.h"
#include "lipstickcompositor_stub.h"
#include "closeeventeater_stub.h"
#include "qmlocks_stub.h"
#include "qmdisplaystate_stub.h"
#include "lipstickqmlpath_stub.h"
#include "lipsticksettings.h"

Q_DECLARE_METATYPE(NotificationPreviewPresenter*)
Q_DECLARE_METATYPE(LipstickNotification*)

#include "homewindow.h"

HomeWindow::HomeWindow()
{
}

HomeWindow::~HomeWindow()
{
}

QList<HomeWindow *> homeWindows;
void HomeWindow::setSource(const QUrl &)
{
    homeWindows.append(this);
}

QHash<HomeWindow *, QString> homeWindowTitle;
void HomeWindow::setWindowTitle(const QString &title)
{
    homeWindowTitle[this] = title;
}

QHash<HomeWindow *, bool> homeWindowVisible;
void HomeWindow::show()
{
    homeWindowVisible[this] = true;
}

void HomeWindow::hide()
{
    homeWindowVisible.remove(this);
}

bool HomeWindow::isVisible() const
{
    return homeWindowVisible.value(const_cast<HomeWindow *>(this));
}

QHash<HomeWindow *, QVariantMap> homeWindowContextProperties;
void HomeWindow::setContextProperty(const QString &key, const QVariant &value)
{
    homeWindowContextProperties[this].insert(key, value);
}

void HomeWindow::setContextProperty(const QString &key, QObject *value)
{
    homeWindowContextProperties[this].insert(key, QVariant::fromValue(static_cast<QObject *>(value)));
}

void HomeWindow::setGeometry(const QRect &)
{
}

QHash<HomeWindow *, QString> homeWindowCategories;

void HomeWindow::setCategory(const QString &category)
{
    homeWindowCategories[this] = category;
}

LipstickSettings *LipstickSettings::instance()
{
    return 0;
}

const char *NotificationManager::HINT_CATEGORY = "category";
const char *NotificationManager::HINT_URGENCY = "urgency";
const char *NotificationManager::HINT_IMAGE_PATH = "image-path";
const char *NotificationManager::HINT_ICON = "x-nemo-icon";
const char *NotificationManager::HINT_ITEM_COUNT = "x-nemo-item-count";
const char *NotificationManager::HINT_PRIORITY = "x-nemo-priority";
const char *NotificationManager::HINT_TIMESTAMP = "x-nemo-timestamp";
const char *NotificationManager::HINT_PREVIEW_ICON = "x-nemo-preview-icon";
const char *NotificationManager::HINT_PREVIEW_BODY = "x-nemo-preview-body";
const char *NotificationManager::HINT_PREVIEW_SUMMARY = "x-nemo-preview-summary";
const char *NotificationManager::HINT_REMOTE_ACTION_PREFIX = "x-nemo-remote-action-";
const char *NotificationManager::HINT_REMOTE_ACTION_ICON_PREFIX = "x-nemo-remote-action-icon-";
const char *NotificationManager::HINT_HIDDEN = "x-nemo-hidden";
const char *NotificationManager::HINT_USER_REMOVABLE = "x-nemo-user-removable";
const char *NotificationManager::HINT_DISPLAY_ON = "x-nemo-display-on";
const char *NotificationManager::HINT_ORIGIN = "x-nemo-origin";
const char *NotificationManager::HINT_OWNER = "x-nemo-owner";
const char *NotificationManager::HINT_MAX_CONTENT_LINES = "x-nemo-max-content-lines";
const char *NotificationManager::HINT_RESTORED = "x-nemo-restored";

NotificationManager::NotificationManager(QObject *parent, bool owner) : QObject(parent)
{
    Q_UNUSED(owner)
}

NotificationManager::~NotificationManager()
{
}

void NotificationManager::invokeAction(const QString &)
{
}

void NotificationManager::removeNotificationIfUserRemovable(uint)
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

QList<uint> notificationManagerDisplayedNotificationIds;
void NotificationManager::MarkNotificationDisplayed(uint id)
{
    notificationManagerDisplayedNotificationIds.append(id);
}

NotificationManager *notificationManagerInstance = 0;
NotificationManager *NotificationManager::instance(bool owner)
{
    if (notificationManagerInstance == 0) {
        notificationManagerInstance = new NotificationManager(qApp, owner);
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

void NotificationManager::expire()
{
}

void NotificationManager::reportModifications()
{
}

enum Urgency { Low = 0, Normal = 1, Critical = 2 };

LipstickNotification *createNotification(uint id, Urgency urgency = Normal)
{
    QVariantHash hints;
    hints.insert(NotificationManager::HINT_PREVIEW_SUMMARY, "summary");
    hints.insert(NotificationManager::HINT_PREVIEW_BODY, "body");
    hints.insert(NotificationManager::HINT_URGENCY, static_cast<int>(urgency));
    LipstickNotification *notification = new LipstickNotification("ut_notificationpreviewpresenter", id, "", "", "", QStringList(), hints, -1);
    notificationManagerNotification.insert(id, notification);
    return notification;
}

QVariantMap qWaylandSurfaceWindowProperties;
QVariantMap QWaylandSurface::windowProperties() const
{
    return qWaylandSurfaceWindowProperties;
}

void Ut_NotificationPreviewPresenter::initTestCase()
{
    qRegisterMetaType<LipstickNotification *>();
    gLipstickCompositorStub->stubSetReturnValue("instance", new LipstickCompositor());
    NotificationManager::instance()->setParent(this);
}

void Ut_NotificationPreviewPresenter::cleanup()
{
    homeWindows.clear();
    homeWindowVisible.clear();
    qDeleteAll(notificationManagerNotification);
    notificationManagerNotification.clear();
    notificationManagerCloseNotificationIds.clear();
    notificationManagerDisplayedNotificationIds.clear();
    gQmLocksStub->stubReset();
    gQmDisplayStateStub->stubReset();
}

void Ut_NotificationPreviewPresenter::testSignalConnections()
{
    NotificationPreviewPresenter presenter;
    QCOMPARE(disconnect(NotificationManager::instance(), SIGNAL(notificationAdded(uint)), &presenter, SLOT(updateNotification(uint))), true);
    QCOMPARE(disconnect(NotificationManager::instance(), SIGNAL(notificationRemoved(uint)), &presenter, SLOT(removeNotification(uint))), true);
    QCOMPARE(disconnect(&presenter, SIGNAL(notificationPresented(uint)), presenter.notificationFeedbackPlayer, SLOT(addNotification(uint))), true);
}

void Ut_NotificationPreviewPresenter::testAddNotificationWhenWindowNotOpen()
{
    NotificationPreviewPresenter presenter;
    QSignalSpy changedSpy(&presenter, SIGNAL(notificationChanged()));
    QSignalSpy presentedSpy(&presenter, SIGNAL(notificationPresented(uint)));

    // Check that the window is not automatically created
    QCOMPARE(homeWindows.isEmpty(), true);

    // Check that the window is created when a notification is added
    LipstickNotification *notification = createNotification(1);
    // The HomeWindow is created with a QTimer::singleShot(0, ...), wait for it
    QTest::qWait(0);
    presenter.updateNotification(1);
    QCOMPARE(homeWindows.count(), 1);

    // Check window properties
    QCOMPARE(homeWindowTitle[homeWindows.first()], QString("Notification"));
    QCOMPARE(homeWindowContextProperties[homeWindows.first()].value("initialSize").toSize(), QGuiApplication::primaryScreen()->size());
    QCOMPARE(homeWindowContextProperties[homeWindows.first()].value("notificationPreviewPresenter"), QVariant::fromValue(static_cast<QObject *>(&presenter)));
    QCOMPARE(homeWindowContextProperties[homeWindows.first()].value("notificationFeedbackPlayer"), QVariant::fromValue(static_cast<QObject *>(presenter.notificationFeedbackPlayer)));
    QCOMPARE(homeWindowContextProperties[homeWindows.first()].contains("LipstickSettings"), true);
    QCOMPARE(homeWindowCategories[homeWindows.first()], QString("notification"));

    // Check that the window was shown
    QCOMPARE(homeWindowVisible[homeWindows.first()], true);

    // Check that the expected notification is signaled onwards
    QCOMPARE(changedSpy.count(), 1);
    QCOMPARE(presenter.notification(), notification);
    QCOMPARE(presentedSpy.count(), 1);
    QCOMPARE(presentedSpy.last().at(0).toUInt(), (uint)1);
}

void Ut_NotificationPreviewPresenter::testAddNotificationWhenWindowAlreadyOpen()
{
    NotificationPreviewPresenter presenter;
    QSignalSpy changedSpy(&presenter, SIGNAL(notificationChanged()));
    QSignalSpy presentedSpy(&presenter, SIGNAL(notificationPresented(uint)));

    // Create a notification: this will create a window
    createNotification(1);
    presenter.updateNotification(1);

    // Reset stubs to see what happens next
    homeWindows.clear();

    // Create another notification
    LipstickNotification *notification = createNotification(2);
    presenter.updateNotification(2);

    // The second notification should not be signaled onwards yet since the first one is being presented
    QCOMPARE(changedSpy.count(), 1);
    QCOMPARE(presentedSpy.count(), 1);
    QCOMPARE(presentedSpy.last().at(0).toUInt(), (uint)1);

    // Show the next notification
    presenter.showNextNotification();

    // Check that the window was not unnecessarily created again
    QCOMPARE(homeWindows.isEmpty(), true);

    // Check that the expected notification is signaled onwards
    QCOMPARE(changedSpy.count(), 2);
    QCOMPARE(presenter.notification(), notification);
    QCOMPARE(presentedSpy.count(), 2);
    QCOMPARE(presentedSpy.last().at(0).toUInt(), (uint)2);
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
    QSignalSpy changedSpy(&presenter, SIGNAL(notificationChanged()));
    QSignalSpy presentedSpy(&presenter, SIGNAL(notificationPresented(uint)));
    presenter.updateNotification(1);
    presenter.updateNotification(2);

    // Check that no signals were sent
    QCOMPARE(changedSpy.count(), 0);
    QCOMPARE(presentedSpy.count(), 0);
}

void Ut_NotificationPreviewPresenter::testRemoveNotification()
{
    NotificationPreviewPresenter presenter;
    QSignalSpy changedSpy(&presenter, SIGNAL(notificationChanged()));

    // Create two notifications
    createNotification(1);
    createNotification(2);
    QTest::qWait(0);
    presenter.updateNotification(1);
    presenter.updateNotification(2);

    // Remove the first one
    presenter.removeNotification(1);

    // Check that an empty notification is signaled onwards
    QCOMPARE(changedSpy.count(), 2);
    QCOMPARE(presenter.notification(), (LipstickNotification *)0);

    // Show and remove the second one
    presenter.showNextNotification();
    presenter.removeNotification(2);

    // Check that an empty notification is signaled onwards
    QCOMPARE(changedSpy.count(), 4);
    QCOMPARE(presenter.notification(), (LipstickNotification *)0);

    // Check that the window is not yet hidden
    QCOMPARE(homeWindowVisible[homeWindows.first()], true);

    // Check that the window is hidden when it's time to show the next notification (which doesn't exist)
    presenter.showNextNotification();
    QCOMPARE(homeWindowVisible[homeWindows.first()], false);
}

void Ut_NotificationPreviewPresenter::testNotificationNotShownIfNoSummaryOrBody_data()
{
    QTest::addColumn<QString>("previewSummary");
    QTest::addColumn<QString>("previewBody");
    QTest::addColumn<int>("changedSignalCount");
    QTest::addColumn<int>("presentedSignalCount");
    QTest::addColumn<bool>("windowVisible");
    QTest::newRow("No summary, no body") << "" << "" << 0 << 1 << false;
    QTest::newRow("Summary, no body") << "summary" << "" << 1 << 1 << true;
    QTest::newRow("No summary, body") << "" << "body" << 1 << 1 << true;
    QTest::newRow("Summary, body") << "summary" << "body" << 1 << 1 << true;
}

void Ut_NotificationPreviewPresenter::testNotificationNotShownIfNoSummaryOrBody()
{
    QFETCH(QString, previewSummary);
    QFETCH(QString, previewBody);
    QFETCH(int, changedSignalCount);
    QFETCH(int, presentedSignalCount);
    QFETCH(bool, windowVisible);

    NotificationPreviewPresenter presenter;
    QSignalSpy changedSpy(&presenter, SIGNAL(notificationChanged()));
    QSignalSpy presentedSpy(&presenter, SIGNAL(notificationPresented(uint)));

    // Create notification
    QVariantHash hints;
    hints.insert(NotificationManager::HINT_PREVIEW_SUMMARY, previewSummary);
    hints.insert(NotificationManager::HINT_PREVIEW_BODY, previewBody);
    LipstickNotification *notification = new LipstickNotification("ut_notificationpreviewpresenter", 1, "", "", "", QStringList(), hints, -1);
    notificationManagerNotification.insert(1, notification);
    QTest::qWait(0);
    presenter.updateNotification(1);

    // Check whether the expected notification is signaled onwards
    QCOMPARE(changedSpy.count(), changedSignalCount);
    QCOMPARE(presentedSpy.count(), presentedSignalCount);

    QCOMPARE(homeWindowVisible.isEmpty(), !windowVisible);
    if (windowVisible) {
        // Check whether the window was shown
        QCOMPARE(homeWindowVisible[homeWindows.first()], windowVisible);
    }
}

void Ut_NotificationPreviewPresenter::testNotificationNotShownIfHidden()
{
    NotificationPreviewPresenter presenter;
    QSignalSpy changedSpy(&presenter, SIGNAL(notificationChanged()));
    QSignalSpy presentedSpy(&presenter, SIGNAL(notificationPresented(uint)));

    // Create notification
    QVariantHash hints;
    hints.insert(NotificationManager::HINT_PREVIEW_SUMMARY, "previewSummary");
    hints.insert(NotificationManager::HINT_PREVIEW_BODY, "previewBody");
    hints.insert(NotificationManager::HINT_HIDDEN, true);
    LipstickNotification *notification = new LipstickNotification("ut_notificationpreviewpresenter", 1, "", "", "", QStringList(), hints, -1);
    notificationManagerNotification.insert(1, notification);
    presenter.updateNotification(1);

    QCOMPARE(changedSpy.count(), 0);
    QCOMPARE(homeWindowVisible.isEmpty(), true);

    // The notification should be considered presented
    QCOMPARE(presentedSpy.count(), 1);
    QCOMPARE(presentedSpy.last().at(0).toUInt(), (uint)1);
}

void Ut_NotificationPreviewPresenter::testNotificationNotShownIfRestored()
{
    NotificationPreviewPresenter presenter;
    QSignalSpy changedSpy(&presenter, SIGNAL(notificationChanged()));
    QSignalSpy presentedSpy(&presenter, SIGNAL(notificationPresented(uint)));

    // Create notification
    QVariantHash hints;
    hints.insert(NotificationManager::HINT_PREVIEW_SUMMARY, "previewSummary");
    hints.insert(NotificationManager::HINT_PREVIEW_BODY, "previewBody");
    hints.insert(NotificationManager::HINT_RESTORED, true);
    LipstickNotification *notification = new LipstickNotification("ut_notificationpreviewpresenter", 1, "", "", "", QStringList(), hints, -1);
    notificationManagerNotification.insert(1, notification);
    presenter.updateNotification(1);

    QCOMPARE(changedSpy.count(), 0);
    QCOMPARE(homeWindowVisible.isEmpty(), true);

    // The notification should be considered presented
    QCOMPARE(presentedSpy.count(), 1);
    QCOMPARE(presentedSpy.last().at(0).toUInt(), (uint)1);
}

void Ut_NotificationPreviewPresenter::testShowingOnlyCriticalNotifications()
{
    NotificationPreviewPresenter presenter;
    QSignalSpy changedSpy(&presenter, SIGNAL(notificationChanged()));
    QSignalSpy presentedSpy(&presenter, SIGNAL(notificationPresented(uint)));

    // Create normal urgency notification
    QVariantHash hints;
    hints.insert(NotificationManager::HINT_PREVIEW_SUMMARY, "previewSummary");
    hints.insert(NotificationManager::HINT_PREVIEW_BODY, "previewBody");
    hints.insert(NotificationManager::HINT_URGENCY, 1);
    LipstickNotification *notification = new LipstickNotification("ut_notificationpreviewpresenter", 1, "", "", "", QStringList(), hints, -1);
    notificationManagerNotification.insert(1, notification);
    QTest::qWait(0);
    QCOMPARE(homeWindowVisible.isEmpty(), true);

    // When the screen or device is locked and the urgency is not high enough, so the notification shouldn't be shown
    gQmLocksStub->stubSetReturnValue("getState", MeeGo::QmLocks::Locked);
    presenter.updateNotification(1);
    QCOMPARE(changedSpy.count(), 0);
    QCOMPARE(homeWindowVisible.isEmpty(), true);

    // The notification should be considered presented
    QCOMPARE(presentedSpy.count(), 1);
    QCOMPARE(presentedSpy.last().at(0).toUInt(), (uint)1);

    // Urgency set to critical, so the notification should be shown
    hints.insert(NotificationManager::HINT_URGENCY, 2);
    notification->setHints(hints);
    presenter.updateNotification(1);
    QCOMPARE(changedSpy.count(), 1);
    QCOMPARE(homeWindowVisible.isEmpty(), false);
    QCOMPARE(homeWindowVisible[homeWindows.first()], true);
    QCOMPARE(presentedSpy.count(), 2);
    QCOMPARE(presentedSpy.last().at(0).toUInt(), (uint)1);
}

void Ut_NotificationPreviewPresenter::testUpdateNotificationRemovesNotificationFromQueueIfNotShowable()
{
    NotificationPreviewPresenter presenter;

    // Create two notifications
    LipstickNotification *notification1 = createNotification(1);
    LipstickNotification *notification2 = createNotification(2);
    QTest::qWait(0);
    presenter.updateNotification(1);
    presenter.updateNotification(2);

    // Update the notifications to have no summary or body
    QSignalSpy changedSpy(&presenter, SIGNAL(notificationChanged()));
    QSignalSpy presentedSpy(&presenter, SIGNAL(notificationPresented(uint)));
    notification1->setHints(QVariantHash());
    notification2->setHints(QVariantHash());
    presenter.updateNotification(1);
    presenter.updateNotification(2);

    // Check that the current notification is not removed
    QCOMPARE(changedSpy.count(), 0);

    // The notifications should be considered presented
    QCOMPARE(presentedSpy.count(), 2);

    // Check that the other notification is removed from the queue
    presenter.showNextNotification();
    QCOMPARE(changedSpy.count(), 1);
    QCOMPARE(presenter.notification(), (LipstickNotification *)0);
    QCOMPARE(presentedSpy.count(), 2);
}

Q_DECLARE_METATYPE(MeeGo::QmDisplayState::DisplayState)
Q_DECLARE_METATYPE(MeeGo::QmLocks::State)

void Ut_NotificationPreviewPresenter::testNotificationNotShownIfTouchScreenIsLockedAndDisplayIsOff_data()
{
    QTest::addColumn<MeeGo::QmDisplayState::DisplayState>("displayState");
    QTest::addColumn<MeeGo::QmLocks::State>("lockState");
    QTest::addColumn<int>("notifications");
    QTest::addColumn<int>("presentedCount");
    QTest::newRow("Display on, touch screen not locked") << MeeGo::QmDisplayState::On << MeeGo::QmLocks::Unlocked << 1 << 1;
    QTest::newRow("Display on, touch screen locked") << MeeGo::QmDisplayState::On << MeeGo::QmLocks::Locked << 1 << 1;
    QTest::newRow("Display off, touch screen not locked") << MeeGo::QmDisplayState::Off << MeeGo::QmLocks::Unlocked << 1 << 1;
    QTest::newRow("Display off, touch screen locked") << MeeGo::QmDisplayState::Off << MeeGo::QmLocks::Locked << 0 << 1;
}

void Ut_NotificationPreviewPresenter::testNotificationNotShownIfTouchScreenIsLockedAndDisplayIsOff()
{
    QFETCH(MeeGo::QmDisplayState::DisplayState, displayState);
    QFETCH(MeeGo::QmLocks::State, lockState);
    QFETCH(int, notifications);
    QFETCH(int, presentedCount);

    gQmDisplayStateStub->stubSetReturnValue("get", displayState);
    gQmLocksStub->stubSetReturnValue("getState", lockState);

    NotificationPreviewPresenter presenter;
    QSignalSpy changedSpy(&presenter, SIGNAL(notificationChanged()));
    QSignalSpy presentedSpy(&presenter, SIGNAL(notificationPresented(uint)));

    createNotification(1, Critical);
    QTest::qWait(0);
    presenter.updateNotification(1);
    QCOMPARE(homeWindowVisible.count(), notifications);
    QCOMPARE(changedSpy.count(), notifications);
    QCOMPARE(presentedSpy.count(), presentedCount);
}

void Ut_NotificationPreviewPresenter::testCriticalNotificationIsMarkedAfterShowing()
{
    NotificationPreviewPresenter presenter;
    createNotification(1, Critical);
    createNotification(2);
    createNotification(3);
    QTest::qWait(0);
    presenter.updateNotification(1);
    presenter.updateNotification(2);
    presenter.updateNotification(3);
    QCOMPARE(notificationManagerDisplayedNotificationIds.count(), 0);
    QCOMPARE(notificationManagerCloseNotificationIds.count(), 0);

    presenter.showNextNotification();
    QCOMPARE(notificationManagerDisplayedNotificationIds.count(), 1);
    QCOMPARE(notificationManagerDisplayedNotificationIds.at(0), (uint)1);
    QCOMPARE(notificationManagerCloseNotificationIds.count(), 0);

    presenter.showNextNotification();
    QCOMPARE(notificationManagerDisplayedNotificationIds.count(), 2);
    QCOMPARE(notificationManagerDisplayedNotificationIds.at(1), (uint)2);
    QCOMPARE(notificationManagerCloseNotificationIds.count(), 0);

    presenter.showNextNotification();
    QCOMPARE(notificationManagerDisplayedNotificationIds.count(), 3);
    QCOMPARE(notificationManagerDisplayedNotificationIds.at(2), (uint)3);
    QCOMPARE(notificationManagerCloseNotificationIds.count(), 0);
}

QWaylandSurface *surface = (QWaylandSurface *)1;
void Ut_NotificationPreviewPresenter::testNotificationPreviewsDisabled_data()
{
    QTest::addColumn<QWaylandSurface *>("surface");
    QTest::addColumn<QVariantMap>("windowProperties");
    QTest::addColumn<int>("urgency");
    QTest::addColumn<int>("showCount");

    QVariantMap allNotificationsEnabled;
    QVariantMap applicationNotificationsDisabled;
    QVariantMap systemNotificationsDisabled;
    QVariantMap allNotificationsDisabled;
    allNotificationsEnabled.insert("NOTIFICATION_PREVIEWS_DISABLED", 0);
    applicationNotificationsDisabled.insert("NOTIFICATION_PREVIEWS_DISABLED", 1);
    systemNotificationsDisabled.insert("NOTIFICATION_PREVIEWS_DISABLED", 2);
    allNotificationsDisabled.insert("NOTIFICATION_PREVIEWS_DISABLED", 3);
    QTest::newRow("No surface, application notification") << (QWaylandSurface *)0 << QVariantMap() << static_cast<int>(Normal) << 1;
    QTest::newRow("Surface, no properties, application notification") << surface << QVariantMap() << static_cast<int>(Normal) << 1;
    QTest::newRow("Surface, all notifications enabled, application notification") << surface << allNotificationsEnabled << static_cast<int>(Normal) << 1;
    QTest::newRow("Surface, application notifications disabled, application notification") << surface << applicationNotificationsDisabled << static_cast<int>(Normal) << 0;
    QTest::newRow("Surface, system notifications disabled, application notification") << surface << systemNotificationsDisabled << static_cast<int>(Normal) << 1;
    QTest::newRow("Surface, all notifications disabled, application notification") << surface << allNotificationsDisabled << static_cast<int>(Normal) << 0;
    QTest::newRow("No surface, system notification") << (QWaylandSurface *)0 << QVariantMap() << static_cast<int>(Critical) << 1;
    QTest::newRow("Surface, no properties, system notification") << surface << QVariantMap() << static_cast<int>(Critical) << 1;
    QTest::newRow("Surface, all notifications enabled, system notification") << surface << allNotificationsEnabled << static_cast<int>(Critical) << 1;
    QTest::newRow("Surface, application notifications disabled, system notification") << surface << applicationNotificationsDisabled << static_cast<int>(Critical) << 1;
    QTest::newRow("Surface, system notifications disabled, system notification") << surface << systemNotificationsDisabled << static_cast<int>(Critical) << 0;
    QTest::newRow("Surface, all notifications disabled, system notification") << surface << allNotificationsDisabled << static_cast<int>(Critical) << 0;
}

void Ut_NotificationPreviewPresenter::testNotificationPreviewsDisabled()
{
    QFETCH(QWaylandSurface *, surface);
    QFETCH(QVariantMap, windowProperties);
    QFETCH(int, urgency);
    QFETCH(int, showCount);

    gLipstickCompositorStub->stubSetReturnValue("surfaceForId", surface);
    qWaylandSurfaceWindowProperties = windowProperties;

    NotificationPreviewPresenter presenter;
    createNotification(1, static_cast<Urgency>(urgency));
    QTest::qWait(0);
    presenter.updateNotification(1);

    QCOMPARE(homeWindowVisible.count(), showCount);
}

QTEST_MAIN(Ut_NotificationPreviewPresenter)
