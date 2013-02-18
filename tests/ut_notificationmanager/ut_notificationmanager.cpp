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
#include "ut_notificationmanager.h"
#include "notificationmanager.h"
#include "notificationmanageradaptor_stub.h"
#include "categorydefinitionstore_stub.h"
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlError>
#include <mremoteaction.h>
#include <sys/statfs.h>

const static uint DISK_SPACE_NEEDED = 1024;

unsigned long diskSpaceAvailableKb;
bool diskSpaceChecked;
int statfs (const char *, struct statfs *st)
{
    st->f_bsize = 4096;
    st->f_bavail = (diskSpaceAvailableKb * 1024) / st->f_bsize;
    diskSpaceChecked = true;
    return 0;
}

// QDir stubs
bool qDirRemoveCalled;
bool QDir::remove(const QString &) {
    return qDirRemoveCalled = true;
}

// QSqlDatabase stubs
QString qSqlDatabaseAddDatabaseType = QString();
QSqlDatabase qSqlDatabaseInstance;
QSqlDatabase QSqlDatabase::addDatabase (const QString & type, const QString&)
{
    qSqlDatabaseAddDatabaseType = type;
    return qSqlDatabaseInstance;
}

QString qSqlDatabaseDatabaseName = QString();
void QSqlDatabase::setDatabaseName(const QString& name)
{
    qSqlDatabaseDatabaseName = name;
}

bool qSqlDatabaseOpenSucceeds = true;
bool qSqlIterateOpenSuccess = false;
int qSqlDatabaseOpenCalledCount = 0;
bool QSqlDatabase::open()
{
    bool success = qSqlDatabaseOpenSucceeds;

    // Change to negation for the next call
    if (qSqlIterateOpenSuccess) {
        qSqlDatabaseOpenSucceeds = !success;
    }
    qSqlDatabaseOpenCalledCount++;

    return success;
}

bool QSqlDatabase::isOpen() const
{
    return (qSqlDatabaseOpenCalledCount > 0);
}

QStringList qSqlDatabaseExec;
QSqlQuery QSqlDatabase::exec(const QString& query) const
{
    qSqlDatabaseExec << query;
    return QSqlQuery();
}

bool QSqlDatabase::transaction()
{
    return true;
}

bool qSqlDatabaseCommitCalled = false;
bool QSqlDatabase::commit()
{
    qSqlDatabaseCommitCalled = true;
    return true;
}

// QSqlQuery stubs
QStringList qSqlQueryExecQuery = QStringList();
int qSqlQueryIndex = -1;
QSqlQuery::QSqlQuery(const QString& query, QSqlDatabase)
{
    if (!query.isEmpty()) {
        qSqlQueryExecQuery << query;
    }
    qSqlQueryIndex = -1;
}

QSqlQuery::~QSqlQuery()
{
}

bool QSqlQuery::exec(const QString& query)
{
    qSqlQueryExecQuery << query;
    qSqlQueryIndex = -1;
    return true;
}

bool QSqlQuery::exec()
{
    return true;
}

QSqlError QSqlQuery::lastError() const
{
    return QSqlError();
}

QStringList qSqlQueryPrepare = QStringList();
bool QSqlQuery::prepare(const QString& query)
{
    qSqlQueryPrepare << query;
    return true;
}

QVariantList qSqlQueryAddBindValue = QVariantList();
void QSqlQuery::addBindValue(const QVariant &val, QSql::ParamType)
{
    qSqlQueryAddBindValue << val;
}

QHash<QString, int> qSqlRecordIndexOf;
QSqlRecord QSqlQuery::record() const
{
    qSqlRecordIndexOf.clear();
    if (qSqlQueryExecQuery.last() == "SELECT * FROM notifications") {
        qSqlRecordIndexOf.insert("id", 0);
        qSqlRecordIndexOf.insert("app_name", 1);
        qSqlRecordIndexOf.insert("app_icon", 2);
        qSqlRecordIndexOf.insert("summary", 3);
        qSqlRecordIndexOf.insert("body", 4);
        qSqlRecordIndexOf.insert("expire_timeout", 5);
    } else if (qSqlQueryExecQuery.last() == "SELECT * FROM actions") {
        qSqlRecordIndexOf.insert("id", 0);
        qSqlRecordIndexOf.insert("action", 1);
    } else if (qSqlQueryExecQuery.last() == "SELECT * FROM hints") {
        qSqlRecordIndexOf.insert("id", 0);
        qSqlRecordIndexOf.insert("hint", 1);
        qSqlRecordIndexOf.insert("value", 2);
    }
    return QSqlRecord();
}

QHash<QString, QList<QHash<int, QVariant> > > qSqlQueryValues;
bool QSqlQuery::next()
{
    if (qSqlQueryIndex < qSqlQueryValues.value(qSqlQueryExecQuery.last()).count() - 1) {
        qSqlQueryIndex++;
        return true;
    } else {
        return false;
    }
}

QVariant QSqlQuery::value(int i) const
{
    return qSqlQueryValues.value(qSqlQueryExecQuery.last()).at(qSqlQueryIndex).value(i);
}

int QSqlRecord::indexOf(const QString &name) const
{
    return qSqlRecordIndexOf.value(name, -1);
}

// QSqlTableModel stubs
QMap<QSqlQueryModel*, QString> modelToTableName = QMap<QSqlQueryModel*, QString>();
void QSqlTableModel::setTable(const QString &tableName)
{
    modelToTableName[this] = tableName;
}

