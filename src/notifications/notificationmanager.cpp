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

#include <QCoreApplication>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <mremoteaction.h>
#include <sys/statfs.h>
#include <limits>
#include "categorydefinitionstore.h"
#include "notificationmanageradaptor.h"
#include "notificationmanager.h"

// Define this if you'd like to see debug messages from the notification manager
#ifdef DEBUG_NOTIFICATIONS
#define NOTIFICATIONS_DEBUG(things) qDebug() << Q_FUNC_INFO << things
#else
#define NOTIFICATIONS_DEBUG(things)
#endif

//! The category definitions directory
static const char *CATEGORY_DEFINITION_FILE_DIRECTORY = "/usr/share/lipstick/notificationcategories";

//! The number configuration files to load into the event type store.
static const uint MAX_CATEGORY_DEFINITION_FILES = 100;

//! Path of the privileged storage directory relative to the home directory
static const char *PRIVILEGED_DATA_PATH= "/.local/share/system/privileged";

//! Minimum amount of disk space needed for the notification database in kilobytes
static const uint MINIMUM_FREE_SPACE_NEEDED_IN_KB = 1024;

const char *NotificationManager::HINT_URGENCY = "urgency";
const char *NotificationManager::HINT_CATEGORY = "category";
const char *NotificationManager::HINT_DESKTOP_ENTRY = "desktop-entry";
const char *NotificationManager::HINT_IMAGE_DATA = "image_data";
const char *NotificationManager::HINT_SOUND_FILE = "sound-file";
const char *NotificationManager::HINT_SUPPRESS_SOUND = "suppress-sound";
const char *NotificationManager::HINT_X = "x";
const char *NotificationManager::HINT_Y = "y";
const char *NotificationManager::HINT_ICON = "x-nemo-icon";
const char *NotificationManager::HINT_ITEM_COUNT = "x-nemo-item-count";
const char *NotificationManager::HINT_PRIORITY = "x-nemo-priority";
const char *NotificationManager::HINT_TIMESTAMP = "x-nemo-timestamp";
const char *NotificationManager::HINT_PREVIEW_ICON = "x-nemo-preview-icon";
const char *NotificationManager::HINT_PREVIEW_BODY = "x-nemo-preview-body";
const char *NotificationManager::HINT_PREVIEW_SUMMARY = "x-nemo-preview-summary";
const char *NotificationManager::HINT_REMOTE_ACTION_PREFIX = "x-nemo-remote-action-";
const char *NotificationManager::HINT_REMOTE_ACTION_ICON_PREFIX = "x-nemo-remote-action-icon-";
const char *NotificationManager::HINT_USER_REMOVABLE = "x-nemo-user-removable";
const char *NotificationManager::HINT_USER_CLOSEABLE = "x-nemo-user-closeable";
const char *NotificationManager::HINT_FEEDBACK = "x-nemo-feedback";
const char *NotificationManager::HINT_HIDDEN = "x-nemo-hidden";
const char *NotificationManager::HINT_DISPLAY_ON = "x-nemo-display-on";
const char *NotificationManager::HINT_LED_DISABLED_WITHOUT_BODY_AND_SUMMARY = "x-nemo-led-disabled-without-body-and-summary";
const char *NotificationManager::HINT_ORIGIN = "x-nemo-origin";

NotificationManager *NotificationManager::instance_ = 0;

NotificationManager *NotificationManager::instance()
{
    if (instance_ == 0) {
        instance_ = new NotificationManager(qApp);
    }
    return instance_;
}

