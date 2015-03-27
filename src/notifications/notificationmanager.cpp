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
#include <mdesktopentry.h>
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

//! Path to probe for desktop entries
static const char *DESKTOP_ENTRY_PATH= "/usr/share/applications/";

//! Minimum amount of disk space needed for the notification database in kilobytes
static const uint MINIMUM_FREE_SPACE_NEEDED_IN_KB = 1024;

const char *NotificationManager::HINT_URGENCY = "urgency";
const char *NotificationManager::HINT_CATEGORY = "category";
const char *NotificationManager::HINT_TRANSIENT = "transient";
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
const char *NotificationManager::HINT_OWNER = "x-nemo-owner";

namespace {

QPair<QString, QString> processProperties(uint pid)
{
    // Cache resolution of process name to properties:
    static QHash<QString, QPair<QString, QString> > nameProperties;

    QPair<QString, QString> rv;

    if (pid == QCoreApplication::applicationPid()) {
        // This notification comes from our process
        rv.first = QCoreApplication::applicationName();
    } else if (pid > 1) {
        const QString procFilename(QString::fromLatin1("/proc/%1/cmdline").arg(QString::number(pid)));
        QFile procFile(procFilename);
        if (procFile.open(QIODevice::ReadOnly)) {
            const QByteArray cmdLine = procFile.readAll();
            const QString processName = QString::fromUtf8(cmdLine.left(cmdLine.indexOf('\0')));
            if (!processName.isEmpty()) {
                const QString basename(QFileInfo(processName).fileName());
                if (!basename.isEmpty()) {
                    QHash<QString, QPair<QString, QString> >::iterator it = nameProperties.find(basename);
                    if (it == nameProperties.end()) {
                        // Look up the desktop entry for this process name
                        MDesktopEntry desktopEntry(DESKTOP_ENTRY_PATH + basename + ".desktop");
                        if (desktopEntry.isValid()) {
                            it = nameProperties.insert(basename, qMakePair(desktopEntry.name(), desktopEntry.icon()));
                        } else {
                            qWarning() << "No desktop entry for process name:" << processName;
                            // Fallback to the basename for application name
                            it = nameProperties.insert(basename, qMakePair(basename, QString()));
                        }
                    }
                    if (it != nameProperties.end()) {
                        rv.first = it->first;
                        rv.second = it->second;
                    }
                }
            }
        } else {
            qWarning() << "Unable to retrieve command line for pid:" << pid;
        }
    }

    return rv;
}

}

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
    QDBusContext(),
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