QHash<QString, int> notificationsTableFieldIndices;
QHash<QString, int> actionsTableFieldIndices;
QHash<QString, int> hintsTableFieldIndices;
int QSqlTableModel::fieldIndex(const QString &fieldName) const
{
    if (notificationsTableFieldIndices.empty()) {
        notificationsTableFieldIndices.insert("id", 0);
        notificationsTableFieldIndices.insert("app_name", 1);
        notificationsTableFieldIndices.insert("app_icon", 2);
        notificationsTableFieldIndices.insert("summary", 3);
        notificationsTableFieldIndices.insert("body", 4);
        notificationsTableFieldIndices.insert("expire_timeout", 5);

        actionsTableFieldIndices.insert("id", 0);
        actionsTableFieldIndices.insert("action", 1);

        hintsTableFieldIndices.insert("id", 0);
        hintsTableFieldIndices.insert("hint", 1);
        hintsTableFieldIndices.insert("value", 2);
    }

    int ret = -1;

    QString tableName = modelToTableName.value(const_cast<QSqlTableModel*>(this));
    if (tableName == "notifications") {
        if (notificationsTableFieldIndices.contains(fieldName)) {
            ret = notificationsTableFieldIndices.value(fieldName);
        }
    } else if (tableName == "actions") {
        if (actionsTableFieldIndices.contains(fieldName)) {
            ret = actionsTableFieldIndices.value(fieldName);
        }
    } else if (tableName == "hints") {
        if (hintsTableFieldIndices.contains(fieldName)) {
            ret = hintsTableFieldIndices.value(fieldName);
        }
    }

    return ret;
}

// QTimer stubs
bool timerStartCalled = false;
int timerInterval = -1;
QList<QObject*> qTimerStartInstances;
void QTimer::start()
{
    qTimerStartInstances.append(this);
    timerStartCalled = true;
    timerInterval = interval();
}

void QTimer::setInterval(int msec)
{
    inter = msec;
    timerInterval = interval();
}

// MRemoteAction stubs
QStringList mRemoteActionTrigger;
void MRemoteAction::trigger()
{
    mRemoteActionTrigger.append(toString());
}

void Ut_NotificationManager::init()
{
    qSqlQueryExecQuery.clear();
    qSqlQueryPrepare.clear();
    qSqlQueryAddBindValue.clear();
    qSqlQueryValues.clear();
    qSqlDatabaseAddDatabaseType.clear();
    qSqlDatabaseDatabaseName.clear();
    qSqlDatabaseOpenCalledCount = 0;
    qSqlDatabaseOpenSucceeds = true;
    qSqlIterateOpenSuccess = false;
    qDirRemoveCalled = false;
    qSqlDatabaseExec.clear();
    qTimerStartInstances.clear();
    qSqlDatabaseCommitCalled = false;
    diskSpaceAvailableKb = DISK_SPACE_NEEDED + 100;
    diskSpaceChecked = true;
    mRemoteActionTrigger.clear();
}

void Ut_NotificationManager::cleanup()
{
    delete NotificationManager::instance_;
    NotificationManager::instance_ = 0;
}

void Ut_NotificationManager::testManagerIsSingleton()
{
    NotificationManager *manager1 = NotificationManager::instance();
    NotificationManager *manager2 = NotificationManager::instance();
    QVERIFY(manager1 != NULL);
    QCOMPARE(manager2, manager1);
}

void Ut_NotificationManager::testDatabaseConnectionSucceedsAndTablesAreOk()
{
    NotificationManager::instance();
    QCOMPARE(diskSpaceChecked, true);
    QCOMPARE(qSqlDatabaseAddDatabaseType, QString("QSQLITE"));
    QCOMPARE(qSqlDatabaseDatabaseName, QDir::homePath() + "/.config/lipstick/notifications.db");
    QCOMPARE(qSqlDatabaseOpenCalledCount, 1);
    QVERIFY(qSqlQueryExecQuery.count() > 0);
    QCOMPARE(qSqlQueryExecQuery.at(0), QString("PRAGMA journal_mode=WAL"));
}

void Ut_NotificationManager::testDatabaseConnectionSucceedsAndTablesAreNotOk()
{
    // Set up the tables so that the schema won't match
    notificationsTableFieldIndices.clear();
    actionsTableFieldIndices.clear();
    hintsTableFieldIndices.clear();
    notificationsTableFieldIndices.insert("created", 0);
    actionsTableFieldIndices.insert("created", 0);
    hintsTableFieldIndices.insert("created", 0);
    qSqlQueryValues.clear();

    // Check that the tables are dropped and recreated
    NotificationManager::instance();
    QCOMPARE(qSqlDatabaseAddDatabaseType, QString("QSQLITE"));
    QCOMPARE(qSqlDatabaseDatabaseName, QDir::homePath() + "/.config/lipstick/notifications.db");
    QCOMPARE(qSqlDatabaseOpenCalledCount, 1);
    QCOMPARE(qSqlQueryExecQuery.count(), 10);
    QCOMPARE(qSqlQueryExecQuery.at(0), QString("PRAGMA journal_mode=WAL"));
    QCOMPARE(qSqlQueryExecQuery.at(1), QString("DROP TABLE notifications"));
    QCOMPARE(qSqlQueryExecQuery.at(2), QString("CREATE TABLE notifications (id INTEGER PRIMARY KEY, app_name TEXT, app_icon TEXT, summary TEXT, body TEXT, expire_timeout INTEGER)"));
    QCOMPARE(qSqlQueryExecQuery.at(3), QString("DROP TABLE actions"));
    QCOMPARE(qSqlQueryExecQuery.at(4), QString("CREATE TABLE actions (id INTEGER, action TEXT, PRIMARY KEY(id, action))"));
    QCOMPARE(qSqlQueryExecQuery.at(5), QString("DROP TABLE hints"));
    QCOMPARE(qSqlQueryExecQuery.at(6), QString("CREATE TABLE hints (id INTEGER, hint TEXT, value TEXT, PRIMARY KEY(id, hint))"));
    QCOMPARE((bool)modelToTableName.values().contains("notifications"), true);
    QCOMPARE((bool)modelToTableName.values().contains("actions"), true);
    QCOMPARE((bool)modelToTableName.values().contains("hints"), true);
    notificationsTableFieldIndices.clear();
    actionsTableFieldIndices.clear();
    hintsTableFieldIndices.clear();
}