NotificationManager::NotificationManager(QObject *parent) :
    QObject(parent),
    previousNotificationID(0),
    categoryDefinitionStore(new CategoryDefinitionStore(CATEGORY_DEFINITION_FILE_DIRECTORY, MAX_CATEGORY_DEFINITION_FILES, this)),
    database(new QSqlDatabase),
    committed(true),
    nextExpirationTime(0)
{
    qDBusRegisterMetaType<QVariantHash>();
    qDBusRegisterMetaType<LipstickNotification>();
    qDBusRegisterMetaType<NotificationList>();

    new NotificationManagerAdaptor(this);
    QDBusConnection::sessionBus().registerService("org.freedesktop.Notifications");
    QDBusConnection::sessionBus().registerObject("/org/freedesktop/Notifications", this);

    connect(categoryDefinitionStore, SIGNAL(categoryDefinitionUninstalled(QString)), this, SLOT(removeNotificationsWithCategory(QString)));
    connect(categoryDefinitionStore, SIGNAL(categoryDefinitionModified(QString)), this, SLOT(updateNotificationsWithCategory(QString)));

    // Commit the modifications to the database 10 seconds after the last modification so that writing to disk doesn't affect user experience
    databaseCommitTimer.setInterval(10000);
    databaseCommitTimer.setSingleShot(true);
    connect(&databaseCommitTimer, SIGNAL(timeout()), this, SLOT(commit()));

    expirationTimer.setSingleShot(true);
    connect(&expirationTimer, SIGNAL(timeout()), this, SLOT(expire()));

    restoreNotifications();
}

NotificationManager::~NotificationManager()
{
    database->commit();
    delete database;
}

LipstickNotification *NotificationManager::notification(uint id) const
{
    return notifications.value(id);
}

QList<uint> NotificationManager::notificationIds() const
{
    return notifications.keys();
}

QStringList NotificationManager::GetCapabilities()
{
    return QStringList() << "body"
                         << "actions"
                         << HINT_ICON
                         << HINT_ITEM_COUNT
                         << HINT_TIMESTAMP
                         << HINT_PREVIEW_ICON
                         << HINT_PREVIEW_BODY
                         << HINT_PREVIEW_SUMMARY
                         << "x-nemo-remote-actions"
                         << HINT_USER_REMOVABLE
                         << HINT_ORIGIN
                         << "x-nemo-get-notifications";
}

uint NotificationManager::Notify(const QString &appName, uint replacesId, const QString &appIcon, const QString &summary, const QString &body, const QStringList &actions, const QVariantHash &originalHints, int expireTimeout)
{
    uint id = replacesId != 0 ? replacesId : nextAvailableNotificationID();

    if (replacesId == 0 || notifications.contains(id)) {
        // Apply a category definition, if any, to the hints
        QVariantHash hints(originalHints);
        applyCategoryDefinition(hints);

        // Ensure the hints contain a timestamp
        addTimestamp(hints);

        if (replacesId == 0) {
            // Create a new notification
            LipstickNotification *notification = new LipstickNotification(appName, id, appIcon, summary, body, actions, hints, expireTimeout, this);
            connect(notification, SIGNAL(actionInvoked(QString)), this, SLOT(invokeAction(QString)), Qt::QueuedConnection);
            connect(notification, SIGNAL(removeRequested()), this, SLOT(removeNotificationIfUserRemovable()), Qt::QueuedConnection);
            notifications.insert(id, notification);
        } else {
            // Only replace an existing notification if it really exists
            LipstickNotification *notification = notifications.value(id);
            notification->setAppName(appName);
            notification->setAppIcon(appIcon);
            notification->setSummary(summary);
            notification->setBody(body);
            notification->setActions(actions);
            notification->setHints(hints);
            notification->setExpireTimeout(expireTimeout);

            // Delete the existing notification from the database
            execSQL(QString("DELETE FROM notifications WHERE id=?"), QVariantList() << id);
            execSQL(QString("DELETE FROM actions WHERE id=?"), QVariantList() << id);
            execSQL(QString("DELETE FROM hints WHERE id=?"), QVariantList() << id);
            execSQL(QString("DELETE FROM expiration WHERE id=?"), QVariantList() << id);
        }

        // Add the notification, its actions and its hints to the database
        execSQL("INSERT INTO notifications VALUES (?, ?, ?, ?, ?, ?)", QVariantList() << id << appName << appIcon << summary << body << expireTimeout);
        foreach (const QString &action, actions) {
            execSQL("INSERT INTO actions VALUES (?, ?)", QVariantList() << id << action);
        }
        foreach (const QString &hint, hints.keys()) {
            execSQL("INSERT INTO hints VALUES (?, ?, ?)", QVariantList() << id << hint << hints.value(hint));
        }

        NOTIFICATIONS_DEBUG("NOTIFY:" << appName << appIcon << summary << body << actions << hints << expireTimeout << "->" << id);
        emit notificationModified(id);
    } else {
        // Return the ID 0 when trying to update a notification which doesn't exist
        id = 0;
    }

    return id;
}

