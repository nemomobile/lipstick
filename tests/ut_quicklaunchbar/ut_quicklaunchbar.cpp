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

#include <QtTest/QtTest>
#include "ut_quicklaunchbar.h"
#include "quicklaunchbar.h"
#include "launcherbutton_stub.h"
#include "launcherbuttonmodel.h"
#include "launcheraction_stub.h"
#include "launcherdatastore_stub.h"
#include "mockdatastore.h"
#include "windowinfo_stub.h"

void Ut_QuickLaunchBar::initTestCase()
{
}

void Ut_QuickLaunchBar::cleanupTestCase()
{
}

void Ut_QuickLaunchBar::init()
{
    launcherDataStore = new LauncherDataStore(new MockDataStore);

    m_subject = new QuickLaunchBar;
    m_subject->setLauncherDataStore(launcherDataStore);
    connect(this, SIGNAL(updateWidgetList()), m_subject, SLOT(updateButtons()));
    connect(this, SIGNAL(launcherDataStoreChanged()), launcherDataStore, SIGNAL(dataStoreChanged()));
}

void Ut_QuickLaunchBar::cleanup()
{
    delete launcherDataStore;
    launcherDataStore = NULL;
    delete m_subject;
    m_subject = NULL;
}

void Ut_QuickLaunchBar::testLauncherDataStoreChanged()
{
    QHash<QString, QVariant> dataForAllDesktopEntries;
    dataForAllDesktopEntries.insert("validPlacement", QVariant("quicklaunchbar/3"));
    gLauncherDataStoreStub->stubSetReturnValue("dataForAllDesktopEntries", dataForAllDesktopEntries);
    emit launcherDataStoreChanged();
    QCOMPARE(m_subject->model()->buttons().count(), 1);
    QVERIFY(dynamic_cast<LauncherButton *>(m_subject->model()->buttons().value(0).data()) == NULL);
    QVERIFY(dynamic_cast<LauncherButton *>(m_subject->model()->buttons().value(1).data()) == NULL);
    QVERIFY(dynamic_cast<LauncherButton *>(m_subject->model()->buttons().value(2).data()) == NULL);
    QVERIFY(dynamic_cast<LauncherButton *>(m_subject->model()->buttons().value(3).data()) != NULL);
}

QTEST_MAIN(Ut_QuickLaunchBar)
