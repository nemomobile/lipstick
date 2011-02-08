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

#include <MApplication>
#include "ut_quicklaunchbarview.h"
#include "quicklaunchbar.h"
#include "quicklaunchbarview.h"
#include "homewindowmonitor.h"
#include "launcherbutton_stub.h"
#include "launcheraction_stub.h"
#include "launcherdatastore_stub.h"
#include "windowinfo_stub.h"
#include "mockdatastore.h"
#include <QGraphicsLinearLayout>

void Ut_QuickLaunchBarView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_quicklaunchbarview" };
    app = new MApplication(argc, app_name);
}

void Ut_QuickLaunchBarView::cleanupTestCase()
{
    delete app;
}

void Ut_QuickLaunchBarView::init()
{
    launcherDataStore = new LauncherDataStore(new MockDataStore, QStringList());
    controller = new QuickLaunchBar;
    controller->setLauncherDataStore(launcherDataStore);
    m_subject = new QuickLaunchBarView(controller);
    connect(this, SIGNAL(updateData(const QList<const char *>&)), m_subject, SLOT(updateData(const QList<const char *>&)));
}

void Ut_QuickLaunchBarView::cleanup()
{
    delete controller;
    delete launcherDataStore;
}

void Ut_QuickLaunchBarView::testInitialization()
{
     // Initially there should be 5 widgets but no LauncherButtons
    QMap<int, QSharedPointer<LauncherButton> > buttons;
    QuickLaunchBarModel model;
    m_subject->setModel(&model);
    model.setButtons(buttons);
    QGraphicsLinearLayout *layout = dynamic_cast<QGraphicsLinearLayout *>(controller->layout());
    QVERIFY(layout != NULL);
    QVERIFY(layout->count() > 0);
    QGraphicsLinearLayout *quickLaunchButtonLayout = dynamic_cast<QGraphicsLinearLayout *>(layout->itemAt(0));
    QVERIFY(quickLaunchButtonLayout != NULL);
    QCOMPARE(quickLaunchButtonLayout->count(), 5);
}

void Ut_QuickLaunchBarView::testUpdateData()
{
    QuickLaunchBarModel model;
    m_subject->setModel(&model);
    QMap<int, QSharedPointer<LauncherButton> > buttons;
    QSharedPointer<LauncherButton> button1 = QSharedPointer<LauncherButton>(new LauncherButton);
    QSharedPointer<LauncherButton> button2 = QSharedPointer<LauncherButton>(new LauncherButton);
    QSharedPointer<LauncherButton> button3 = QSharedPointer<LauncherButton>(new LauncherButton);
    buttons.insert(0, button1);
    buttons.insert(1, button2);
    buttons.insert(2, button3);
    model.setButtons(buttons);

    QGraphicsLinearLayout *layout = dynamic_cast<QGraphicsLinearLayout *>(controller->layout());
    QGraphicsLinearLayout *quickLaunchButtonLayout = dynamic_cast<QGraphicsLinearLayout *>(layout->itemAt(0));
    QVERIFY(quickLaunchButtonLayout != NULL);
    // Three buttons + toggleLauncherButton + placeholder widget = 5
    QCOMPARE(quickLaunchButtonLayout->count(), 5);
    QCOMPARE(quickLaunchButtonLayout->itemAt(0), button1.data());
    QCOMPARE(quickLaunchButtonLayout->itemAt(1), button2.data());
    // The toggleLauncherbutton is the third item so buttons3 is fourth
    QCOMPARE(quickLaunchButtonLayout->itemAt(3), button3.data());

}

QTEST_APPLESS_MAIN(Ut_QuickLaunchBarView)