void NotificationManager::CloseNotification(uint id, NotificationClosedReason closeReason)
{
    if (notifications.contains(id)) {
        emit NotificationClosed(id, closeReason);

        // Remove the notification, its actions and its hints from database
        execSQL(QString("DELETE FROM notifications WHERE id=?"), QVariantList() << id);
        execSQL(QString("DELETE FROM actions WHERE id=?"), QVariantList() << id);
        execSQL(QString("DELETE FROM hints WHERE id=?"), QVariantList() << id);
        execSQL(QString("DELETE FROM expiration WHERE id=?"), QVariantList() << id);

        NOTIFICATIONS_DEBUG("REMOVE:" << id);
        emit notificationRemoved(id);

        // Mark the notification to be destroyed
        removedNotifications.insert(notifications.take(id));
    }
}

void NotificationManager::MarkNotificationDisplayed(uint id)
{
    if (notifications.contains(id)) {
        const LipstickNotification *notification = notifications.value(id);
        const int timeout(notification->expireTimeout());
        if (timeout == 0) {
            // We can remove this notification immediately
            CloseNotification(id, NotificationExpired);
        } else if (timeout > 0) {
            // Insert the timeout into the expiration table, or leave the existing value if already present
            const qint64 currentTime(QDateTime::currentDateTimeUtc().toMSecsSinceEpoch());
            const qint64 expireAt(currentTime + timeout);
            execSQL(QString("INSERT OR IGNORE INTO expiration(id, expire_at) VALUES(?, ?)"), QVariantList() << id << expireAt);

            if (nextExpirationTime == 0 || (expireAt < nextExpirationTime)) {
                // This will be the next notification to expire - update the timer
                nextExpirationTime = expireAt;
                expirationTimer.start(timeout);
            }

            NOTIFICATIONS_DEBUG("DISPLAYED:" << id << "expiring in:" << timeout);
        }
    }
}

QString NotificationManager::GetServerInformation(QString &name, QString &vendor, QString &version)
{
    name = qApp->applicationName();
    vendor = "Nemo Mobile";
    version = qApp->applicationVersion();
    return QString();
}

NotificationList NotificationManager::GetNotifications(const QString &appName)
{
    QList<LipstickNotification *> notificationList;
    foreach (uint id, notifications.keys()) {
        LipstickNotification *notification = notifications.value(id);
        if (notification->appName() == appName) {
            notificationList.append(notification);
        }
    }

    return NotificationList(notificationList);
}

uint NotificationManager::nextAvailableNotificationID()
{
    bool idIncreased = false;

    // Try to find an unused ID. Increase the ID at least once but only up to 2^32-1 times.
    for (uint i = 0; i < UINT32_MAX && (!idIncreased || notifications.contains(previousNotificationID)); i++, idIncreased = true) {
        previousNotificationID++;

        if (previousNotificationID == 0) {
            // 0 is not a valid ID so skip it
            previousNotificationID = 1;
        }
    }

    return previousNotificationID;
}

void NotificationManager::removeNotificationsWithCategory(const QString &category)
{
    foreach(uint id, notifications.keys()) {
        if (notifications[id]->hints().value("category").toString() == category) {
            CloseNotification(id);
        }
    }
}

