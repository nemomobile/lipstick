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

#include "extradirwatcher.h"

#include <MDesktopEntry>

const QString ExtraDirWatcher::DESKTOP_ENTRY_KEY_PACKAGE_STATE = "PackageState";
const QString ExtraDirWatcher::DESKTOP_ENTRY_KEY_PACKAGE_HAD_ERROR = "PackageHadError";
const QString ExtraDirWatcher::DESKTOP_ENTRY_KEY_PACKAGE_NAME = "Package";
const QString ExtraDirWatcher::DESKTOP_ENTRY_KEY_PACKAGE_REMOVABLE = "PackageRemovable";
const QString ExtraDirWatcher::DESKTOP_ENTRY_GROUP_MEEGO = "X-MeeGo";

ExtraDirWatcher::ExtraDirWatcher(MDataStore *dataStore, const QStringList &directories) :
    LauncherDataStore(dataStore, directories)
{
}

ExtraDirWatcher::~ExtraDirWatcher()
{
}

bool ExtraDirWatcher::isDesktopEntryValid(const MDesktopEntry &entry, const QStringList &acceptedTypes)
{
    Q_UNUSED(acceptedTypes);

    return entry.contains(DESKTOP_ENTRY_GROUP_MEEGO, DESKTOP_ENTRY_KEY_PACKAGE_NAME)
        && entry.contains(DESKTOP_ENTRY_GROUP_MEEGO, DESKTOP_ENTRY_KEY_PACKAGE_STATE);
}

QVariant ExtraDirWatcherData::value(const QString &key) const
{
    return values.value(key);
}

bool ExtraDirWatcherData::setValue(const QString &key, const QVariant &value)
{
    values.insert(key, value.toString());
    return true;
}

QStringList ExtraDirWatcherData::allKeys() const
{
    return values.keys();
}

bool ExtraDirWatcherData::contains(const QString &key) const
{
    return values.contains(key);
}

bool ExtraDirWatcherData::createValue(const QString &key, const QVariant &value)
{
    values.insert(key, value.toString());
    return true;
}
void ExtraDirWatcherData::remove (const QString &key)
{
    values.remove(key);
}

void ExtraDirWatcherData::clear ()
{
    values.clear();
}

QString ExtraDirWatcherData::key(const QString &value)
{
    return values.key(value);
}
