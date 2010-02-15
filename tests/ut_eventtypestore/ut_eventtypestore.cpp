/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of duihome.
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
#include <QObject>
#include "ut_eventtypestore.h"

// List of event type files
QStringList eventTypeFilesList;
// Map of event type and map of its settings
QMap<QString, QMap<QString, QString> > eventTypeSettingsMap;
//Size of the event configuration file
uint eventTypeFileSize;

// QFileInfo stubs
bool QFileInfo::exists() const
{
    return eventTypeFilesList.contains(fileName());
}

qint64 QFileInfo::size() const
{
    return eventTypeFileSize;
}

// Stubs of QSettings methods
QStringList QSettings::allKeys() const
{
    return QStringList(eventTypeSettingsMap.value(QFileInfo(fileName()).baseName()).keys());
}

bool QSettings::contains(const QString &key) const
{
    return eventTypeSettingsMap.value(QFileInfo(fileName()).baseName()).contains(key);
}

QVariant QSettings::value(const QString &key, const QVariant &defaultValue) const
{
    if (eventTypeSettingsMap.value(QFileInfo(fileName()).baseName()).contains(key)) {
        return eventTypeSettingsMap.value(QFileInfo(fileName()).baseName()).value(key);
    } else {
        return defaultValue;
    }
}

void Ut_EventTypeStore::init()
{
    eventTypeFilesList.clear();
    eventTypeSettingsMap.clear();
    eventTypeFileSize = 100;
}

void Ut_EventTypeStore::cleanup()
{
    delete m_subject;
    m_subject = NULL;
}

void Ut_EventTypeStore::testEventTypeSettingsValues()
{
    // Create three event type files
    eventTypeFilesList.append("smsEventType.conf");
    eventTypeFilesList.append("emailEventType.conf");
    eventTypeFilesList.append("chatEventType.conf");

    // Create settings map for storing the values for each event type and insert keys/values
    QMap<QString, QString> smsSettingsMap;
    QMap<QString, QString> emailSettingsMap;
    QMap<QString, QString> chatSettingsMap;
    smsSettingsMap.insert("iconId", "sms-icon");
    smsSettingsMap.insert("feedbackId", "sound-file");
    emailSettingsMap.insert("iconId", "email-icon");
    emailSettingsMap.insert("feedbackId", "sound-file-for-email");
    chatSettingsMap.insert("iconId", "chat-icon");
    chatSettingsMap.insert("feedbackId", "sound-file-for-chat");
    eventTypeSettingsMap.insert("smsEventType", smsSettingsMap);
    eventTypeSettingsMap.insert("emailEventType", emailSettingsMap);
    eventTypeSettingsMap.insert("chatEventType", chatSettingsMap);

    // Create a store that can hold up to 2 event types in memory at the same time
    m_subject = new EventTypeStore("/foo/bar", 2);

    // Verify settings object is null for invalid key
    QVERIFY(m_subject->settingsForEventType("foo") == NULL);

    // Verify that settings object for each event type contains the provided keys/values
    QVERIFY(m_subject->settingsForEventType("smsEventType") != NULL);
    QCOMPARE(m_subject->settingsForEventType("smsEventType")->allKeys().count(), 2);
    QCOMPARE(m_subject->settingsForEventType("smsEventType")->contains("iconId"), true);
    QCOMPARE(m_subject->settingsForEventType("smsEventType")->value("iconId").toString(), QString("sms-icon"));
    QCOMPARE(m_subject->settingsForEventType("smsEventType")->contains("feedbackId"), true);
    QCOMPARE(m_subject->settingsForEventType("smsEventType")->value("feedbackId").toString(), QString("sound-file"));
    QVERIFY(m_subject->settingsForEventType("emailEventType") != NULL);
    QCOMPARE(m_subject->settingsForEventType("emailEventType")->allKeys().count(), 2);
    QCOMPARE(m_subject->settingsForEventType("emailEventType")->contains("iconId"), true);
    QCOMPARE(m_subject->settingsForEventType("emailEventType")->value("iconId").toString(), QString("email-icon"));
    QCOMPARE(m_subject->settingsForEventType("emailEventType")->contains("feedbackId"), true);
    QCOMPARE(m_subject->settingsForEventType("emailEventType")->value("feedbackId").toString(), QString("sound-file-for-email"));

    // Verify that event types work even if the maximum amount of types stored in memory is reached
    QVERIFY(m_subject->settingsForEventType("chatEventType") != NULL);
    QCOMPARE(m_subject->settingsForEventType("chatEventType")->allKeys().count(), 2);
    QCOMPARE(m_subject->settingsForEventType("chatEventType")->contains("iconId"), true);
    QCOMPARE(m_subject->settingsForEventType("chatEventType")->value("iconId").toString(), QString("chat-icon"));
    QCOMPARE(m_subject->settingsForEventType("chatEventType")->contains("feedbackId"), true);
    QCOMPARE(m_subject->settingsForEventType("chatEventType")->value("feedbackId").toString(), QString("sound-file-for-chat"));
    QVERIFY(m_subject->settingsForEventType("smsEventType") != NULL);
    QCOMPARE(m_subject->settingsForEventType("smsEventType")->allKeys().count(), 2);
    QCOMPARE(m_subject->settingsForEventType("smsEventType")->contains("iconId"), true);
    QCOMPARE(m_subject->settingsForEventType("smsEventType")->value("iconId").toString(), QString("sms-icon"));
    QCOMPARE(m_subject->settingsForEventType("smsEventType")->contains("feedbackId"), true);
    QCOMPARE(m_subject->settingsForEventType("smsEventType")->value("feedbackId").toString(), QString("sound-file"));
}
void Ut_EventTypeStore::testEventTypeStoreMaxFileSizeHandling()
{
    // Verify if the event type file is above its maximum size, than its not loaded
    eventTypeFileSize = 90000;
    eventTypeFilesList.append("bigEventType.conf");
    QMap<QString, QString> bigEventTypeSettingsMap;
    bigEventTypeSettingsMap.insert("iconId", "sms-icon");
    bigEventTypeSettingsMap.insert("feedbackId", "sound-file");
    eventTypeSettingsMap.insert("bigEventType", bigEventTypeSettingsMap);

    m_subject = new EventTypeStore("/foo/bar", 2);
    // Verify that settings object is NULL
    QVERIFY(m_subject->settingsForEventType("bigEventType") == NULL);
}

QTEST_APPLESS_MAIN(Ut_EventTypeStore)
