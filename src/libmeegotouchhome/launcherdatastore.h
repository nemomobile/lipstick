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
#include <QHash>
#include <QStringList>
#include <QFileSystemWatcher>
#include <QTimer>
#include <QFileInfoList>
#include <QSet>

class MDataStore;
class LauncherPage;
class LauncherButton;
class MDesktopEntry;

/*!
 * \class LauncherDataStore
 * \brief LauncherDataStore provides a way to store .desktop entry file
 * related data such as the location of the .desktop entry in the user
 * interface.
 *
 * The data is stored as key-value pairs. Key is entry file path relative to root.
 * (eg. "usr/share/applications/deskentry.desktop").
 * The value is user specific data.
 *
 * The LauncherDataStore uses a MDataStore as a backend to actually store the 
 * the data. The ownership of the MDataStore is transferred to the LauncherDataStore.
 *
 */
class LauncherDataStore : public QObject
{
    Q_OBJECT

public:
    /*!
     * Constructs LauncherDataStore. The ownership of the MDataStore is transferred
     * to this LauncherDataStore.
     *
     * \param dataStore The backend data store. 
     * \param directories The directories to watch.
     */
    LauncherDataStore(MDataStore* dataStore, const QStringList &directories);

    /*!
     * Destroys LauncherDataStore
     */
    virtual ~LauncherDataStore();

    /*!
     * Returns the data for all desktop entries stored in the data store.
     */
    QHash<QString, QVariant> dataForAllDesktopEntries();

    /*!
     * Updates the data for a desktop entry in the data store.
     * Should be called once to initialize the data store.
     *
     * \param entryPath the path of the desktop entry to update
     * \param data the data to update the desktop entry with
     */
    void updateDataForDesktopEntry(const QString &entryPath, const QVariant &data);

    /*!
     * Updates the data for given desktop entries in the data store.
     *
     * \param newValues The desktop entries to update, as 'entry path'-'placement' pairs
     */
    void updateDataForDesktopEntries(const QHash<QString, QString> &newValues);

    /*!
     * Removes the data for a desktop entry in the data store.
     *
     * \param entryPath the path of the desktop entry to remove
     */
    void removeDataForDesktopEntry(const QString &entryPath);

    /*!
      * This helper method gets a key from entry path by adding key prefix to the path.
      *
      * Due QSettings HomeFileDataStore removes preceding forward slash from key.
      * To overcome this we need to use a key prefix in the key.
      *
      * \param entryPath The absolute entry path of the desktop entry (eg. "/usr/share/applications/deskentry.desktop").
      * \return The key for the entry.
      */
    static QString entryPathToKey(const QString &entryPath);

    /*!
      * This helper method gets entry path from key by removing key prefix.
      *
      * Due QSettings HomeFileDataStore removes preceding forward slash from key.
      * To overcome this we need to use a key prefix in the key.
      *
      * \param key The key as key prefix and entry path (eg. "KEY_PREFIX/usr/share/applications/deskentry.desktop").
      * \return The entry path.
      */
    static QString keyToEntryPath(const QString &key);

    /*!
     * Returns \c true if the data store is aware of the desktop entry and
     * has checked it to be an invalid entry. Otherwise returns \c false.
     *
     * \param entryPath the path of the desktop entry to get validity for
     * \return \c true if the desktop entry is known to be invalid, \c false otherwise
     */
    bool isDesktopEntryKnownToBeInvalid(const QString &entryPath) const;

signals:
    /*!
     * \brief A signal for informing that the contents of the data store have changed.
     */
    void dataStoreChanged();

    /*!
     * \brief A signal for informing that the contents of a desktop entry have changed.
     */
    void desktopEntryChanged(const QSharedPointer<MDesktopEntry> &);

    /*!
     * A signal for informing that the a new entry has been added.
     *
     * Added desktop entry path as parameter.
     */
    void desktopEntryAdded(const QSharedPointer<MDesktopEntry> &);

    /*!
     * A signal for informing that the a entry has been removed.
     *
     * Removed desktop entry path as parameter.
     */
    void desktopEntryRemoved(const QString &);

public slots:
    /*!
     * Updates a desktop entry.
     * If file exists, sends a signal about the change. If file does NOT exist, removes path from watcher.
     *
     * \param desktopEntryPath Path to desktop entry that has changed
     */
    void updateDesktopEntry(const QString &desktopEntryPath);

    /*!
     * Updates the contents of the data store from the desktop entries in
     * the watched directories.
     * New .desktop files are added to the data store with no associated data.
     * .desktop files that do not exist anymore are removed from the data store.
     * The dataStoreChanged() signal is emitted after changes are made.
     */
    void updateDesktopEntryFiles();

private:
    /*!
     * Starts processing of the update queue.
     */
    void startProcessingUpdateQueue();

    /*!
     * Checks if desktop entry is valid.
     *
     * \param entryPath Path to desktop entry to be validated
     * \param acceptedTypes List of accepted entry types
     * \return is desktop entry valid
     */
    virtual bool isDesktopEntryValid(const QString &entryPath, const QStringList &acceptedTypes);

    /*!
      * Add path to watcher, if path isn't already watched.
      *
      * This helper method is need as QFileSystemWatcher produces some very annoying
      * debug messages when trying to add already existing paths.
      *
      * \param filePath File path to add to watcher
      */
    void addFilePathToWatcher(const QString &filePath);

    //! A file system watcher for the desktop entry file directory
    QFileSystemWatcher watcher;

    //! List of file types to support for desktop entries.
    QStringList supportedDesktopEntryFileTypes;

    //! The actual data store where the data is stored.
    MDataStore *store;

    //! The directories being watched as canonical paths
    QStringList directories;

    //! Invalid entries in the watched dir
    QStringList invalidEntries;

#ifdef UNIT_TEST
    friend class Ut_LauncherDataStore;
#endif
};



#endif