void Ut_NotificationManager::testFirstDatabaseConnectionFails()
{
    // Make the first database connection fail but the second to succeed
    qSqlDatabaseOpenSucceeds = false;
    qSqlIterateOpenSuccess = true;
    NotificationManager::instance();

    // Check that the old database is removed, the database opened twice and the database opened as expected on the second time
    QCOMPARE(qDirRemoveCalled, true);
    QCOMPARE(qSqlDatabaseOpenCalledCount, 2);
    QCOMPARE(qSqlQueryExecQuery.count(), 4);
}

void Ut_NotificationManager::testNotEnoughDiskSpaceToOpenDatabase()
{
    diskSpaceAvailableKb = DISK_SPACE_NEEDED - 100;

    // Check that the database is not opened when there is not enough space
    NotificationManager::instance();
    QCOMPARE(qSqlDatabaseOpenCalledCount, 0);
}

void Ut_NotificationManager::testNotificationsAreRestoredOnConstruction()
{
    // Make the database return two notifications with different values
    QHash<int, QVariant> notification1Values;
    QHash<int, QVariant> notification2Values;
    notification1Values.insert(0, 1);
    notification1Values.insert(1, "appName1");
    notification1Values.insert(2, "appIcon1");
    notification1Values.insert(3, "summary1");
    notification1Values.insert(4, "body1");
    notification1Values.insert(5, 1);
    notification2Values.insert(0, 2);
    notification2Values.insert(1, "appName2");
    notification2Values.insert(2, "appIcon2");
    notification2Values.insert(3, "summary2");
    notification2Values.insert(4, "body2");
    notification2Values.insert(5, 2);
    QList<QHash<int, QVariant> > notificationValues;
    notificationValues << notification1Values << notification2Values;
    qSqlQueryValues["SELECT * FROM notifications"].append(notificationValues);

    QHash<int, QVariant> notification1ActionIdentifier;
    QHash<int, QVariant> notification1ActionName;
    QHash<int, QVariant> notification2ActionIdentifier;
    QHash<int, QVariant> notification2ActionName;
    notification1ActionIdentifier.insert(0, 1);
    notification1ActionIdentifier.insert(1, "action1");
    notification1ActionName.insert(0, 1);
    notification1ActionName.insert(1, "Action 1");
    notification2ActionIdentifier.insert(0, 2);
    notification2ActionIdentifier.insert(1, "action2");
    notification2ActionName.insert(0, 2);
    notification2ActionName.insert(1, "Action 2");
    QList<QHash<int, QVariant> > notificationActions;
    notificationActions << notification1ActionIdentifier << notification1ActionName << notification2ActionIdentifier << notification2ActionName;
    qSqlQueryValues["SELECT * FROM actions"].append(notificationActions);
    QHash<uint, QStringList> notificationActionsById;
    notificationActionsById.insert(1, QStringList() << "action1" << "Action 1");
    notificationActionsById.insert(2, QStringList() << "action2" << "Action 2");

    QHash<int, QVariant> notification1Hint1;
    QHash<int, QVariant> notification1Hint2;
    QHash<int, QVariant> notification2Hint1;
    QHash<int, QVariant> notification2Hint2;
    notification1Hint1.insert(0, 1);
    notification1Hint1.insert(1, "hint1-1");
    notification1Hint1.insert(2, "value1-1");
    notification1Hint2.insert(0, 1);
    notification1Hint2.insert(1, "hint1-2");
    notification1Hint2.insert(2, "value1-2");
    notification2Hint1.insert(0, 2);
    notification2Hint1.insert(1, "hint2-1");
    notification2Hint1.insert(2, "value2-1");
    notification2Hint2.insert(0, 2);
    notification2Hint2.insert(1, "hint2-2");
    notification2Hint2.insert(2, "value2-2");
    QList<QHash<int, QVariant> > notificationHints;
    notificationHints << notification1Hint1 << notification1Hint2 << notification2Hint1 << notification2Hint2;
    qSqlQueryValues["SELECT * FROM hints"].append(notificationHints);
    QHash<uint, QList<QPair<QString, QVariant> > > notificationHintsById;
    notificationHintsById.insert(1, QList<QPair<QString, QVariant> >() << qMakePair(QString("hint1-1"), QVariant("value1-1")) << qMakePair(QString("hint1-2"), QVariant("value1-2")));
    notificationHintsById.insert(2, QList<QPair<QString, QVariant> >() << qMakePair(QString("hint2-1"), QVariant("value2-1")) << qMakePair(QString("hint2-2"), QVariant("value2-2")));

    // Check that the notifications exist in the manager after construction and contain the expected values
    NotificationManager *manager = NotificationManager::instance();
    QList<uint> ids = manager->notificationIds();
    QCOMPARE(ids.count(), notificationValues.count());
    for (int notificationIndex = 0; notificationIndex < notificationValues.count(); notificationIndex++) {
        uint id = ids.at(notificationIndex);
        QCOMPARE(id, notificationValues.at(notificationIndex).value(0).toUInt());
        Notification *notification = manager->notification(id);
        QCOMPARE(notification->appName(), notificationValues.at(notificationIndex).value(1).toString());
        QCOMPARE(notification->appIcon(), notificationValues.at(notificationIndex).value(2).toString());
        QCOMPARE(notification->summary(), notificationValues.at(notificationIndex).value(3).toString());
        QCOMPARE(notification->body(), notificationValues.at(notificationIndex).value(4).toString());
        QCOMPARE(notification->expireTimeout(), notificationValues.at(notificationIndex).value(5).toInt());
        QCOMPARE(notification->actions(), notificationActionsById.value(id));

        typedef QPair<QString, QVariant> HintData;
        foreach (const HintData &hint, notificationHintsById.value(id)) {
            QCOMPARE(notification->hints().value(hint.first), hint.second);
        }
    }
}

