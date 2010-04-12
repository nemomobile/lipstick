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

#ifndef UT_LAUNCHERDATASTORE_H
#define UT_LAUNCHERDATASTORE_H

#include <QtTest/QtTest>
#include <QObject>
#include <MDataStore>

class MApplication;
class LauncherDataStore;

class TestDataStore : public MDataStore
{
    //! \reimp
    // From MDataAccess
    virtual QVariant value(const QString &key) const;
    virtual bool setValue(const QString &key, const QVariant& value);
    virtual QStringList allKeys() const;
    virtual bool contains(const QString &key) const;

    // From MDataStore
    virtual bool createValue (const QString &key, const QVariant &value);
    virtual void remove (const QString &key);
    virtual void clear();
    //! \reimp_end
    
};


class Ut_LauncherDataStore : public QObject
{
    Q_OBJECT

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    // Test cases
    void testAddingLauncherItemsToAnEmptyStore();
    void testAddingLauncherItemsToNonEmptyStore();
    void testGettingLauncherButtonsFromAnEmptyDataStore();
    void testGettingLauncherButtonsFromDataStore();
    void testLaucherButtonLocation();

private:
    // MApplication
    MApplication *app;
    // The object being tested
    LauncherDataStore *m_subject;
    TestDataStore* testStore;
};

#endif
