/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
#include "ut_categorydefinitionstore.h"
#include "categorydefinitionstore.h"

// List of category definition files
QStringList categoryDefinitionFilesList;
// Map of category definition and map of its settings
QMap<QString, QMap<QString, QString> > categoryDefinitionSettingsMap;
// Size of the category definition file
uint categoryDefinitionFileSize;

// QFileSystemWatcher stubs
void QFileSystemWatcher::addPath(const QString &)
{
}

// QFileInfo stubs
bool QFileInfo::exists() const
{
    return categoryDefinitionFilesList.contains(fileName());
}

qint64 QFileInfo::size() const
{
    return categoryDefinitionFileSize;
}

// QDir stubs
bool QDir::exists() const
{
    return true;
}

QStringList QDir::entryList(const QStringList &, Filters, SortFlags) const
{
    return categoryDefinitionFilesList;
}

// Stubs of QSettings methods
QStringList QSettings::allKeys() const
{
    return QStringList(categoryDefinitionSettingsMap.value(QFileInfo(fileName()).baseName()).keys());
}

bool QSettings::contains(const QString &key) const
{
    return categoryDefinitionSettingsMap.value(QFileInfo(fileName()).baseName()).contains(key);
}

QVariant QSettings::value(const QString &key, const QVariant &defaultValue) const
{
    if (categoryDefinitionSettingsMap.value(QFileInfo(fileName()).baseName()).contains(key)) {
        return categoryDefinitionSettingsMap.value(QFileInfo(fileName()).baseName()).value(key);
    } else {
        return defaultValue;
    }
}

void Ut_CategoryDefinitionStore::init()
{
    categoryDefinitionFilesList.clear();
    categoryDefinitionSettingsMap.clear();
    categoryDefinitionFileSize = 100;
}

void Ut_CategoryDefinitionStore::cleanup()
{
    delete store;
    store = NULL;
}

