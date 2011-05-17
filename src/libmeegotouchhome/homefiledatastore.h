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

#ifndef HOMEFILEDATASTORE_H
#define HOMEFILEDATASTORE_H

#include "mdatastore.h"

class HomeFileDataStorePrivate;

/*!
 * Concrete implementation of \c MDataStore interface. This class stores the data to the
 * filesystem. The file name is given as a constructor parameter.
 */
class HomeFileDataStore : public MDataStore
{
    Q_OBJECT
public:
    /*!
     * Constructor.
     * \param filePath Absolute path to the file that the settings will be written to and read from.
     */
    explicit HomeFileDataStore(const QString &filePath);

    /*!
     * Destructor
     */
    virtual ~HomeFileDataStore();

    //! \reimp
    /*!
     * If \c isWritable returns \c false, this method returns \c false.
     */
    virtual bool createValue(const QString &key, const QVariant &value);
    /*!
     * If \c isWritable returns \c false, this method returns \c false.
     */
    virtual bool setValue(const QString &key, const QVariant &value);
    /*!
     * If \c isReadable returns \c false, this method returns an empty QVariant.
     */
    virtual QVariant value(const QString &key) const;
    /*!
     * If \c isReadable returns \c false, this method returns an empty list.
     */
    virtual QStringList allKeys() const;
    /*!
     * If \c isWritable returns \c false, this method does nothing.
     */
    virtual void remove(const QString &key);
    /*!
     * If \c isWritable returns \c false, this method does nothing.
     */
    virtual void clear();
    /*!
     * If \c isReadable returns \c false, this method returns \c false.
     */
    virtual bool contains(const QString &key) const;
    //! \reimp_end

    /*!
     * Queries if this data store is readable. If this method returns \c true you
     * can use the reading methods of this class (\c value, \c allKeys, \c contains).
     * If this method returns \c false, the reading methods don't provide the real data.
     * \sa value, allKeys, contains
     * \return \c true if the data store can be read.
     */
    bool isReadable() const;

    /*!
     * Queries if this data store is writable. If this method returns \c true you
     * can use the writing methods of this class (\c setValue, \c remove, \c clear).
     * If this method returns \c false, the writing methods don't modify the data store.
     * \sa setValue, remove, clear
     * \return \c true if the data store can be written.
     */
    bool isWritable() const;

    /*!
     * Creates values to data store from given hash of key value pairs.
     * With this method QSetting sync (file write) is done only once, which
     * provides better performance then createValue() when storing multiple values.
     *
     * \param newValues the hash of key-value pairs that are updated to data store.
     * \return \c true if the data store can be written
     */
    bool createValues(const QHash<QString, QVariant> &values);

private:
    /*!
     * Takes a snapshot of keys and values in the underlying QSettings.
     */
    void takeSnapshot();

protected:
    HomeFileDataStorePrivate * const d_ptr;

private:
    Q_DECLARE_PRIVATE(HomeFileDataStore)
    Q_DISABLE_COPY(HomeFileDataStore)
};

#endif
