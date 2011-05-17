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
#include "homefiledatastore.h"
#include "homefiledatastore_p.h"
#include <QTemporaryFile>
#include <QFileInfo>

/*!
 * Creates a temporary file in the directory of an original file.
 * \param originalPath Absolute path that is used as a base for generating
 * the temporary file.
 * \return Path of the created file, or an empty string if creating the
 * file fails. The returned value is a copy, so for uses where it's assigned
 * to a variable, it need not be const. For cases where it's passed as a
 * parameter, it need not be const because references to non-const will
 * not bind to function return values, because they are rvalues. When
 * C++0x brings rvalue references, the value should not be const in order
 * to allow rvalue references to bind to it and thus enable moving from it.
 */
static QString createTempFile(const QString &originalPath)
{
    QString returnValue;
    QTemporaryFile tempFile(originalPath);
    if (tempFile.open()) {
        tempFile.setAutoRemove(false);
        returnValue = tempFile.fileName();
    }
    return returnValue;
}

/*!
 * Copies settings from a QSettings object to another QSettings object.
 * \param originalSettings Settings to copy.
 * \param newSettings Target of the copy.
 * \return true if copying succeeds, false if it fails.
 */
static bool copySettings(const QSettings &originalSettings,
                         QSettings &newSettings)
{
    QStringList keys = originalSettings.allKeys();
    foreach(const QString & key, originalSettings.allKeys()) {
        newSettings.setValue(key, originalSettings.value(key));
        if (newSettings.status() != QSettings::NoError) {
            return false;
        }
    }
    return true;
}

/*!
 * Renames a file. Ensures that a file with the new name
 * doesn't exist.
 * \param oldname Name of the file to rename.
 * \param newName New name.
 */
static void renameSettingFile(const QString &oldName,
                              const QString &newName)
{
    QFile::remove(newName);
    QFile::rename(oldName, newName);
}

/*!
 * Saves the settings to a file. The settings are first
 * saved to a temporary file, and then that file is copied
 * over the original settings. This avoids clearing settings
 * when there's no disk space.
 * \param originalSettings Settings to save.
 */
static bool doSync(QSettings &originalSettings)
{
    bool returnValue = false;
    QString tempFileName = createTempFile(originalSettings.fileName());
    if (!tempFileName.isEmpty()) {
        QSettings copiedSettings(tempFileName, QSettings::IniFormat);
        if (copySettings(originalSettings, copiedSettings)) {
            copiedSettings.sync();
            if (copiedSettings.status() == QSettings::NoError) {
                renameSettingFile(tempFileName, originalSettings.fileName());
                returnValue = true;
            }
        }
    }
    return returnValue;
}

HomeFileDataStorePrivate::HomeFileDataStorePrivate(const QString &filePath) :
    settings(filePath, QSettings::IniFormat)
{
    settings.sync();
}

HomeFileDataStore::HomeFileDataStore(const QString &filePath) :
    d_ptr(new HomeFileDataStorePrivate(filePath))
{
    takeSnapshot();
}

HomeFileDataStore::~HomeFileDataStore()
{
    delete d_ptr;
}

bool HomeFileDataStore::createValue(const QString &key, const QVariant &value)
{
    Q_D(HomeFileDataStore);
    bool returnValue = false;
    // QSettings has some kind of a cache so we'll prevent any temporary writes
    // by checking if the data can be actually stored before doing anything
    if (isWritable()) {
        bool originalValueSet = d->settings.contains(key);
        QVariant originalValue = d->settings.value(key);
        d->settings.setValue(key, value);
        bool syncOk = doSync(d->settings);
        if (syncOk) {
            returnValue = true;
            // Emit valueChanged signal when value is changed or a new key is added
            if ((originalValueSet && originalValue != value)
                    || !originalValueSet) {
                d->settingsSnapshot[key] = value;
                emit valueChanged(key, value);
            }
        } else if (originalValueSet) {
            // if sync fails, make sure the value in memory is the original
            d->settings.setValue(key, originalValue);
        } else {
            d->settings.remove(key);
        }

    }
    return returnValue;
}

