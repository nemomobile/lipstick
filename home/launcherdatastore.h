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

#ifndef LAUNCHERBUTTONDATASTORE_H
#define LAUNCHERBUTTONDATASTORE_H

#include <QSharedPointer>
#include <QMap>

class MDataStore;
class LauncherPage;
class LauncherButton;
class MDesktopEntry;

/*!
 * \class LauncherDataStore
 * \brief LauncherDataStore provides a way for the launcher and the quick lauch 
 * bar to store the locations of their item so that the setup can be restored
 * after a reboot
 *
 * The data is stored as key-value pairs. Key is entry file path relative to root.
 * (eg. "usr/share/applications/deskentry.desktop").
 * Value consists of location info. When the entry is for launcher:
 *   location, page index and position on page (eg. "launcher/1/1")
 * and when it's for quick launch bar:
 *   location, position on quick launch bar (eg. "quicklaunchbar/3").
 *
 * The LauncherDataStore uses a MDataStore as a backend to actually store the 
 * the data. The ownership of the MDataStore is transferred to the LauncherDataStore.
 *
 */
class LauncherDataStore : public QObject
{
    Q_OBJECT
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

    //! A class for storing, parsing and ordering the placement information of the items
    class Placement {
    public:
        Placement(const QString &placement);

        EntryLocation location;
        int page;
        int position;
    };

    /*!
     * Constructs LauncherDataStore. The ownership of the MDataStore is transferred
     * to this LauncherDataStore.
     *
     * \param dataStore The backend data store. 
     */
    LauncherDataStore(MDataStore* dataStore);

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
     * Get launcher button data from data store. The loaded pages contain launcher buttons
     * and their respective DesktopEntries. However the actions of the launcher buttons are not
     * connected anywhere.
     *
     * \return A list of pages consisting of the stored buttons.
     */
    QList< QSharedPointer<LauncherPage> > launcherButtons();

    /*!
     * Get quick launch bar data from data store. The actions of the launcher buttons are not
     * connected anywhere.
     * \return A list of launcher buttons. The list may contain NULL values, to indicate an empty place.
     */
    QList<LauncherButton*> quickLaunchBarButtons();

    /*!
     * Returns where this entry is located at. By default the entries are in the launcher grid
     * \param entry The entry whos location is returned.
     * \return The location of the entry
     */
    EntryLocation location(const MDesktopEntry &entry);

signals:
    void dataStoreChanged();

private:
    /*!
     * Makes a map of placements of all the entries at specific entry location
     * \param location the location of the entries
     * \return a map of placement to the entry
     */
    QMap<Placement, QString> entryPlacementMap(EntryLocation location);

    /*!
      * This helper method gets a key from entry path by adding key prefix to the path.
      *
      * Due QSettings MFileDataStore removes preceding forward slash from key.
      * To overcome this we need to use a key prefix in the key.
      *
      * \param entryPath The absolute entry path of the desktop entry (eg. "/usr/share/applications/deskentry.desktop").
      * \return The key for the entry.
      */
    QString entryPathToKey(QString entryPath);

    /*!
      * This helper method gets entry path from key by removing key prefix.
      *
      * Due QSettings MFileDataStore removes preceding forward slash from key.
      * To overcome this we need to use a key prefix in the key.
      *
      * \param key The key as key prefix and entry path (eg. "KEY_PREFIX/usr/shareapplications/deskentry.desktop").
      * \return The entry path.
      */
    QString keyToEntryPath(QString key);

    //! The actual data store where the data is stored.
    MDataStore* store;
};

#endif