uint NotificationManager::Notify(const QString &appName, uint replacesId, const QString &appIcon, const QString &summary, const QString &body, const QStringList &actions, const QVariantHash &hints, int expireTimeout)
{
    uint id = replacesId != 0 ? replacesId : nextAvailableNotificationID();

    if (replacesId == 0 || notifications.contains(id)) {
        QString appName_(appName);
        QString appIcon_(appIcon);
        QString summary_(summary);
        QString body_(body);
        QVariantHash hints_(hints);
        int expireTimeout_(expireTimeout);

        // Ensure the hints contain a timestamp
        addTimestamp(hints_);

        // Apply a category definition, if any
        const QHash<QString, QString> categoryParameters(categoryDefinitionParameters(hints_));
        QHash<QString, QString>::const_iterator it = categoryParameters.constBegin(), end = categoryParameters.constEnd();
        for ( ; it != end; ++it) {
            const QString &key(it.key());
            const QString &value(it.value());

            if (key == QString("appName")) {
                if (appName_.isEmpty()) {
                    appName_ = value;
                }
            } else if (key == QString("appIcon")) {
                if (appIcon_ .isEmpty()) {
                    appIcon_ = value;
                }
            } else if (key == QString("summary")) {
                if (summary_ .isEmpty()) {
                    summary_ = value;
                }
            } else if (key == QString("body")) {
                if (body_.isEmpty()) {
                    body_ = value;
                }
            } else if (key == QString("expireTimeout")) {
                if (expireTimeout == -1) {
                    expireTimeout_ = value.toInt();
                }
            } else if (!hints_.contains(key)) {
                hints_.insert(key, value);
            }
        }

        if ((appName_.isEmpty() || appIcon_.isEmpty()) && calledFromDBus()) {
            // Use the pid to try to provide these properties
            const QString callerService(message().service());
            const QDBusReply<uint> pidReply(connection().interface()->servicePid(callerService));
            if (pidReply.isValid()) {
                const QPair<QString, QString> properties(processProperties(pidReply.value()));
                if (appName_.isEmpty() && !properties.first.isEmpty()) {
                    appName_ = properties.first;
                }
                if (appIcon_.isEmpty() && !properties.second.isEmpty()) {
                    appIcon_ = properties.second;
                }
            }
        } else if (appName_ == QStringLiteral("AndroidNotification")) {
            // This forwarded Android notification contains the real app name in the summary
            appName_ = summary_;
        }

        if (replacesId == 0) {
            // Create a new notification
            LipstickNotification *notification = new LipstickNotification(appName_, id, appIcon_, summary_, body_, actions, hints_, expireTimeout_, this);
            connect(notification, SIGNAL(actionInvoked(QString)), this, SLOT(invokeAction(QString)), Qt::QueuedConnection);
            connect(notification, SIGNAL(removeRequested()), this, SLOT(removeNotificationIfUserRemovable()), Qt::QueuedConnection);
            notifications.insert(id, notification);
        } else {
            // Only replace an existing notification if it really exists
            LipstickNotification *notification = notifications.value(id);
            notification->setAppName(appName_);
            notification->setAppIcon(appIcon_);
            notification->setSummary(summary_);
            notification->setBody(body_);
            notification->setActions(actions);
            notification->setHints(hints_);
            notification->setExpireTimeout(expireTimeout_);

            // Delete the existing notification from the database
            execSQL(QString("DELETE FROM notifications WHERE id=?"), QVariantList() << id);
            execSQL(QString("DELETE FROM actions WHERE id=?"), QVariantList() << id);
            execSQL(QString("DELETE FROM hints WHERE id=?"), QVariantList() << id);
            execSQL(QString("DELETE FROM expiration WHERE id=?"), QVariantList() << id);
        }

        // Add the notification, its actions and its hints to the database
        execSQL("INSERT INTO notifications VALUES (?, ?, ?, ?, ?, ?)", QVariantList() << id << appName_ << appIcon_ << summary_ << body_ << expireTimeout_);
        foreach (const QString &action, actions) {
            execSQL("INSERT INTO actions VALUES (?, ?)", QVariantList() << id << action);
        }
        QVariantHash::const_iterator hit = hints_.constBegin(), hend = hints_.constEnd();
        for ( ; hit != hend; ++hit) {
            execSQL("INSERT INTO hints VALUES (?, ?, ?)", QVariantList() << id << hit.key() << hit.value());
        }

        NOTIFICATIONS_DEBUG("NOTIFY:" << appName_ << appIcon_ << summary_ << body_ << actions << hints_ << expireTimeout_ << "->" << id);
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
        const QVariantList params(QVariantList() << id);
        execSQL(QString("DELETE FROM notifications WHERE id=?"), params);
        execSQL(QString("DELETE FROM actions WHERE id=?"), params);
        execSQL(QString("DELETE FROM hints WHERE id=?"), params);
        execSQL(QString("DELETE FROM expiration WHERE id=?"), params);

        NOTIFICATIONS_DEBUG("REMOVE:" << id);
        emit notificationRemoved(id);

        // Mark the notification to be destroyed
        removedNotifications.insert(notifications.take(id));
    }
}

void NotificationManager::CloseNotifications(const QList<uint> &ids, NotificationClosedReason closeReason)
{
    if (!ids.isEmpty()) {
        foreach (uint id, ids) {
            if (notifications.contains(id)) {
                emit NotificationClosed(id, closeReason);

                // Remove the notification, its actions and its hints from database
                const QVariantList params(QVariantList() << id);
                execSQL(QString("DELETE FROM notifications WHERE id=?"), params);
                execSQL(QString("DELETE FROM actions WHERE id=?"), params);
                execSQL(QString("DELETE FROM hints WHERE id=?"), params);
                execSQL(QString("DELETE FROM expiration WHERE id=?"), params);
            }
        }

        NOTIFICATIONS_DEBUG("REMOVE:" << ids);
        emit notificationsRemoved(ids);

        foreach (uint id, ids) {
            emit notificationRemoved(id);

            // Mark the notification to be destroyed
            removedNotifications.insert(notifications.take(id));
        }
    }
}