void Ut_NotificationManager::testDatabaseCommitIsDoneOnDestruction()
{
    delete NotificationManager::instance();
    NotificationManager::instance_ = 0;

    QCOMPARE(qSqlDatabaseCommitCalled, true);
}

void Ut_NotificationManager::testCapabilities()
{
    // Check the supported capabilities includes all the Nemo hints
    QStringList capabilities = NotificationManager::instance()->GetCapabilities();
    QCOMPARE(capabilities.count(), 11);
    QCOMPARE((bool)capabilities.contains("body"), true);
    QCOMPARE((bool)capabilities.contains("actions"), true);
    QCOMPARE((bool)capabilities.contains(NotificationManager::HINT_ICON), true);
    QCOMPARE((bool)capabilities.contains(NotificationManager::HINT_ITEM_COUNT), true);
    QCOMPARE((bool)capabilities.contains(NotificationManager::HINT_TIMESTAMP), true);
    QCOMPARE((bool)capabilities.contains(NotificationManager::HINT_PREVIEW_ICON), true);
    QCOMPARE((bool)capabilities.contains(NotificationManager::HINT_PREVIEW_BODY), true);
    QCOMPARE((bool)capabilities.contains(NotificationManager::HINT_PREVIEW_SUMMARY), true);
    QCOMPARE((bool)capabilities.contains("x-nemo-remote-actions"), true);
    QCOMPARE((bool)capabilities.contains(NotificationManager::HINT_USER_REMOVABLE), true);
    QCOMPARE((bool)capabilities.contains("x-nemo-get-notifications"), true);
}

void Ut_NotificationManager::testAddingNotification()
{
    NotificationManager *manager = NotificationManager::instance();

    // Check that notifications are inserted to a database, a timestamp is added and a signal about them are sent
    QSignalSpy spy(manager, SIGNAL(notificationModified(uint)));
    QVariantHash hints;
    hints.insert("hint", "value");
    uint id = manager->Notify("appName", 0, "appIcon", "summary", "body", QStringList() << "action" << "Action", hints, 1);
    Notification *notification = manager->notification(id);
    QCOMPARE(disconnect(notification, SIGNAL(actionInvoked(QString)), manager, SLOT(invokeAction(QString))), true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.last().at(0).toUInt(), id);
    QCOMPARE(qSqlQueryPrepare.count(), 5);
    QCOMPARE(qSqlQueryPrepare.at(0), QString("INSERT INTO notifications VALUES (?, ?, ?, ?, ?, ?)"));
    QCOMPARE(qSqlQueryPrepare.at(1), QString("INSERT INTO actions VALUES (?, ?)"));
    QCOMPARE(qSqlQueryPrepare.at(2), QString("INSERT INTO actions VALUES (?, ?)"));
    QCOMPARE(qSqlQueryPrepare.at(3), QString("INSERT INTO hints VALUES (?, ?, ?)"));
    QCOMPARE(qSqlQueryPrepare.at(4), QString("INSERT INTO hints VALUES (?, ?, ?)"));
    QCOMPARE(qSqlQueryAddBindValue.count(), 16);
    QCOMPARE(qSqlQueryAddBindValue.at(0).toUInt(), id);
    QCOMPARE(qSqlQueryAddBindValue.at(1), QVariant("appName"));
    QCOMPARE(qSqlQueryAddBindValue.at(2), QVariant("appIcon"));
    QCOMPARE(qSqlQueryAddBindValue.at(3), QVariant("summary"));
    QCOMPARE(qSqlQueryAddBindValue.at(4), QVariant("body"));
    QCOMPARE(qSqlQueryAddBindValue.at(5).toInt(), 1);
    QCOMPARE(qSqlQueryAddBindValue.at(6).toUInt(), id);
    QCOMPARE(qSqlQueryAddBindValue.at(7), QVariant("action"));
    QCOMPARE(qSqlQueryAddBindValue.at(8).toUInt(), id);
    QCOMPARE(qSqlQueryAddBindValue.at(9), QVariant("Action"));
    QCOMPARE(qSqlQueryAddBindValue.at(10).toUInt(), id);
    QCOMPARE(qSqlQueryAddBindValue.at(11), QVariant("hint"));
    QCOMPARE(qSqlQueryAddBindValue.at(12), QVariant("value"));
    QCOMPARE(qSqlQueryAddBindValue.at(13).toUInt(), id);
    QCOMPARE(qSqlQueryAddBindValue.at(14), QVariant(NotificationManager::HINT_TIMESTAMP));
    QCOMPARE(qSqlQueryAddBindValue.at(15).type(), QVariant::DateTime);
    QCOMPARE(notification->appName(), QString("appName"));
    QCOMPARE(notification->appIcon(), QString("appIcon"));
    QCOMPARE(notification->summary(), QString("summary"));
    QCOMPARE(notification->body(), QString("body"));
    QCOMPARE(notification->actions().count(), 2);
    QCOMPARE(notification->actions().at(0), QString("action"));
    QCOMPARE(notification->actions().at(1), QString("Action"));
    QCOMPARE(notification->hints().value("hint"), QVariant("value"));
    QCOMPARE(notification->hints().value(NotificationManager::HINT_TIMESTAMP).type(), QVariant::DateTime);
}

