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

#ifndef UT_QUICKLAUNCHBAR_H
#define UT_QUICKLAUNCHBAR_H

#include "mdatastore.h"

class QuickLaunchBar;

class TestDataStore : public MDataStore
{
    Q_OBJECT

private:
    QHash<QString, QVariant> values;

public:
    virtual QVariant value(const QString&) const;
    virtual bool setValue(const QString&, const QVariant&);
    virtual QStringList allKeys() const;
    virtual bool contains(const QString&) const;
    virtual bool createValue(const QString&, const QVariant&);
    virtual void remove(const QString&);
    virtual void clear();
};

class Ut_QuickLaunchBar : public QObject
{
    Q_OBJECT

public:
    static bool mkpathCalled;

signals:
    void updateWidgetList();
    void applicationLaunched(const QString &application);
    void mApplicationLaunched(const QString &serviceName);

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    void setupTestSubject(MDataStore *configuration);

    // Test cases
    void testInitialization();
    void testInitializationWithoutDataStore();
    void testRemoveOneApplicationFromFileSystem();
    void testLaunchApplication();
    void testLaunchMApplication();
    void testExternalConfigurationChangeIsNoticed();

private:
    // The object being tested
    QuickLaunchBar *m_subject;
    // The configuration object used by the test subject
    MDataStore *m_configuration;
};

#endif