void Ut_CategoryDefinitionStore::testCategoryDefinitionSettingsValues()
{
    // Create three category definition files
    categoryDefinitionFilesList.append("smsCategoryDefinition.conf");
    categoryDefinitionFilesList.append("emailCategoryDefinition.conf");
    categoryDefinitionFilesList.append("chatCategoryDefinition.conf");

    // Create settings map for storing the values for each category definition and insert keys/values
    QMap<QString, QString> smsSettingsMap;
    QMap<QString, QString> emailSettingsMap;
    QMap<QString, QString> chatSettingsMap;
    smsSettingsMap.insert("iconId", "sms-icon");
    smsSettingsMap.insert("feedbackId", "sound-file");
    emailSettingsMap.insert("iconId", "email-icon");
    emailSettingsMap.insert("feedbackId", "sound-file-for-email");
    chatSettingsMap.insert("iconId", "chat-icon");
    chatSettingsMap.insert("feedbackId", "sound-file-for-chat");
    categoryDefinitionSettingsMap.insert("smsCategoryDefinition", smsSettingsMap);
    categoryDefinitionSettingsMap.insert("emailCategoryDefinition", emailSettingsMap);
    categoryDefinitionSettingsMap.insert("chatCategoryDefinition", chatSettingsMap);

    // Create a store that can hold up to 2 category definitions in memory at the same time
    store = new CategoryDefinitionStore("/categorydefinitionpath", 2);

    // Verify settings object is null for invalid key
    QCOMPARE(store->categoryDefinitionExists("idontexist"), false);

    // Verify that settings object for each category definition contains the provided keys/values
    QCOMPARE(store->categoryDefinitionExists("smsCategoryDefinition"), true);
    QCOMPARE(store->allKeys("smsCategoryDefinition").count(), 2);
    QCOMPARE(store->contains("smsCategoryDefinition", "iconId"), true);
    QCOMPARE(store->value("smsCategoryDefinition", "iconId"), QString("sms-icon"));
    QCOMPARE(store->contains("smsCategoryDefinition", "feedbackId"), true);
    QCOMPARE(store->value("smsCategoryDefinition", "feedbackId"), QString("sound-file"));
    QCOMPARE(store->categoryDefinitionExists("emailCategoryDefinition"), true);
    QCOMPARE(store->allKeys("emailCategoryDefinition").count(), 2);
    QCOMPARE(store->contains("emailCategoryDefinition", "iconId"), true);
    QCOMPARE(store->value("emailCategoryDefinition", "iconId"), QString("email-icon"));
    QCOMPARE(store->contains("emailCategoryDefinition", "feedbackId"), true);
    QCOMPARE(store->value("emailCategoryDefinition", "feedbackId"), QString("sound-file-for-email"));

    // Verify that category definitions work even if the maximum amount of types stored in memory is reached
    QCOMPARE(store->categoryDefinitionExists("chatCategoryDefinition"), true);
    QCOMPARE(store->allKeys("chatCategoryDefinition").count(), 2);
    QCOMPARE(store->contains("chatCategoryDefinition", "iconId"), true);
    QCOMPARE(store->value("chatCategoryDefinition", "iconId"), QString("chat-icon"));
    QCOMPARE(store->contains("chatCategoryDefinition", "feedbackId"), true);
    QCOMPARE(store->value("chatCategoryDefinition", "feedbackId"), QString("sound-file-for-chat"));
    QCOMPARE(store->categoryDefinitionExists("smsCategoryDefinition"), true);
    QCOMPARE(store->allKeys("smsCategoryDefinition").count(), 2);
    QCOMPARE(store->contains("smsCategoryDefinition", "iconId"), true);
    QCOMPARE(store->value("smsCategoryDefinition", "iconId"), QString("sms-icon"));
    QCOMPARE(store->contains("smsCategoryDefinition", "feedbackId"), true);
    QCOMPARE(store->value("smsCategoryDefinition", "feedbackId"), QString("sound-file"));
}
void Ut_CategoryDefinitionStore::testCategoryDefinitionStoreMaxFileSizeHandling()
{
    // Verify if the category definition file is above its maximum size, than its not loaded
    categoryDefinitionFileSize = 90000;
    categoryDefinitionFilesList.append("bigCategoryDefinition.conf");
    QMap<QString, QString> bigCategoryDefinitionSettingsMap;
    bigCategoryDefinitionSettingsMap.insert("iconId", "sms-icon");
    bigCategoryDefinitionSettingsMap.insert("feedbackId", "sound-file");
    categoryDefinitionSettingsMap.insert("bigCategoryDefinition", bigCategoryDefinitionSettingsMap);

    store = new CategoryDefinitionStore("/categorydefinitionpath", 2);
    // Verify that settings object is NULL
    QCOMPARE(store->categoryDefinitionExists("bigCategoryDefinition"), false);
}

void Ut_CategoryDefinitionStore::testCategoryDefinitionUninstalling()
{
    // Create existing category definition file
    categoryDefinitionFilesList.append("smsCategoryDefinition.conf");

    store = new CategoryDefinitionStore("/categorydefinitionpath");
    QSignalSpy uninstallSpy(store, SIGNAL(categoryDefinitionUninstalled(QString)));
    connect(this, SIGNAL(directoryChanged(QString)), store, SLOT(updateCategoryDefinitionFileList()));

    // Add new category definition file
    categoryDefinitionFilesList.append("chatCategoryDefinition.conf");
    emit directoryChanged("/categorydefinitionpath");
    QCOMPARE(uninstallSpy.count(), 0);
    QCOMPARE(store->categoryDefinitionExists("chatCategoryDefinition"), true);

    // Remove the added category definition file
    categoryDefinitionFilesList.removeOne("chatCategoryDefinition.conf");
    emit directoryChanged("/categorydefinitionpath");
    QCOMPARE(uninstallSpy.count(), 1);
    QCOMPARE(store->categoryDefinitionExists("chatCategoryDefinition"), false);

    // Remove the existing category definition file
    categoryDefinitionFilesList.removeOne("smsCategoryDefinition.conf");
    emit directoryChanged("/categorydefinitionpath");
    QCOMPARE(uninstallSpy.count(), 2);
    QCOMPARE(store->categoryDefinitionExists("smsCategoryDefinition"), false);
}

QTEST_APPLESS_MAIN(Ut_CategoryDefinitionStore)