void Ut_NotificationManager::testUpdatingExistingNotification()
{
    NotificationManager *manager = NotificationManager::instance();

    uint id = manager->Notify("appName", 0, "appIcon", "summary", "body", QStringList(), QVariantHash(), 1);
    qSqlQueryPrepare.clear();
    qSqlQueryAddBindValue.clear();

    QSignalSpy spy(manager, SIGNAL(notificationModified(uint)));
    uint newId = manager->Notify("newAppName", id, "newAppIcon", "newSummary", "newBody", QStringList() << "action", QVariantHash(), 2);
    QCOMPARE(newId, id);
    Notification *notification = manager->notification(id);
    QCOMPARE(disconnect(notification, SIGNAL(actionInvoked(QString)), manager, SLOT(invokeAction(QString))), true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.last().at(0).toUInt(), id);
    QCOMPARE(qSqlQueryPrepare.count(), 6);
    QCOMPARE(qSqlQueryPrepare.at(0), QString("DELETE FROM notifications WHERE id=?"));
    QCOMPARE(qSqlQueryPrepare.at(1), QString("DELETE FROM actions WHERE id=?"));
    QCOMPARE(qSqlQueryPrepare.at(2), QString("DELETE FROM hints WHERE id=?"));
    QCOMPARE(qSqlQueryPrepare.at(3), QString("INSERT INTO notifications VALUES (?, ?, ?, ?, ?, ?)"));
    QCOMPARE(qSqlQueryPrepare.at(4), QString("INSERT INTO actions VALUES (?, ?)"));
    QCOMPARE(qSqlQueryPrepare.at(5), QString("INSERT INTO hints VALUES (?, ?, ?)"));
    QCOMPARE(qSqlQueryAddBindValue.count(), 14);
    QCOMPARE(qSqlQueryAddBindValue.at(0).toUInt(), id);
    QCOMPARE(qSqlQueryAddBindValue.at(1).toUInt(), id);
    QCOMPARE(qSqlQueryAddBindValue.at(2).toUInt(), id);
    QCOMPARE(qSqlQueryAddBindValue.at(3).toUInt(), id);
    QCOMPARE(qSqlQueryAddBindValue.at(4), QVariant("newAppName"));
    QCOMPARE(qSqlQueryAddBindValue.at(5), QVariant("newAppIcon"));
    QCOMPARE(qSqlQueryAddBindValue.at(6), QVariant("newSummary"));
    QCOMPARE(qSqlQueryAddBindValue.at(7), QVariant("newBody"));
    QCOMPARE(qSqlQueryAddBindValue.at(8).toInt(), 2);
    QCOMPARE(qSqlQueryAddBindValue.at(9).toUInt(), id);
    QCOMPARE(qSqlQueryAddBindValue.at(10), QVariant("action"));
    QCOMPARE(qSqlQueryAddBindValue.at(11).toUInt(), id);
    QCOMPARE(qSqlQueryAddBindValue.at(12), QVariant(NotificationManager::HINT_TIMESTAMP));
    QCOMPARE(qSqlQueryAddBindValue.at(13).type(), QVariant::DateTime);
    QCOMPARE(notification->appName(), QString("newAppName"));
    QCOMPARE(notification->appIcon(), QString("newAppIcon"));
    QCOMPARE(notification->summary(), QString("newSummary"));
    QCOMPARE(notification->body(), QString("newBody"));
    QCOMPARE(notification->actions().count(), 1);
    QCOMPARE(notification->actions().at(0), QString("action"));
    QCOMPARE(notification->hints().value(NotificationManager::HINT_TIMESTAMP).type(), QVariant::DateTime);
}

void Ut_NotificationManager::testUpdatingInexistingNotification()
{
    NotificationManager *manager = NotificationManager::instance();
    QSignalSpy spy(manager, SIGNAL(notificationModified(uint)));
    uint id = manager->Notify("appName", 1, "appIcon", "summary", "body", QStringList(), QVariantHash(), 1);
    QCOMPARE(id, (uint)0);
    QCOMPARE(spy.count(), 0);
    QCOMPARE(qSqlQueryPrepare.count(), 0);
}

void Ut_NotificationManager::testRemovingExistingNotification()
{
    NotificationManager *manager = NotificationManager::instance();
    uint id = manager->Notify("appName", 0, "appIcon", "summary", "body", QStringList(), QVariantHash(), 1);
    qSqlQueryPrepare.clear();
    qSqlQueryAddBindValue.clear();

    QSignalSpy removedSpy(manager, SIGNAL(notificationRemoved(uint)));
    QSignalSpy closedSpy(manager, SIGNAL(NotificationClosed(uint,uint)));
    manager->CloseNotification(id);
    QCOMPARE(removedSpy.count(), 1);
    QCOMPARE(removedSpy.last().at(0).toUInt(), id);
    QCOMPARE(closedSpy.count(), 1);
    QCOMPARE(closedSpy.last().at(0).toUInt(), id);
    QCOMPARE(closedSpy.last().at(1).toInt(), (int)NotificationManager::CloseNotificationCalled);
    QCOMPARE(qSqlQueryPrepare.count(), 3);
    QCOMPARE(qSqlQueryPrepare.at(0), QString("DELETE FROM notifications WHERE id=?"));
    QCOMPARE(qSqlQueryPrepare.at(1), QString("DELETE FROM actions WHERE id=?"));
    QCOMPARE(qSqlQueryPrepare.at(2), QString("DELETE FROM hints WHERE id=?"));
    QCOMPARE(qSqlQueryAddBindValue.count(), 3);
    QCOMPARE(qSqlQueryAddBindValue.at(0).toUInt(), id);
    QCOMPARE(qSqlQueryAddBindValue.at(1).toUInt(), id);
    QCOMPARE(qSqlQueryAddBindValue.at(2).toUInt(), id);
}

void Ut_NotificationManager::testRemovingInexistingNotification()
{
    NotificationManager *manager = NotificationManager::instance();
    QSignalSpy removedSpy(manager, SIGNAL(notificationRemoved(uint)));
    QSignalSpy closedSpy(manager, SIGNAL(NotificationClosed(uint,uint)));
    manager->CloseNotification(1);
    QCOMPARE(removedSpy.count(), 0);
    QCOMPARE(closedSpy.count(), 0);
    QCOMPARE(qSqlQueryPrepare.count(), 0);
}

