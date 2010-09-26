/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouchhome.
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

#ifndef MOCKDATASTORE_H
#define MOCKDATASTORE_H

#include <mdatastore.h>

class MockDataStore : public MDataStore
{
public:
    virtual bool createValue(const QString &key, const QVariant &value);
    virtual bool setValue(const QString &key, const QVariant &value);
    virtual QVariant value(const QString &key) const;
    virtual QStringList allKeys() const;
    virtual void remove(const QString &key);
    virtual void clear();
    virtual bool contains(const QString &key) const;

private:
    QMap<QString, QVariant> values;

};

bool MockDataStore::createValue(const QString &key, const QVariant &value)
{
    values[key] = value;
    emit valueChanged(key, value);
    return true;
}

bool MockDataStore::setValue(const QString &key, const QVariant &value)
{
    if (allKeys().contains(key)) {
        values[key] = value;
        emit valueChanged(key, value);
        return true;
    } else {
        return false;
    }
}

QVariant MockDataStore::value(const QString &key) const
{
    if (values.contains(key)) {
        return values[key];
    } else {
        return QVariant();
    }
}

QStringList MockDataStore::allKeys() const
{
    return values.keys();
}

void MockDataStore::remove(const QString &key)
{
    values.remove(key);
    emit valueChanged(key, QVariant());
}

void MockDataStore::clear()
{
    values.clear();
}

bool MockDataStore::contains(const QString &key) const
{
    return values.contains(key);
}

#endif