void NotificationManager::updateNotificationsWithCategory(const QString &category)
{
    foreach(uint id, notifications.keys()) {
        if (notifications[id]->hints().value("category").toString() == category) {
            // Remove the preview summary and body hints to avoid showing the preview banner again
            QVariantHash hints = notifications[id]->hints();
            hints.remove(HINT_PREVIEW_SUMMARY);
            hints.remove(HINT_PREVIEW_BODY);

            Notify(notifications[id]->appName(), id, notifications[id]->appIcon(), notifications[id]->summary(), notifications[id]->body(), notifications[id]->actions(), hints, notifications[id]->expireTimeout());
        }
    }
}

void NotificationManager::applyCategoryDefinition(QVariantHash &hints)
{
    QString category = hints.value(HINT_CATEGORY).toString();
    if (!category.isEmpty()) {
        foreach (const QString &key, categoryDefinitionStore->allKeys(category)) {
            if (!hints.contains(key))
                hints.insert(key, categoryDefinitionStore->value(category, key));
        }
    }
}

void NotificationManager::addTimestamp(QVariantHash &hints)
{
    if (hints.value(HINT_TIMESTAMP).toString().isEmpty()) {
        hints.insert(HINT_TIMESTAMP, QDateTime::currentDateTimeUtc());
    }
}

void NotificationManager::restoreNotifications()
{
    if (connectToDatabase()) {
        if (checkTableValidity()) {
            fetchData();
        } else {
            database->close();
        }
    }
}

bool NotificationManager::connectToDatabase()
{
    QString databasePath = QDir::homePath() + QString(PRIVILEGED_DATA_PATH) + QDir::separator() + "Notifications";
    if (!QDir::root().exists(databasePath)) {
        QDir::root().mkpath(databasePath);
    }
    QString databaseName = databasePath + "/notifications.db";

    *database = QSqlDatabase::addDatabase("QSQLITE", metaObject()->className());
    database->setDatabaseName(databaseName);
    bool success = checkForDiskSpace(databasePath, MINIMUM_FREE_SPACE_NEEDED_IN_KB);
    if (success) {
        success = database->open();
        if (!success) {
            NOTIFICATIONS_DEBUG(database->lastError().driverText() << databaseName << database->lastError().databaseText());

            // If opening the database fails, try to recreate the database
            removeDatabaseFile(databaseName);
            success = database->open();
            NOTIFICATIONS_DEBUG("Unable to open database file. Recreating. Success: " << success);
        }
    } else {
        NOTIFICATIONS_DEBUG("Not enough free disk space available. Unable to open database.");
    }

    if (success) {
        // Set up the database mode to write-ahead locking to improve performance
        QSqlQuery(*database).exec("PRAGMA journal_mode=WAL");
    }

    return success;
}

bool NotificationManager::checkForDiskSpace(const QString &path, unsigned long freeSpaceNeeded)
{
    struct statfs st;
    bool spaceAvailable = false;
    if (statfs(path.toUtf8().data(), &st) != -1) {
        unsigned long freeSpaceInKb = (st.f_bsize * st.f_bavail) / 1024;
        if (freeSpaceInKb > freeSpaceNeeded) {
            spaceAvailable = true;
        }
    }
    return spaceAvailable;
}

void NotificationManager::removeDatabaseFile(const QString &path)
{
    // Remove also -shm and -wal files created when journal-mode=WAL is being used
    QDir::root().remove(path + "-shm");
    QDir::root().remove(path + "-wal");
    QDir::root().remove(path);
}