void Ut_NotificationManager::testServerInformation()
{
    // Check that the server information uses application information from qApp
    QString name, vendor, version;
    qApp->setApplicationName("testApp");
    qApp->setApplicationVersion("1.2.3");
    NotificationManager::instance()->GetServerInformation(name, vendor, version);
    QCOMPARE(name, qApp->applicationName());
    QCOMPARE(vendor, QString("Nemo Mobile"));
    QCOMPARE(version, qApp->applicationVersion());
}

void Ut_NotificationManager::testModifyingCategoryDefinitionUpdatesNotifications()
{
    NotificationManager *manager = NotificationManager::instance();

    // Check the signal connection
    QCOMPARE(disconnect(manager->categoryDefinitionStore, SIGNAL(categoryDefinitionModified(QString)), manager, SLOT(updateNotificationsWithCategory(QString))), true);

    // Add two notifications, one with category "category1" and one with category "category2"
    QVariantHash hints1;
    QVariantHash hints2;
    hints1.insert(NotificationManager::HINT_CATEGORY, "category1");
    hints2.insert(NotificationManager::HINT_CATEGORY, "category2");
    manager->Notify("app1", 0, QString(), QString(), QString(), QStringList(), hints1, 0);
    uint id2 = manager->Notify("app2", 0, QString(), QString(), QString(), QStringList(), hints2, 0);

    // Updating notifications with category "category2" should only update the notification with that category
    QSignalSpy modifiedSpy(manager, SIGNAL(notificationModified(uint)));
    manager->updateNotificationsWithCategory("category2");
    QCOMPARE(modifiedSpy.count(), 1);
    QCOMPARE(modifiedSpy.last().at(0).toUInt(), id2);
}

void Ut_NotificationManager::testUninstallingCategoryDefinitionRemovesNotifications()
{
    NotificationManager *manager = NotificationManager::instance();

    // Check the signal connection
    QCOMPARE(disconnect(manager->categoryDefinitionStore, SIGNAL(categoryDefinitionUninstalled(QString)), manager, SLOT(removeNotificationsWithCategory(QString))), true);

    // Add two notifications, one with category "category1" and one with category "category2"
    QVariantHash hints1;
    QVariantHash hints2;
    hints1.insert(NotificationManager::HINT_CATEGORY, "category1");
    hints2.insert(NotificationManager::HINT_CATEGORY, "category2");
    uint id1 = manager->Notify("app1", 0, QString(), QString(), QString(), QStringList(), hints1, 0);
    uint id2 = manager->Notify("app2", 0, QString(), QString(), QString(), QStringList(), hints2, 0);

    // Removing notifications with category "category2" should only remove the notification with that category
    QSignalSpy removedSpy(manager, SIGNAL(notificationRemoved(uint)));
    manager->removeNotificationsWithCategory("category2");
    QCOMPARE(removedSpy.count(), 1);
    QCOMPARE(removedSpy.last().at(0).toUInt(), id2);
    QVERIFY(manager->notification(id1) != 0);
    QCOMPARE(manager->notification(id2), (Notification *)0);
}

void Ut_NotificationManager::testActionIsInvokedIfDefined()
{
    // Add two notifications, only the first one with an action named "action1"
    NotificationManager *manager = NotificationManager::instance();
    uint id1 = manager->Notify("app1", 0, QString(), QString(), QString(), QStringList() << "action1" << "Action 1", QVariantHash(), 0);
    uint id2 = manager->Notify("app2", 0, QString(), QString(), QString(), QStringList() << "action2" << "Action 2", QVariantHash(), 0);
    Notification *notification1 = manager->notification(id1);
    Notification *notification2 = manager->notification(id2);
    connect(this, SIGNAL(actionInvoked(QString)), notification1, SIGNAL(actionInvoked(QString)));
    connect(this, SIGNAL(actionInvoked(QString)), notification2, SIGNAL(actionInvoked(QString)));

    // Make both notifications emit the actionInvoked() signal for action "action1"; only the first one contains it and should be invoked
    QSignalSpy spy(manager, SIGNAL(ActionInvoked(uint, QString)));
    emit actionInvoked("action1");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.last().at(0).toUInt(), id1);
    QCOMPARE(spy.last().at(1).toString(), QString("action1"));
}

void Ut_NotificationManager::testActionIsNotInvokedIfIncomplete()
{
    // Add two notifications, the first one with an incomplete action named "action1"
    NotificationManager *manager = NotificationManager::instance();
    uint id1 = manager->Notify("app1", 0, QString(), QString(), QString(), QStringList() << "action1", QVariantHash(), 0);
    uint id2 = manager->Notify("app2", 0, QString(), QString(), QString(), QStringList() << "action2" << "Action 2", QVariantHash(), 0);
    Notification *notification1 = manager->notification(id1);
    Notification *notification2 = manager->notification(id2);
    connect(this, SIGNAL(actionInvoked(QString)), notification1, SIGNAL(actionInvoked(QString)));
    connect(this, SIGNAL(actionInvoked(QString)), notification2, SIGNAL(actionInvoked(QString)));

    // Make both notifications emit the actionInvoked() signal for action "action1"; no action should be invoked
    QSignalSpy spy(manager, SIGNAL(ActionInvoked(uint, QString)));
    emit actionInvoked("action1");
    QCOMPARE(spy.count(), 0);
}

