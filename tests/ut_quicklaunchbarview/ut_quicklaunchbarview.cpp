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

#include <DuiApplication>
#include "ut_quicklaunchbarview.h"
#include "quicklaunchbar.h"
#include "quicklaunchbarview.h"
#include "launcherbutton_stub.h"
#include "launcher_stub.h"
#include <QGraphicsLinearLayout>

void Ut_QuickLaunchBarView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_quicklaunchbarview" };
    app = new DuiApplication(argc, app_name);
}

void Ut_QuickLaunchBarView::cleanupTestCase()
{
    delete app;
}

void Ut_QuickLaunchBarView::init()
{
    controller = new QuickLaunchBar();
    m_subject = new QuickLaunchBarView(controller);
    connect(this, SIGNAL(updateData(const QList<const char *>&)), m_subject, SLOT(updateData(const QList<const char *>&)));
}

void Ut_QuickLaunchBarView::cleanup()
{
    delete controller;
}

void Ut_QuickLaunchBarView::testUpdateData()
{
    QuickLaunchBarModel model;
    m_subject->setModel(&model);
    QList<DuiWidget *> widgets;
    DuiWidget *widget1 = new DuiWidget;
    DuiWidget *widget2 = new DuiWidget;
    DuiWidget *widget3 = new DuiWidget;
    widgets << widget1;
    widgets << widget2;
    widgets << widget3;
    model.setWidgets(widgets);

    QList<const char *> modifications;
    modifications << QuickLaunchBarModel::Widgets;
    emit updateData(modifications);

    QGraphicsLinearLayout *layout = dynamic_cast<QGraphicsLinearLayout *>(controller->layout());
    QVERIFY(layout != NULL);
    QVERIFY(layout->count() > 0);
    QGraphicsLinearLayout *quickLaunchButtonLayout = dynamic_cast<QGraphicsLinearLayout *>(layout->itemAt(0));
    QVERIFY(quickLaunchButtonLayout != NULL);
    // The widgets plus a launcher button should be there
    QCOMPARE(quickLaunchButtonLayout->count(), 4);
    QCOMPARE(quickLaunchButtonLayout->itemAt(0), widget1);
    QCOMPARE(quickLaunchButtonLayout->itemAt(1), widget2);
    // The launcher button is the third item so widget3 is fourth
    QCOMPARE(quickLaunchButtonLayout->itemAt(3), widget3);
}

QTEST_APPLESS_MAIN(Ut_QuickLaunchBarView)