void NotificationManager::MarkNotificationDisplayed(uint id)
{
    if (notifications.contains(id)) {
        const LipstickNotification *notification = notifications.value(id);
        if (notification->hints().value(HINT_TRANSIENT).toBool()) {
            // Remove this notification immediately
            CloseNotification(id, NotificationExpired);
            NOTIFICATIONS_DEBUG("REMOVED transient:" << id);
        } else {
            const int timeout(notification->expireTimeout());
            if (timeout > 0) {
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
}

QString NotificationManager::GetServerInformation(QString &name, QString &vendor, QString &version)
{
    name = qApp->applicationName();
    vendor = "Nemo Mobile";
    version = qApp->applicationVersion();
    return QString();
}

NotificationList NotificationManager::GetNotifications(const QString &owner)
{
    QList<LipstickNotification *> notificationList;
    QHash<uint, LipstickNotification *>::const_iterator it = notifications.constBegin(), end = notifications.constEnd();
    for ( ; it != end; ++it) {
        LipstickNotification *notification = it.value();
        if (notification->owner() == owner) {
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
    QList<uint> ids;
    QHash<uint, LipstickNotification *>::const_iterator it = notifications.constBegin(), end = notifications.constEnd();
    for ( ; it != end; ++it) {
        LipstickNotification *notification(it.value());
        if (notification->category() == category) {
            ids.append(it.key());
        }
    }
    CloseNotifications(ids);
}

void NotificationManager::updateNotificationsWithCategory(const QString &category)
{
    QHash<uint, LipstickNotification *>::const_iterator it = notifications.constBegin(), end = notifications.constEnd();
    for ( ; it != end; ++it) {
        LipstickNotification *notification(it.value());
        if (notification->category() == category) {
            // Remove the preview summary and body hints to avoid showing the preview banner again
            QVariantHash hints = notification->hints();
            hints.remove(HINT_PREVIEW_SUMMARY);
            hints.remove(HINT_PREVIEW_BODY);

            Notify(notification->appName(), it.key(), notification->appIcon(), notification->summary(), notification->body(), notification->actions(), hints, notification->expireTimeout());
        }
    }
}

QHash<QString, QString> NotificationManager::categoryDefinitionParameters(const QVariantHash &hints) const
{
    return categoryDefinitionStore->categoryParameters(hints.value(HINT_CATEGORY).toString());
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

    CloseNotifications(expiredIds, NotificationExpired);

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

    CloseNotifications(expiredIds, NotificationExpired);

    nextExpirationTime = unexpiredRemaining ? nextTimeout : 0;
    if (nextExpirationTime) {
        const qint64 nextTriggerInterval(nextExpirationTime - currentTime);
        expirationTimer.start(static_cast<int>(std::min<qint64>(nextTriggerInterval, std::numeric_limits<int>::max())));
    }
}

void NotificationManager::removeUserRemovableNotifications()
{
    QList<uint> closableNotifications;

    // Find any closable notifications we can close as a batch
    QHash<uint, LipstickNotification *>::const_iterator it = notifications.constBegin(), end = notifications.constEnd();
    for ( ; it != end; ++it) {
        LipstickNotification *notification(it.value());
        QVariant userRemovable = notification->hints().value(HINT_USER_REMOVABLE);
        if (!userRemovable.isValid() || userRemovable.toBool()) {
            QVariant userCloseable = notification->hints().value(HINT_USER_CLOSEABLE);
            if (!userCloseable.isValid() || userCloseable.toBool()) {
                closableNotifications.append(it.key());
            }
        }
    }

    CloseNotifications(closableNotifications, NotificationDismissedByUser);

    // Remove any remaining notifications
    foreach(uint id, notifications.keys()) {
        removeNotificationIfUserRemovable(id);
    }
}