bool HomeFileDataStore::setValue(const QString &key, const QVariant &value)
{
    Q_D(HomeFileDataStore);
    bool returnValue = false;
    // QSettings has some kind of a cache so we'll prevent any temporary writes
    // by checking if the data can be actually stored before doing anything
    if (isWritable() && d->settings.contains(key)) {
        QVariant originalValue = d->settings.value(key);
        d->settings.setValue(key, value);
        bool syncOk = doSync(d->settings);
        if (syncOk) {
            returnValue = true;
            // Emit valueChanged signal when value is changed
            if (originalValue != value) {
                d->settingsSnapshot[key] = value;
                emit valueChanged(key, value);
            }
        } else {
            // if sync fails, make sure the value in memory is the original
            d->settings.setValue(key, originalValue);
        }
    }
    return returnValue;
}

QVariant HomeFileDataStore::value(const QString &key) const
{
    Q_D(const HomeFileDataStore);
    return d->settings.value(key);
}

QStringList HomeFileDataStore::allKeys() const
{
    Q_D(const HomeFileDataStore);
    return d->settings.allKeys();
}

void HomeFileDataStore::remove(const QString &key)
{
    Q_D(HomeFileDataStore);
    // QSettings has some kind of a cache so we'll prevent any temporary writes
    // by checking if the data can be actually stored before doing anything
    if (isWritable()) {
        bool originalValueSet = d->settings.contains(key);
        if (!originalValueSet) {
            return;
        }
        QVariant originalValue = d->settings.value(key);
        d->settings.remove(key);
        bool syncOk = doSync(d->settings);
        if (!syncOk) {
            if (originalValueSet) {
                // if sync fails, make sure the value in memory is the original
                d->settings.setValue(key, originalValue);
            }
        } else {
            d->settingsSnapshot.remove(key);
            emit valueChanged(key, QVariant());
        }
    }
}

void HomeFileDataStore::clear()
{
    Q_D(HomeFileDataStore);
    // QSettings has some kind of a cache so we'll prevent any temporary writes
    // by checking if the data can be actually stored before doing anything
    if (isWritable()) {
        d->settings.clear();
        d->settings.sync();
        takeSnapshot();
    }
}

bool HomeFileDataStore::contains(const QString &key) const
{
    Q_D(const HomeFileDataStore);
    return d->settings.contains(key);
}

bool HomeFileDataStore::isReadable() const
{
    Q_D(const HomeFileDataStore);
    return d->settings.status() == QSettings::NoError;
}

bool HomeFileDataStore::isWritable() const
{
    Q_D(const HomeFileDataStore);
    return d->settings.isWritable() && d->settings.status() == QSettings::NoError;
}

void HomeFileDataStore::takeSnapshot()
{
    Q_D(HomeFileDataStore);
    d->settingsSnapshot.clear();
    foreach(const QString & key, d->settings.allKeys()) {
        d->settingsSnapshot.insert(key, d->settings.value(key));
    }
}

bool HomeFileDataStore::createValues(const QHash<QString, QVariant> &values)
{
    Q_D(HomeFileDataStore);
    if (!isWritable()) {
        return false;
    }

    bool returnValue = false;
    // QSettings has a cache so we'll prevent any temporary writes
    // by checking if the data can be actually stored before doing anything
    QHash<QString, QVariant> originalValues;
    foreach (const QString &key, values.keys()) {
        // Collect original values; add also new values with QVariant()
        if (d->settings.contains(key)) {
            originalValues.insert(key, d->settings.value(key));
        } else {
            originalValues.insert(key, QVariant());
        }

        d->settings.setValue(key, values[key]);
    }

    bool syncOk = doSync(d->settings);
    if (syncOk) {
        returnValue = true;
        // Go through list of changed values and emit valueChanged() signals
        foreach (const QString &key, originalValues.keys()) {
            QVariant newValue = d->settings.value(key);
            if (originalValues[key] != newValue) {
                emit valueChanged(key, newValue);
            }
        }
    } else {
        // if sync fails, restore the original values and remove new values
        foreach (const QString &key, originalValues.keys()) {
            if (originalValues[key].isNull()) {
                d->settings.remove(key);
            } else {
                d->settings.setValue(key, originalValues[key]);
            }
        }
    }

    return returnValue;
}
