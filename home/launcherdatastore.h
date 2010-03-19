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

#ifndef LAUNCHERBUTTONDATASTORE_H
#define LAUNCHERBUTTONDATASTORE_H

#include <QSharedPointer>

class DuiDataStore;
class LauncherPage;
class DuiDesktopEntry;

/*!
 * \class LauncherDataStore
 * \brief LauncherDataStore provides a way for the launcher and the quick lauch 
 * bar to store the locations of their item so that the setup can be restored
 * after a reboot
 *
 * The data is stored as key-value pairs. Key is entry file path relative to root.
 * (eg. "usr/share/applications/deskentry.desktop").
 * Value consists of location info: location, page index and position on page (eg. "launcher/1/1").
 *
 * The LauncherDataStore uses a DuiDataStore as a backend to actually store the 
 * the data. The ownership of the DuiDataStore is transferred to the LauncherDataStore.
 *
 * Currently only launcher grid is supported.
 */
class LauncherDataStore
{

public:

    //! The location of the desktop entry
    enum EntryLocation {
        //! The entry location is unknow to the data store
        Unknown,
	//! The entry is defined to be in the launcher grid
	LauncherGrid,
	//! The entry is defined to be in the quick launch bar
	QuickLaunchBar
    };
    
    /*!
     * Constructs LauncherDataStore. The ownership of the DuiDataStore is transferred
     * to this LauncherDataStore.
     *
     * \param dataStore The backend data store. 
     */
    LauncherDataStore(DuiDataStore* dataStore);

    /*!
     * Destroys LauncherDataStore
     */
    virtual ~LauncherDataStore();

    /*!
     * Update launcher button data in data store
     * \param pages List of launcher pages containing the buttons that are stored
     */
    void updateLauncherButtons(const QList< QSharedPointer<LauncherPage> > &pages);

    /*!
     * Get launcher button data from data store. The loaded pages contain launcer buttons
     * and their respective DesktopEntries. However the actions of the laucher buttons are not
     * connected anywhere.
     *
     * return A list of pages consisting of the stored buttons.
     */
    QList< QSharedPointer<LauncherPage> > launcherButtons();

    /*!
     * Returns where this entry is located at. By default the entries are in the launcher grid
     * \param entry The entry whos location is returned.
     * \return The location of the entry
     */
    EntryLocation location(const DuiDesktopEntry &entry);

private:
    /*!
      * This helper method gets a key from entry path by adding key prefix to the path.
      *
      * Due QSettings DuiFileDataStore removes preceding forward slash from key.
      * To overcome this we need to use a key prefix in the key.
      *
      * \param entryPath The absolute entry path of the desktop entry (eg. "/usr/share/applications/deskentry.desktop").
      * \return The key for the entry.
      */
    QString entryPathToKey(QString entryPath);

    /*!
      * This helper method gets entry path from key by removing key prefix.
      *
      * Due QSettings DuiFileDataStore removes preceding forward slash from key.
      * To overcome this we need to use a key prefix in the key.
      *
      * \param key The key as key prefix and entry path (eg. "KEY_PREFIX/usr/shareapplications/deskentry.desktop").
      * \return The entry path.
      */
    QString keyToEntryPath(QString key);

    //! The actual data store where the data is stored.
    DuiDataStore* store;
};

#endif