bool NotificationManager::checkTableValidity()
{
    bool result = true;
    bool recreateNotificationsTable = false;
    bool recreateActionsTable = false;
    bool recreateHintsTable = false;
    bool recreateExpirationTable = false;

    {
        // Check that the notifications table schema is as expected
        QSqlTableModel notificationsTableModel(0, *database);
        notificationsTableModel.setTable("notifications");
        recreateNotificationsTable = (notificationsTableModel.fieldIndex("id") == -1 ||
                                      notificationsTableModel.fieldIndex("app_name") == -1 ||
                                      notificationsTableModel.fieldIndex("app_icon") == -1 ||
                                      notificationsTableModel.fieldIndex("summary") == -1 ||
                                      notificationsTableModel.fieldIndex("body") == -1 ||
                                      notificationsTableModel.fieldIndex("expire_timeout") == -1);

        // Check that the actions table schema is as expected
        QSqlTableModel actionsTableModel(0, *database);
        actionsTableModel.setTable("actions");
        recreateActionsTable = (actionsTableModel.fieldIndex("id") == -1 ||
                                actionsTableModel.fieldIndex("action") == -1);

        // Check that the hints table schema is as expected
        QSqlTableModel hintsTableModel(0, *database);
        hintsTableModel.setTable("hints");
        recreateHintsTable = (hintsTableModel.fieldIndex("id") == -1 ||
                              hintsTableModel.fieldIndex("hint") == -1 ||
                              hintsTableModel.fieldIndex("value") == -1);

        // Check that the expiration table schema is as expected
        QSqlTableModel expirationTableModel(0, *database);
        expirationTableModel.setTable("expiration");
        recreateExpirationTable = (expirationTableModel.fieldIndex("id") == -1 ||
                                   expirationTableModel.fieldIndex("expire_at") == -1);
    }

    if (recreateNotificationsTable) {
        result &= recreateTable("notifications", "id INTEGER PRIMARY KEY, app_name TEXT, app_icon TEXT, summary TEXT, body TEXT, expire_timeout INTEGER");
    }

    if (recreateActionsTable) {
        result &= recreateTable("actions", "id INTEGER, action TEXT, PRIMARY KEY(id, action)");
    }

    if (recreateHintsTable) {
        result &= recreateTable("hints", "id INTEGER, hint TEXT, value TEXT, PRIMARY KEY(id, hint)");
    }

    if (recreateExpirationTable) {
        result &= recreateTable("expiration", "id INTEGER PRIMARY KEY, expire_at INTEGER");
    }

    return result;
}

bool NotificationManager::recreateTable(const QString &tableName, const QString &definition)
{
    bool result = false;

    if (database->isOpen()) {
        QSqlQuery(*database).exec("DROP TABLE " + tableName);
        result = QSqlQuery(*database).exec("CREATE TABLE " + tableName + " (" + definition + ")");
    }

    return result;
}

