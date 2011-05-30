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

#ifndef EXTRADIRWATCHER_H
#define EXTRADIRWATCHER_H

#include "launcherdatastore.h"
#include <MDataStore>

//! Directory and file watcher for installer extra desktop entries
class ExtraDirWatcher : public LauncherDataStore
{
public:
    ExtraDirWatcher(MDataStore *dataStore, const QStringList &directories);
    ~ExtraDirWatcher();

    static const QString DESKTOP_ENTRY_KEY_PACKAGE_STATE;
    static const QString DESKTOP_ENTRY_KEY_PACKAGE_HAD_ERROR;
    static const QString DESKTOP_ENTRY_KEY_PACKAGE_NAME;
    static const QString DESKTOP_ENTRY_KEY_PACKAGE_REMOVABLE;
    static const QString DESKTOP_ENTRY_GROUP_MEEGO;

protected:
    virtual bool isDesktopEntryValid(const MDesktopEntry &entry, const QStringList &acceptedTypes);
};

//! MDataStore implementation for ExtraDirWatcher. Not a persistent storage, but just uses QHash as a dynamic back-end.
class ExtraDirWatcherData : public MDataStore
{
public:
    //! \reimp
    virtual QVariant value (const QString &key) const;
    virtual bool setValue (const QString &key, const QVariant &value);
    virtual QStringList allKeys () const;
    virtual bool contains (const QString &) const;
    virtual bool createValue (const QString &, const QVariant &);
    virtual void remove (const QString &);
    virtual void clear ();
    //! \reimp_end

    QString key(const QString &value);

private:
    QHash<QString, QString> values;
};

#endif