void Ut_NotificationManager::testRemoteActionIsInvokedIfDefined()
{
    // Add a notifications with an action named "action"
    NotificationManager *manager = NotificationManager::instance();
    QVariantHash hints;
    hints.insert(QString(NotificationManager::HINT_REMOTE_ACTION_PREFIX) + "action", "a b c d");
    uint id = manager->Notify("app", 0, QString(), QString(), QString(), QStringList(), hints, 0);
    Notification *notification = manager->notification(id);
    connect(this, SIGNAL(actionInvoked(QString)), notification, SIGNAL(actionInvoked(QString)));

    // Invoking the notification should trigger the remote action
    emit actionInvoked("action");
    QCOMPARE(mRemoteActionTrigger.count(), 1);
    QCOMPARE(mRemoteActionTrigger.last(), hints.value(QString(NotificationManager::HINT_REMOTE_ACTION_PREFIX) + "action").toString());
}

void Ut_NotificationManager::testInvokingActionClosesNotificationIfUserRemovable()
{
    // Add three notifications with user removability not set, set to true and set to false
    NotificationManager *manager = NotificationManager::instance();
    QVariantHash hints1;
    QVariantHash hints2;
    QVariantHash hints3;
    QVariantHash hints4;
    QVariantHash hints5;
    hints2.insert(NotificationManager::HINT_USER_REMOVABLE, true);
    hints3.insert(NotificationManager::HINT_USER_REMOVABLE, false);
    hints4.insert(NotificationManager::HINT_USER_CLOSEABLE, true);
    hints5.insert(NotificationManager::HINT_USER_CLOSEABLE, false);
    uint id1 = manager->Notify("app1", 0, QString(), QString(), QString(), QStringList(), hints1, 0);
    uint id2 = manager->Notify("app2", 0, QString(), QString(), QString(), QStringList(), hints2, 0);
    uint id3 = manager->Notify("app3", 0, QString(), QString(), QString(), QStringList(), hints3, 0);
    uint id4 = manager->Notify("app4", 0, QString(), QString(), QString(), QStringList(), hints4, 0);
    uint id5 = manager->Notify("app5", 0, QString(), QString(), QString(), QStringList(), hints5, 0);
    connect(this, SIGNAL(actionInvoked(QString)), manager->notification(id1), SIGNAL(actionInvoked(QString)));
    connect(this, SIGNAL(actionInvoked(QString)), manager->notification(id2), SIGNAL(actionInvoked(QString)));
    connect(this, SIGNAL(actionInvoked(QString)), manager->notification(id3), SIGNAL(actionInvoked(QString)));
    connect(this, SIGNAL(actionInvoked(QString)), manager->notification(id4), SIGNAL(actionInvoked(QString)));
    connect(this, SIGNAL(actionInvoked(QString)), manager->notification(id5), SIGNAL(actionInvoked(QString)));

    // Make all notifications emit the actionInvoked() signal for action "action"; removable notifications should get removed
    QSignalSpy removedSpy(manager, SIGNAL(notificationRemoved(uint)));
    QSignalSpy closedSpy(manager, SIGNAL(NotificationClosed(uint,uint)));
    emit actionInvoked("action");
    QCOMPARE(removedSpy.count(), 4);
    QCOMPARE(removedSpy.at(0).at(0).toUInt(), id1);
    QCOMPARE(removedSpy.at(1).at(0).toUInt(), id2);
    QCOMPARE(removedSpy.at(2).at(0).toUInt(), id4);
    QCOMPARE(removedSpy.at(3).at(0).toUInt(), id5);
    QCOMPARE(closedSpy.count(), 3);
    QCOMPARE(closedSpy.at(0).at(0).toUInt(), id1);
    QCOMPARE(closedSpy.at(0).at(1).toInt(), (int)NotificationManager::NotificationDismissedByUser);
    QCOMPARE(closedSpy.at(1).at(0).toUInt(), id2);
    QCOMPARE(closedSpy.at(1).at(1).toInt(), (int)NotificationManager::NotificationDismissedByUser);
    QCOMPARE(closedSpy.at(2).at(0).toUInt(), id4);
    QCOMPARE(closedSpy.at(2).at(1).toInt(), (int)NotificationManager::NotificationDismissedByUser);
}

void Ut_NotificationManager::testInvokingActionRemovesNotificationIfUserRemovableAndNotCloseable()
{
    // Add notification which is removable but not closeable
    NotificationManager *manager = NotificationManager::instance();
    QVariantHash hints;
    hints.insert(NotificationManager::HINT_USER_REMOVABLE, true);
    hints.insert(NotificationManager::HINT_USER_CLOSEABLE, false);
    uint id = manager->Notify("app2", 0, QString(), QString(), QString(), QStringList(), hints, 0);
    Notification *notification = manager->notification(id);
    connect(this, SIGNAL(actionInvoked(QString)), notification, SIGNAL(actionInvoked(QString)));
    qSqlQueryPrepare.clear();
    qSqlQueryAddBindValue.clear();

    // Make the notifications emit the actionInvoked() signal for action "action"; removable notifications should get removed but non-closeable should not be closed
    QSignalSpy removedSpy(manager, SIGNAL(notificationRemoved(uint)));
    QSignalSpy closedSpy(manager, SIGNAL(NotificationClosed(uint,uint)));
    emit actionInvoked("action");
    QCOMPARE(removedSpy.count(), 1);
    QCOMPARE(removedSpy.at(0).at(0).toUInt(), id);
    QCOMPARE(closedSpy.count(), 0);

    // Check that the notification was marked hidden
    QCOMPARE(qSqlQueryPrepare.count(), 1);
    QCOMPARE(qSqlQueryPrepare.at(0), QString("INSERT INTO hints VALUES (?, ?, ?)"));
    QCOMPARE(qSqlQueryAddBindValue.count(), 3);
    QCOMPARE(qSqlQueryAddBindValue.at(0).toUInt(), id);
    QCOMPARE(qSqlQueryAddBindValue.at(1), QVariant(NotificationManager::HINT_HIDDEN));
    QCOMPARE(qSqlQueryAddBindValue.at(2), QVariant(true));
}