void NotificationManager::fetchData()
{
    // Gather actions for each notification
    QSqlQuery actionsQuery("SELECT * FROM actions", *database);
    QSqlRecord actionsRecord = actionsQuery.record();
    int actionsTableIdFieldIndex = actionsRecord.indexOf("id");
    int actionsTableActionFieldIndex = actionsRecord.indexOf("action");
    QHash<uint, QStringList> actions;
    while (actionsQuery.next()) {
        uint id = actionsQuery.value(actionsTableIdFieldIndex).toUInt();
        actions[id].append(actionsQuery.value(actionsTableActionFieldIndex).toString());
    }

    // Gather hints for each notification
    QSqlQuery hintsQuery("SELECT * FROM hints", *database);
    QSqlRecord hintsRecord = hintsQuery.record();
    int hintsTableIdFieldIndex = hintsRecord.indexOf("id");
    int hintsTableHintFieldIndex = hintsRecord.indexOf("hint");
    int hintsTableValueFieldIndex = hintsRecord.indexOf("value");
    QHash<uint, QVariantHash> hints;
    while (hintsQuery.next()) {
        uint id = hintsQuery.value(hintsTableIdFieldIndex).toUInt();
        hints[id].insert(hintsQuery.value(hintsTableHintFieldIndex).toString(), hintsQuery.value(hintsTableValueFieldIndex));
    }

    // Gather expiration times for displayed notifications
    QSqlQuery expirationQuery("SELECT * FROM expiration", *database);
    QSqlRecord expirationRecord = expirationQuery.record();
    int expirationTableIdFieldIndex = expirationRecord.indexOf("id");
    int expirationTableExpireAtFieldIndex = expirationRecord.indexOf("expire_at");
    QHash<uint, qint64> expireAt;
    while (expirationQuery.next()) {
        uint id = expirationQuery.value(expirationTableIdFieldIndex).toUInt();
        expireAt.insert(id, expirationQuery.value(expirationTableExpireAtFieldIndex).value<qint64>());
    }

    const qint64 currentTime(QDateTime::currentDateTimeUtc().toMSecsSinceEpoch());
    QList<uint> expiredIds;
    qint64 nextTimeout = std::numeric_limits<qint64>::max();
    bool unexpiredRemaining = false;

    // Create the notifications
    QSqlQuery notificationsQuery("SELECT * FROM notifications", *database);
    QSqlRecord notificationsRecord = notificationsQuery.record();
    int notificationsTableIdFieldIndex = notificationsRecord.indexOf("id");
    int notificationsTableAppNameFieldIndex = notificationsRecord.indexOf("app_name");
    int notificationsTableAppIconFieldIndex = notificationsRecord.indexOf("app_icon");
    int notificationsTableSummaryFieldIndex = notificationsRecord.indexOf("summary");
    int notificationsTableBodyFieldIndex = notificationsRecord.indexOf("body");
    int notificationsTableExpireTimeoutFieldIndex = notificationsRecord.indexOf("expire_timeout");
    while (notificationsQuery.next()) {
        uint id = notificationsQuery.value(notificationsTableIdFieldIndex).toUInt();
        QString appName = notificationsQuery.value(notificationsTableAppNameFieldIndex).toString();
        QString appIcon = notificationsQuery.value(notificationsTableAppIconFieldIndex).toString();
        QString summary = notificationsQuery.value(notificationsTableSummaryFieldIndex).toString();
        QString body = notificationsQuery.value(notificationsTableBodyFieldIndex).toString();
        int expireTimeout = notificationsQuery.value(notificationsTableExpireTimeoutFieldIndex).toInt();

        if (expireAt.contains(id)) {
            const qint64 expiry(expireAt.value(id));
            if (expiry <= currentTime) {
                NOTIFICATIONS_DEBUG("EXPIRED AT RESTORE:" << appName << appIcon << summary << body << actions[id] << hints[id] << expireTimeout << "->" << id);
                expiredIds.append(id);
                continue;
            } else {
                nextTimeout = qMin(expiry, nextTimeout);
                unexpiredRemaining = true;
            }
        }

        LipstickNotification *notification = new LipstickNotification(appName, id, appIcon, summary, body, actions[id], hints[id], expireTimeout, this);
        connect(notification, SIGNAL(actionInvoked(QString)), this, SLOT(invokeAction(QString)), Qt::QueuedConnection);
        connect(notification, SIGNAL(removeRequested()), this, SLOT(removeNotificationIfUserRemovable()), Qt::QueuedConnection);
        notifications.insert(id, notification);

        NOTIFICATIONS_DEBUG("RESTORED:" << appName << appIcon << summary << body << actions[id] << hints[id] << expireTimeout << "->" << id);
        emit notificationModified(id);

        if (id > previousNotificationID) {
            // Use the highest notification ID found as the previous notification ID
            previousNotificationID = id;
        }
    }

    foreach (uint id, expiredIds) {
        CloseNotification(id, NotificationExpired);
    }

    nextExpirationTime = unexpiredRemaining ? nextTimeout : 0;
    if (nextExpirationTime) {
        const qint64 nextTriggerInterval(nextExpirationTime - currentTime);
        expirationTimer.start(static_cast<int>(std::min<qint64>(nextTriggerInterval, std::numeric_limits<int>::max())));
    }
}

void NotificationManager::commit()
{
    // Any aditional rules about when database commits are allowed can be added here
    if (!committed) {
        database->commit();
        committed = true;
    }

    qDeleteAll(removedNotifications);
    removedNotifications.clear();
}

void NotificationManager::execSQL(const QString &command, const QVariantList &args)
{
    if (!database->isOpen()) {
        return;
    }

    if (committed) {
        committed = false;
        database->transaction();
    }

    QSqlQuery query(*database);
    query.prepare(command);

    foreach(const QVariant &arg, args) {
        query.addBindValue(arg);
    }

    query.exec();

    if (query.lastError().isValid()) {
        NOTIFICATIONS_DEBUG(command << args << query.lastError());
    }

    databaseCommitTimer.start();
}

void NotificationManager::invokeAction(const QString &action)
{
    LipstickNotification *notification = qobject_cast<LipstickNotification *>(sender());
    if (notification != 0) {
        uint id = notifications.key(notification, 0);
        if (id > 0) {
            QString remoteAction = notification->hints().value(QString(HINT_REMOTE_ACTION_PREFIX) + action).toString();
            if (!remoteAction.isEmpty()) {
                NOTIFICATIONS_DEBUG("INVOKE REMOTE ACTION:" << action << id);

                // If a remote action has been defined for the given action, trigger it
                MRemoteAction(remoteAction).trigger();
            }

            for (int actionIndex = 0; actionIndex < notification->actions().count() / 2; actionIndex++) {
                // Actions are sent over as a list of pairs. Each even element in the list (starting at index 0) represents the identifier for the action. Each odd element in the list is the localized string that will be displayed to the user.
                if (notification->actions().at(actionIndex * 2) == action) {
                    NOTIFICATIONS_DEBUG("INVOKE ACTION:" << action << id);

                    emit ActionInvoked(id, action);
                }
            }

            removeNotificationIfUserRemovable(id);
        }
    }
}

void NotificationManager::removeNotificationIfUserRemovable(uint id)
{
    if (id == 0) {
        LipstickNotification *notification = qobject_cast<LipstickNotification *>(sender());
        if (notification != 0) {
            id = notifications.key(notification, 0);
        }
    }

    LipstickNotification *notification = notifications[id];
    QVariant userRemovable = notification->hints().value(HINT_USER_REMOVABLE);
    if (!userRemovable.isValid() || userRemovable.toBool()) {
        // The notification should be removed if user removability is not defined (defaults to true) or is set to true
        QVariant userCloseable = notification->hints().value(HINT_USER_CLOSEABLE);
        if (!userCloseable.isValid() || userCloseable.toBool()) {
            // The notification should be closed if user closeability is not defined (defaults to true) or is set to true
            CloseNotification(id, NotificationDismissedByUser);
        } else {
            // Uncloseable notifications should be only removed
            emit notificationRemoved(id);

            // Mark the notification as hidden
            execSQL("INSERT INTO hints VALUES (?, ?, ?)", QVariantList() << id << HINT_HIDDEN << true);
        }
    }
}

void NotificationManager::expire()
{
    const qint64 currentTime(QDateTime::currentDateTimeUtc().toMSecsSinceEpoch());
    QList<uint> expiredIds;
    qint64 nextTimeout = std::numeric_limits<qint64>::max();
    bool unexpiredRemaining = false;

    QSqlQuery expirationQuery("SELECT * FROM expiration", *database);
    QSqlRecord expirationRecord = expirationQuery.record();
    int expirationTableIdFieldIndex = expirationRecord.indexOf("id");
    int expirationTableExpireAtFieldIndex = expirationRecord.indexOf("expire_at");
    while (expirationQuery.next()) {
        const uint id = expirationQuery.value(expirationTableIdFieldIndex).toUInt();
        const qint64 expiry = expirationQuery.value(expirationTableExpireAtFieldIndex).value<qint64>();

        if (expiry <= currentTime) {
            expiredIds.append(id);
        } else {
            nextTimeout = qMin(expiry, nextTimeout);
            unexpiredRemaining = true;
        }
    }

    foreach (uint id, expiredIds) {
        CloseNotification(id, NotificationExpired);
    }

    nextExpirationTime = unexpiredRemaining ? nextTimeout : 0;
    if (nextExpirationTime) {
        const qint64 nextTriggerInterval(nextExpirationTime - currentTime);
        expirationTimer.start(static_cast<int>(std::min<qint64>(nextTriggerInterval, std::numeric_limits<int>::max())));
    }
}

void NotificationManager::removeUserRemovableNotifications()
{
    foreach(uint id, notifications.keys()) {
        removeNotificationIfUserRemovable(id);
    }
}