void Ut_NotificationManager::testListingNotifications()
{
    NotificationManager *manager = NotificationManager::instance();

    // Add three notifications, two for application appName1 and one for appName2
    QVariantHash hints1;
    QVariantHash hints2;
    QVariantHash hints3;
    hints1.insert(NotificationManager::HINT_CATEGORY, "category1");
    hints2.insert(NotificationManager::HINT_CATEGORY, "category2");
    hints3.insert(NotificationManager::HINT_CATEGORY, "category3");
    uint id1 = manager->Notify("appName1", 0, "appIcon1", "summary1", "body1", QStringList() << "action1", hints1, 1);
    uint id2 = manager->Notify("appName1", 0, "appIcon2", "summary2", "body2", QStringList() << "action2", hints2, 2);
    uint id3 = manager->Notify("appName2", 0, "appIcon3", "summary3", "body3", QStringList() << "action3", hints3, 3);

    // Check that only notifications for the given application are returned and that they contain all the information
    QList<Notification> notifications = manager->GetNotifications("appName1");
    QCOMPARE(notifications.count(), 2);
    QCOMPARE(notifications.at(0).appName(), QString("appName1"));
    QCOMPARE(notifications.at(1).appName(), QString("appName1"));
    QCOMPARE(notifications.at(0).replacesId(), id1);
    QCOMPARE(notifications.at(1).replacesId(), id2);
    QCOMPARE(notifications.at(0).appIcon(), QString("appIcon1"));
    QCOMPARE(notifications.at(1).appIcon(), QString("appIcon2"));
    QCOMPARE(notifications.at(0).summary(), QString("summary1"));
    QCOMPARE(notifications.at(1).summary(), QString("summary2"));
    QCOMPARE(notifications.at(0).body(), QString("body1"));
    QCOMPARE(notifications.at(1).body(), QString("body2"));
    QCOMPARE(notifications.at(0).actions(), QStringList() << "action1");
    QCOMPARE(notifications.at(1).actions(), QStringList() << "action2");
    QCOMPARE(notifications.at(0).hints().value(NotificationManager::HINT_CATEGORY), QVariant("category1"));
    QCOMPARE(notifications.at(1).hints().value(NotificationManager::HINT_CATEGORY), QVariant("category2"));
    QCOMPARE(notifications.at(0).expireTimeout(), 1);
    QCOMPARE(notifications.at(1).expireTimeout(), 2);

    notifications = manager->GetNotifications("appName2");
    QCOMPARE(notifications.count(), 1);
    QCOMPARE(notifications.at(0).appName(), QString("appName2"));
    QCOMPARE(notifications.at(0).replacesId(), id3);
    QCOMPARE(notifications.at(0).appIcon(), QString("appIcon3"));
    QCOMPARE(notifications.at(0).summary(), QString("summary3"));
    QCOMPARE(notifications.at(0).body(), QString("body3"));
    QCOMPARE(notifications.at(0).actions(), QStringList() << "action3");
    QCOMPARE(notifications.at(0).hints().value(NotificationManager::HINT_CATEGORY), QVariant("category3"));
    QCOMPARE(notifications.at(0).expireTimeout(), 3);
}

void Ut_NotificationManager::testRemoveUserRemovableNotifications()
{
    NotificationManager *manager = NotificationManager::instance();
    QVariantHash hints1;
    QVariantHash hints2;
    QVariantHash hints3;
    QVariantHash hints4;
    QVariantHash hints5;
    hints2.insert(NotificationManager::HINT_USER_REMOVABLE, true);
    hints3.insert(NotificationManager::HINT_USER_REMOVABLE, false);
    hints4.insert(NotificationManager::HINT_USER_CLOSEABLE, true);
    hints5.insert(NotificationManager::HINT_USER_CLOSEABLE, false);
    uint id1 = manager->Notify("app1", 0, QString(), QString(), QString(), QStringList(), hints1, 0);
    uint id2 = manager->Notify("app2", 0, QString(), QString(), QString(), QStringList(), hints2, 0);
    manager->Notify("app3", 0, QString(), QString(), QString(), QStringList(), hints3, 0);
    uint id4 = manager->Notify("app4", 0, QString(), QString(), QString(), QStringList(), hints4, 0);
    uint id5 = manager->Notify("app5", 0, QString(), QString(), QString(), QStringList(), hints5, 0);

    QSignalSpy removedSpy(manager, SIGNAL(notificationRemoved(uint)));
    QSignalSpy closedSpy(manager, SIGNAL(NotificationClosed(uint,uint)));
    manager->removeUserRemovableNotifications();
    QCOMPARE(removedSpy.count(), 4);
    QCOMPARE(removedSpy.at(0).at(0).toUInt(), id1);
    QCOMPARE(removedSpy.at(1).at(0).toUInt(), id2);
    QCOMPARE(removedSpy.at(2).at(0).toUInt(), id4);
    QCOMPARE(removedSpy.at(3).at(0).toUInt(), id5);
    QCOMPARE(closedSpy.count(), 3);
    QCOMPARE(closedSpy.at(0).at(0).toUInt(), id1);
    QCOMPARE(closedSpy.at(0).at(1).toInt(), (int)NotificationManager::NotificationDismissedByUser);
    QCOMPARE(closedSpy.at(1).at(0).toUInt(), id2);
    QCOMPARE(closedSpy.at(1).at(1).toInt(), (int)NotificationManager::NotificationDismissedByUser);
    QCOMPARE(closedSpy.at(2).at(0).toUInt(), id4);
    QCOMPARE(closedSpy.at(2).at(1).toInt(), (int)NotificationManager::NotificationDismissedByUser);
}

QTEST_MAIN(Ut_NotificationManager)
